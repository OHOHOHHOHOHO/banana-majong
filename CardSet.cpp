#include "CardSet.h"
#include <iostream>
#include <algorithm>
#include <random>
#include <stdexcept>

using namespace std;


// Private
// option: 'b' = add to back, 'f' = add to front, 's' = sort, 'r' = remove
string CardSet::mod_cards(string original_string, string target_string, char option, int count) {
    if (option == 'b') {
        original_string += target_string;
    }
    else if (option == 'f') {
        original_string = target_string + original_string;
    }
    else if (option == 's') {
        original_string += target_string;
        std::sort(original_string.begin(), original_string.end());
    }
    else if (option == 'r') {
        string result = "";
        bool remove_all = (count == -1);
        for (char c : original_string) {
            if (c == target_string[0] && (count > 0 || remove_all)) {
                count--;
            }
            else {
                result += c;
            }
        }
        if (count > 0 && !remove_all) {
            throw std::out_of_range("[ERROR] Not enough cards " + target_string + " to remove while executing CardSet::mod_cards.");
        }
        original_string = result;
    }
    else {
        throw std::invalid_argument("[ERROR] Invalid option parameter for CardSet::mod_cards.");
    }
    return original_string;
}


// Public
CardSet::CardSet(string initial_cards) {
    cards = initial_cards;
}

int CardSet::length() {
    return cards.length();
}

void CardSet::sort() {
    std::sort(cards.begin(), cards.end());
}

void CardSet::print() {
    cout << cards << endl;
}

void CardSet::clear() {
    cards = "";
}

void CardSet::set(string new_cards) {
    cards = new_cards;
}

void CardSet::shuffle() {
    static random_device rd;
    static mt19937 generator(rd());
    std::shuffle(cards.begin(), cards.end(), generator);
}

// 預設選項為'b'，表示將新的字母牌添加到牌組的末尾
void CardSet::add(char card, char option) {
    cards = mod_cards(cards, string(1, card), option);
}
void CardSet::add(string new_cards, char option) {
    cards = mod_cards(cards, new_cards, option);
}

// 預設刪除1張指定字母的牌
void CardSet::remove(char card, int count) {
    cards = mod_cards(cards, string(1, card), 'r', count);
}

// 預設替換1張指定字母的牌
void CardSet::replace(char old_card, char new_card, int count) {
    for (char& c : cards) {
        if (c == old_card) {
            c = new_card;
            count--;
            if (count == 0) {
                return;
            }
        }
    }
    if (count > 0) {
        throw std::out_of_range("[ERROR] Not enough cards " + string(1, old_card) + " to replace while executing CardSet::replace.");
    }
}

// 預設彈出1張牌，選項為'f'(預設)表示從牌組前面彈出，'b'表示從牌組後面彈出
string CardSet::pop(int count, char option) {
    if (count > length()) {
        throw std::out_of_range("[ERROR] Not enough cards to pop while executing CardSet::pop.");
    }
    if (option == 'f') {
        string popped_cards = cards.substr(0, count);
        cards = cards.substr(count);
        return popped_cards;
    }
    else if (option == 'b') {
        string popped_cards = cards.substr(length() - count, count);
        cards = cards.substr(0, length() - count);
        return popped_cards;
    }
    else {
        throw std::invalid_argument("[ERROR] Invalid option parameter for CardSet::pop.");
    }
}