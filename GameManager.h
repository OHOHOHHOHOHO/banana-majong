#pragma once
#include "CardSet.h"
#include "CardMountain.h"
#include "Player.h"
#include <vector>
#include <iostream>

// 管理遊戲的進行
class GameManager {
public:
    int order = 0;                // 目前要動作的玩家位置
    int stage = 0;                // 目前狀態 (0 -> 摸牌/打牌階段 、 1 -> 其他玩家吃碰階段)
    char card_buffer = ' ';       // 用來暫存玩家丟出的牌，給其他玩家檢查是否可吃碰槓
    CardMountain mountain;        // 本場使用的牌山
    std::vector<Player> players;  // 玩家列表

    // 建構函數
    GameManager(CardMountain &_mountain , std::vector<Player> &_players);

    // 發牌給所有玩家
    void distribute();

    // 檢查是否可鳴牌
    void check_naki(int player_index, char buffer_card);

    // 遊戲開始
    void start();
};