#include <cstddef>
#include <cstdint>
#include <vector>

struct BorNode {
    BorNode* left_child;
    BorNode* right_child;
    int16_t value;
    size_t frequency;

    BorNode(BorNode* left_child, BorNode* right_child, int16_t value, size_t frequency);
};

bool CompareNodes(BorNode* a, BorNode* b);

void DeleteBor(BorNode* parent);

void GetLengths(BorNode* node, std::vector<std::pair<int16_t, int16_t>>& answer, int16_t cur_len);

BorNode* BuildBorByLengths(std::vector<std::pair<int16_t, int16_t>>& lengths);

void GetCodes(BorNode* parent, std::vector<std::vector<unsigned char>>& codes);