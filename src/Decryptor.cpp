#include "defines.h"
#include "Bor.h"
#include "Decryptor.h"
#include "Writer.h"

#include <cstdint>

Decryptor::Decryptor(BitReader& reader) : bit_reader_(reader) {
}

bool Decryptor::StartDecryption() {
    while (true) {
        int16_t symbols_count = 0;
        if (!bit_reader_.ReadBits(symbols_count, BitsNeeded)) {
            return false;
        }
        std::vector<std::pair<int16_t, int16_t>> lengths;
        for (int16_t i = 0; i < symbols_count; ++i) {
            int16_t help = 0;
            if (!bit_reader_.ReadBits(help, BitsNeeded)) {
                return false;
            }
            lengths.push_back(std::make_pair(0, help));
        }
        int16_t cnt_read = 0;
        int16_t curr_len = 1;
        while (cnt_read < symbols_count) {
            int16_t help = 0;
            if (!bit_reader_.ReadBits(help, BitsNeeded)) {
                return false;
            }
            for (int16_t i = cnt_read; i < cnt_read + help; ++i) {
                lengths[i].first = curr_len;
            }
            cnt_read = static_cast<int16_t>(cnt_read + help);
            ++curr_len;
        }
        BorNode* parent = BuildBorByLengths(lengths);
        BorNode* current_node = parent;
        std::string next_filename;
        while (true) {
            unsigned char next_bit = 0;
            if (!bit_reader_.ReadBit(next_bit)) {
                DeleteBor(parent);
                return false;
            }
            if (next_bit == 0) {
                current_node = current_node->left_child;
            } else {
                current_node = current_node->right_child;
            }
            if (current_node->left_child == nullptr) {
                if (current_node->value == FilenameEnd) {
                    break;
                }
                next_filename.push_back(static_cast<char>(current_node->value));
                current_node = parent;
            }
        }
        ByteWriter byte_writer;
        if (!byte_writer.Open(next_filename.c_str())) {
            DeleteBor(parent);
            return false;
        }
        current_node = parent;
        bool archive_ended = false;
        while (true) {
            unsigned char next_bit = 0;
            if (!bit_reader_.ReadBit(next_bit)) {
                DeleteBor(parent);
                return false;
            }
            if (next_bit == 0) {
                current_node = current_node->left_child;
            } else {
                current_node = current_node->right_child;
            }
            if (current_node->left_child == nullptr) {
                if (current_node->value == ArchiveEnd || current_node->value == OneMoreFile) {
                    archive_ended = (current_node->value == ArchiveEnd);
                    break;
                }
                byte_writer.WriteByte(current_node->value);
                current_node = parent;
            }
        }
        DeleteBor(parent);
        if (archive_ended) {
            break;
        }
    }
    return true;
}