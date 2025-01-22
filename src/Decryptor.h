#include "Reader.h"

class Decryptor {
public:
    explicit Decryptor(BitReader& reader);

    bool StartDecryption();

private:
    BitReader& bit_reader_;
};