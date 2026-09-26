#pragma once
#include "CardSet.h"
#include "CardMountain.h"
#include "Player.h"
#include <vector>

// 建立GameManager類，以管理遊戲的進行
class GameManager {
public:
    int order = 1;                // (unknown)
    int stage = 0;                // (unknown)
    CardMountain mountain;        // 本場使用的牌山
    std::vector<Player> players;  // 玩家列表

    // 建構函數
    GameManager(CardMountain _mountain , std::vector<Player> _players);

    // 遊戲開始，發牌給所有玩家
    void distribute();
};