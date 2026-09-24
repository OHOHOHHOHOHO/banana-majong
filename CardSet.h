#pragma once
#include <string>

class CardSet {
private:
    // 根據不同的選項修改牌組的函數，選項包括在牌組前面或後面添加牌、排序牌組、刪除指定數量的牌。
    std::string mod_cards(std::string original_string, std::string target_string, char option, int count = 1);

public:
    int length;        // 牌組的長度
    std::string cards; // 牌組的內容

    // 建構函數，初始化牌組的長度和內容
    CardSet(std::string initial_cards = "");

    // 排序
    void sort();

    // 輸出到終端
    void print();

    // 清空
    void clear();

    // 設置牌組內容
    void set(std::string new_cards);

    // 洗牌
    void shuffle();

    // 添加指定字母牌至牌組
    void add(char card, char option = 'b');
    void add(std::string new_cards, char option = 'b');

    // 刪除牌組內容
    void remove(char card, int count = 1);

    // 將一段連續牌彈出成字串，並從原牌組移除
    std::string pop(int count = 1, char option = 'f');
};