#include "Reader.h"
#include "Writer.h"

class Encryptor {
public:
    Encryptor(ByteReader& reader, BitWriter& writer);

    bool StartEncryption(bool is_last);

private:
    ByteReader& byte_reader_;
    BitWriter& bit_writer_;
};