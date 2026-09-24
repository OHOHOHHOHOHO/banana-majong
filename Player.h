#pragma once
#include "CardSet.h"
#include "CardMountain.h"
#include <string>
#include <vector>
#include <utility>

// 建立一個Player類別，包含玩家的名稱、位置、分數、狀態、手牌、副露、牌河。該類別提供了初始化玩家、抽牌、動作檢查以及各種動作（吃、碰、槓、立直、自摸、丟牌）的功能。
class Player {
private:
    // 啟動吃,碰,槓,立,和(摸),丟的操作執行函數
    void execute_action(char action);

public:
    std::string name;   // 玩家名稱
    int position;       // 玩家位置
    int point;          // 點棒數量
    int status;         // 玩家狀態：0 = 門清未立, 1 = 立直, 2 = 副露

    CardSet hand;                                // 玩家手牌
    std::vector<std::pair<CardSet, int>> fuuro;  // 副露牌組 + 餵牌玩家位置
    CardSet river;                               // 牌河

    // 建構函數
    Player(std::string _name = "", int _position = 0, int _point = 35000, int _status = 0);

    // 初始化
    void initialization(CardMountain &mountain);
    
    // 秀出手牌
    void display_hand();
    
    // 從牌山拿牌
    void take(CardMountain &mountain, int count = 1);
    
    // 讓玩家選擇動作
    void action_choose();
    
    // 動作檢查
    void action_check();
    
    // 讓玩家輸入要丟的牌並丟出
    void throw_card();

    // 玩家動作
    void chii();
    void pon();
    void kan();
    void ron_nya();
    void reach();
    void tsumo();

    // 動作可行性判定
    bool chiiable();
    bool ponable();
    bool kanable();
    bool ronable();
    bool reachable();
    bool tsumouable();
};