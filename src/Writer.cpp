#include "defines.h"
#include "Writer.h"

#include <iostream>

BitWriter::BitWriter() : current_(0), count_(0) {
}

bool BitWriter::Open(const char* filename) {
    output_stream_.open(filename, std::ios_base::out | std::ios_base::binary);
    if (!output_stream_.good()) {
        std::cerr << "error while opening file " << filename << '\n';
    }
    opened_filename_ = filename;
    return output_stream_.good();
}

bool BitWriter::WriteBit(unsigned char bit_to_write) {
    if (bit_to_write > 0) {
        current_ |= (bit_to_write << count_);
    }
    ++count_;
    if (count_ == BitsInByte) {
        output_stream_.put(static_cast<char>(current_));
        current_ = 0;
        count_ = 0;
    }
    if (!output_stream_.good()) {
        std::cerr << "error while writing to file " << opened_filename_ << '\n';
    }
    return output_stream_.good();
}

BitWriter::~BitWriter() {
    if (count_ > 0) {
        output_stream_.put(static_cast<char>(current_));
        if (!output_stream_.good()) {
            std::cerr << "error while writing to file " << opened_filename_ << '\n';
        }
    }
}

ByteWriter::ByteWriter() {
}

bool ByteWriter::Open(const char* filename) {
    output_stream_.open(filename);
    if (!output_stream_.good()) {
        std::cerr << "error while opening file " << filename << '\n';
    }
    opened_filename_ = filename;
    return output_stream_.good();
}

bool ByteWriter::WriteByte(unsigned char byte_to_write) {
    output_stream_.put(static_cast<char>(byte_to_write));
    if (!output_stream_.good()) {
        std::cerr << "error while writing to file " << opened_filename_ << '\n';
    }
    return output_stream_.good();
}

bool WriteBits(BitWriter& bit_writer, int16_t value, int16_t amount_to_print) {
    for (int16_t i = 0; i < amount_to_print; ++i) {
        if (!bit_writer.WriteBit(value & 1)) {
            return false;
        }
        value >>= 1;
    }
    return true;
}

bool WriteBits(BitWriter& bit_writer, std::vector<unsigned char>& bits_to_write) {
    for (size_t i = 0; i < bits_to_write.size(); ++i) {
        if (!bit_writer.WriteBit(bits_to_write[i])) {
            return false;
        }
    }
    return true;
}