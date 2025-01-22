#include "defines.h"
#include "Reader.h"

#include <iostream>

BitReader::BitReader() : count_(BitsInByte) {
}

bool BitReader::Open(char* filename) {
    input_stream_.open(filename, std::ios_base::in | std::ios_base::binary);
    if (!input_stream_.good()) {
        std::cerr << "error while opening file " << filename << '\n';
    }
    opened_filename_ = filename;
    return input_stream_.good();
}

bool BitReader::ReadBit(unsigned char& x) {
    bool res = true;
    if (count_ == BitsInByte) {
        current_ = input_stream_.get();
        res = input_stream_.good();
        count_ = 0;
    }
    ++count_;
    x = ((current_ >> (count_ - 1)) & 1);
    if (!res) {
        std::cerr << "error while reading file " << opened_filename_ << '\n';
    }
    return res;
}

bool BitReader::ReadBit(int16_t& x) {
    unsigned char help = 0;
    bool res = ReadBit(help);
    x = static_cast<int16_t>(help);
    return res;
}

ByteReader::ByteReader() : opened_filename(nullptr) {
}

void ByteReader::Close() {
    if (input_stream_.is_open()) {
        input_stream_.close();
    }
}

bool ByteReader::Open(char* filename) {
    Close();
    input_stream_.open(filename, std::ios_base::in | std::ios_base::binary);
    opened_filename = filename;
    if (!input_stream_.good()) {
        std::cerr << "error while opening file " << filename << '\n';
    }
    return input_stream_.good();
}

bool ByteReader::Reopen() {
    input_stream_.clear();
    input_stream_.seekg(0, std::ios::beg);
    if (!input_stream_.good()) {
        std::cerr << "error while operating on file " << opened_filename << '\n';
    }
    return input_stream_.good();
}

bool ByteReader::CheckEOF() {
    input_stream_.peek();
    return input_stream_.eof();
}

bool ByteReader::ReadByte(unsigned char& x) {
    x = input_stream_.get();
    if (!input_stream_.good()) {
        std::cerr << "error while reading file " << opened_filename << '\n';
    }
    return input_stream_.good();
}

bool BitReader::ReadBits(int16_t& num, int16_t cnt) {
    int16_t h = 0;
    num = 0;
    for (int16_t i = 0; i < cnt; ++i) {
        if (!ReadBit(h)) {
            return false;
        }
        num = static_cast<int16_t>(num | (h << i));
    }
    return true;
}