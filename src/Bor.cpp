#include "Bor.h"

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <utility>

BorNode::BorNode(BorNode* left_child, BorNode* right_child, int16_t value, size_t frequency)
    : left_child(left_child), right_child(right_child), value(value), frequency(frequency) {
}

bool CompareNodes(BorNode* a, BorNode* b) {
    return std::make_pair(a->frequency, a->value) < std::make_pair(b->frequency, b->value);
}

void DeleteBor(BorNode* node) {
    if (node->left_child != nullptr) {
        DeleteBor(node->left_child);
        DeleteBor(node->right_child);
    }
    delete node;
}

void GetLengths(BorNode* node, std::vector<std::pair<int16_t, int16_t>>& answer, int16_t cur_len) {
    if (node->left_child == nullptr) {
        answer.emplace_back(cur_len, node->value);
    } else {
        GetLengths(node->left_child, answer, static_cast<int16_t>(cur_len + 1));
        GetLengths(node->right_child, answer, static_cast<int16_t>(cur_len + 1));
    }
}

void Build(BorNode* nd, int16_t cur_len, std::vector<std::pair<int16_t, int16_t>>& lengths, int16_t& nxt) {
    if (nxt == lengths.size()) {
        return;
    }
    if (cur_len == lengths[nxt].first) {
        nd->value = lengths[nxt].second;
        ++nxt;
        return;
    }
    nd->left_child = new BorNode(nullptr, nullptr, 0, 0);
    nd->right_child = new BorNode(nullptr, nullptr, 0, 0);
    Build(nd->left_child, static_cast<int16_t>(cur_len + 1), lengths, nxt);
    Build(nd->right_child, static_cast<int16_t>(cur_len + 1), lengths, nxt);
}

BorNode* BuildBorByLengths(std::vector<std::pair<int16_t, int16_t>>& lengths) {
    std::sort(lengths.begin(), lengths.end());
    BorNode* parent = new BorNode(nullptr, nullptr, 0, 0);
    int16_t help = 0;
    Build(parent, 0, lengths, help);
    return parent;
}

void GetCodesDFS(BorNode* parent, std::vector<std::vector<unsigned char>>& codes, std::vector<unsigned char>& help) {
    if (parent->left_child == nullptr) {
        codes[parent->value] = help;
        return;
    }
    help.push_back(0);
    GetCodesDFS(parent->left_child, codes, help);
    help.pop_back();
    help.push_back(1);
    GetCodesDFS(parent->right_child, codes, help);
    help.pop_back();
}

void GetCodes(BorNode* parent, std::vector<std::vector<unsigned char>>& codes) {
    std::vector<unsigned char> help;
    GetCodesDFS(parent, codes, help);
}