#include "Bor.h"
#include "Encryptor.h"
#include "Reader.h"
#include "Writer.h"
#include "defines.h"

#include <cstddef>
#include <cstdint>
#include <set>
#include <string>
#include <vector>

Encryptor::Encryptor(ByteReader& byte_reader, BitWriter& bit_writer)
    : byte_reader_(byte_reader), bit_writer_(bit_writer) {
}

std::string GetFilename(std::string fn) {
    std::string ans;
    size_t fok = 0;
    for (size_t i = 0; i < fn.size(); ++i) {
        if (fn[i] == '/') {
            fok = i + 1;
        }
    }
    for (size_t i = fok; i < fn.size(); ++i) {
        ans.push_back(fn[i]);
    }
    return ans;
}

bool Encryptor::StartEncryption(bool is_last) {
    std::vector<size_t> bytes_count(ValuesBound, 0);
    while (!byte_reader_.CheckEOF()) {
        unsigned char help = 0;
        if (!byte_reader_.ReadByte(help)) {
            return false;
        }
        ++bytes_count[help];
    }
    std::string filename_string(byte_reader_.opened_filename);
    filename_string = GetFilename(filename_string);
    for (unsigned char ch : filename_string) {
        ++bytes_count[ch];
    }
    ++bytes_count[FilenameEnd];
    ++bytes_count[OneMoreFile];
    ++bytes_count[ArchiveEnd];
    std::multiset<BorNode*, decltype(&CompareNodes)> node_queue(&CompareNodes);
    int16_t symbols_count = 0;
    for (int16_t i = 0; i < ValuesBound; ++i) {
        if (bytes_count[i] > 0) {
            ++symbols_count;
            node_queue.insert(new BorNode(nullptr, nullptr, i, bytes_count[i]));
        }
    }
    while (node_queue.size() > 1) {
        BorNode* first_min = (*node_queue.begin());
        node_queue.erase(node_queue.begin());
        BorNode* second_min = (*node_queue.begin());
        node_queue.erase(node_queue.begin());
        node_queue.insert(new BorNode(first_min, second_min, 0, first_min->frequency + second_min->frequency));
    }
    BorNode* parent = (*node_queue.begin());
    node_queue.clear();
    std::vector<std::pair<int16_t, int16_t>> lengths;
    GetLengths(parent, lengths, 0);
    DeleteBor(parent);
    parent = BuildBorByLengths(lengths);
    std::vector<std::vector<unsigned char>> codes(ValuesBound);
    GetCodes(parent, codes);
    DeleteBor(parent);
    if (!WriteBits(bit_writer_, symbols_count, BitsNeeded)) {
        return false;
    }
    for (std::pair<int16_t, int16_t> p : lengths) {
        if (!WriteBits(bit_writer_, p.second, BitsNeeded)) {
            return false;
        }
    }
    size_t max_symbol_code_size = 0;
    for (int16_t i = 0; i < ValuesBound; ++i) {
        if (bytes_count[i] > 0) {
            max_symbol_code_size = std::max(max_symbol_code_size, codes[i].size());
        }
    }
    std::vector<size_t> sizes_count(max_symbol_code_size + 1, 0);
    for (int16_t i = 0; i < ValuesBound; ++i) {
        if (bytes_count[i] > 0) {
            ++sizes_count[codes[i].size()];
        }
    }
    for (size_t i = 1; i <= max_symbol_code_size; ++i) {
        if (!WriteBits(bit_writer_, static_cast<int16_t>(sizes_count[i]), BitsNeeded)) {
            return false;
        }
    }
    for (unsigned char ch : filename_string) {
        if (!WriteBits(bit_writer_, codes[ch])) {
            return false;
        }
    }
    if (!WriteBits(bit_writer_, codes[FilenameEnd])) {
        return false;
    }
    if (!byte_reader_.Reopen()) {
        return false;
    }
    while (!byte_reader_.CheckEOF()) {
        unsigned char help = 0;
        if (!byte_reader_.ReadByte(help)) {
            return false;
        }
        if (!WriteBits(bit_writer_, codes[help])) {
            return false;
        }
    }
    if (is_last) {
        if (!WriteBits(bit_writer_, codes[ArchiveEnd])) {
            return false;
        }
    } else {
        if (!WriteBits(bit_writer_, codes[OneMoreFile])) {
            return false;
        }
    }
    return true;
}