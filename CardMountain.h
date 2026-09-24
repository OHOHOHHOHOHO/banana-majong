#pragma once
#include "CardSet.h"
#include <string>

// original_mountain 在外部宣告
extern const std::string original_mountain;

// 建立一個CardMountain類別，包含主要的牌山、寶牌指示牌、裡寶指示牌以及已開啟的寶牌指示牌數。該類別提供了重新生成牌山、獲取寶牌和裡寶指示牌以及開啟寶牌指示牌的功能。
class CardMountain {
public:
    CardSet main;               // 主牌山
    CardSet dora;               // 寶牌指示牌
    CardSet ura_dora;           // 裡寶指示牌
    int opened_dora_count;      // 已開啟的寶牌指示牌數

    // 建構函數
    CardMountain();

    // 重生牌山
    void regenerate();

    // 獲取已開啟的寶牌指示牌
    std::string get_dora();

    // 獲取已開啟的裡寶指示牌
    std::string get_ura_dora();

    // 開啟寶牌指示牌，最多可以開啟5張
    void open_dora(int count = 1);

    // 從主牌山中彈出指定數量的牌
    std::string pop(int count = 1, char option = 'f');
};