#pragma once

#include <fstream>

class BitReader {
public:
    BitReader();

    bool Open(char* filename);

    bool ReadBit(unsigned char& x);

    bool ReadBit(int16_t& x);

    bool ReadBits(int16_t& num, int16_t cnt);

private:
    unsigned char current_;
    unsigned char count_;
    std::ifstream input_stream_;
    char* opened_filename_;
};

class ByteReader {
public:
    ByteReader();

    bool Open(char* filename);
    bool Reopen();

    bool CheckEOF();

    void Close();

    bool ReadByte(unsigned char& x);

    char* opened_filename;

private:
    std::ifstream input_stream_;
};