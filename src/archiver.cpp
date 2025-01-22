#include "Encryptor.h"
#include "Decryptor.h"
#include "Reader.h"
#include "Writer.h"

#include <iostream>

bool InitEncrypt(int argc, char** argv) {
    BitWriter bit_writer;
    if (!bit_writer.Open(argv[2])) {
        return false;
    }
    ByteReader byte_reader;
    for (int i = 3; i < argc; ++i) {
        if (!byte_reader.Open(argv[i])) {
            return false;
        }
        Encryptor encryptor(byte_reader, bit_writer);
        if (!encryptor.StartEncryption(i + 1 == argc)) {
            return false;
        }
    }
    return true;
}

int InitDecrypt(char* archive_name) {
    BitReader bit_reader;
    if (!bit_reader.Open(archive_name)) {
        return false;
    }
    Decryptor decryptor(bit_reader);
    if (!decryptor.StartDecryption()) {
        return false;
    }
    return true;
}

constexpr int ErrorCode = 111;

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "dont know what to do :(\n";
        std::cerr << "run \"archiver -h\" for help\n";
        return ErrorCode;
    }
    if (std::string(argv[1]) == "-h") {
        std::cerr << "archiver -c archive_name file1 [file2 ...] - archive files file1,";
        std::cerr << " file2, ... and put it into file archive_name.\n";
        std::cerr << "archiver -d archive_name - unarchive files from ";
        std::cerr << "archive_name and put it into current directory\n";
        return 0;
    }
    if (std::string(argv[1]) == "-c") {
        if (argc < 4) {
            std::cerr << "you need to specify archive_name and at least one file\n";
            std::cerr << "run \"archiver -h\" for help\n";
            return ErrorCode;
        }
        if (!InitEncrypt(argc, argv)) {
            return ErrorCode;
        }
        return 0;
    }
    if (std::string(argv[1]) == "-d") {
        if (argc == 2) {
            std::cerr << "no archive_name specified\n";
            std::cerr << "run \"archiver -h\" for help\n";
            return ErrorCode;
        }
        if (!InitDecrypt(argv[2])) {
            return ErrorCode;
        }
        return 0;
    }
    std::cerr << "unknown argument " << argv[1] << '\n';
    std::cerr << "run \"archiver -h\" for help\n";
    return ErrorCode;
}