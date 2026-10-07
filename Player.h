#pragma once
#include "CardSet.h"
#include "CardMountain.h"
#include <string>
#include <vector>
#include <utility>
#include <variant>

// 玩家。包含名稱、位置、分數、狀態、手牌、副露、牌河。該類別提供了初始化玩家、抽牌、動作檢查以及各種動作（吃、碰、槓、立直、自摸、丟牌）的功能。
class Player {
private:
    // 定義一個空結構，代表不需要傳參的動作（如 pon, chii 等）
    struct NoArgs {};
    using ActionParam = std::variant<NoArgs, std::string, std::vector<std::pair<char, std::string>>>;
    // 啟動吃,碰,槓,立,和(摸),丟的操作執行函數，並回傳丟出的牌(和牌則回傳0)
    char execute_action(char action, ActionParam param = Player::NoArgs{});

public:
    std::string name;   // 玩家名稱
    int position;       // 玩家位置
    int point;          // 點棒數量
    int status;         // 玩家狀態：0 = 未立直/副露, 1 = 立直, 2 = 已和牌

    CardSet hand;                                // 玩家手牌
    std::vector<std::pair<CardSet, int>> fuuro;  // pair<副露牌組, 餵牌玩家位置>
    CardSet river;                               // 牌河

    // 建構函數
    Player(std::string _name = "", int _position = 0, int _point = 35000, int _status = 0);

    // 初始化，拿13張牌
    void initialization(CardMountain &mountain);
    
    // 秀出手牌
    void display_hand();

    // 判斷是否有副露
    bool is_fuuroed();
    
    // 從牌山拿牌
    void take(CardMountain &mountain, int count = 1);
    
    // 摸牌後讓玩家選擇動作，回傳丟出的牌(和牌則回傳'0')
    char action_choose();
    
    // 詢問玩家的鳴牌意願，回傳期望的鳴牌動作代碼
    char naki_action_ask(char buffer_card = '.');
    
    // 讓玩家輸入要丟的牌並從手牌中移除該牌，回傳丟出的牌
    char throw_card();

    char chii();     // 吃(回傳吃完丟的牌)
    char pon();      // 碰(回傳碰完丟的牌)
    char kan(std::string kanable_cards);      // 槓(回傳槓完丟的牌)
    char ron_nya();  // 和(回傳'0')
    char reach(std::vector<std::pair<char, std::string>> reachable_cards);    // 立直(回傳丟的牌)
    char tsumo();    // 自摸(回傳'0')

    bool chiiable(char buffer_card = '.');     // 可吃
    bool ponable(char buffer_card = '.');      // 可碰
    bool kanable(char buffer_card = '.');      // 可槓(輸入為'@'則檢查暗槓)
    bool ronable(char buffer_card = '.');      // 可和
    std::pair<bool, std::vector<std::pair<char, std::string>>> reachable();     // pair<可立直, pair<捨牌, 聽牌>>
    bool tsumouable(CardSet card_set = CardSet());                       // 可自摸
};