#pragma once

#include <fstream>
#include <vector>

class BitWriter {
public:
    BitWriter();
    ~BitWriter();

    bool Open(const char* filename);

    bool WriteBit(unsigned char bit_to_write);

private:
    unsigned char current_;
    unsigned char count_;
    std::ofstream output_stream_;
    const char* opened_filename_;
};

bool WriteBits(BitWriter& bit_writer, int16_t value, int16_t amount_to_print);
bool WriteBits(BitWriter& bit_writer, std::vector<unsigned char>& bits_to_write);

class ByteWriter {
public:
    ByteWriter();

    bool Open(const char* filename);

    bool WriteByte(unsigned char byte_to_write);

private:
    std::ofstream output_stream_;
    const char* opened_filename_;
};