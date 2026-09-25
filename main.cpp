#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <string>
#include <utility>

#include "CardSet.h"
#include "CardMountain.h"
#include "Player.h"
#include "GameManager.h"

using namespace std;


const vector<int> card_amount={13,3,3,6,18,3,4,3,12,2,2,5,3,8,11,3,2,9,6,9,6,3,3,2,3,2};  // 各種牌的數量
const int card_type_count = card_amount.size();       // 牌的種類數量，即card_amount的大小
const int total_card_amount = []() {                  // 計算總牌數量，將card_amount中的所有數量相加
    int total = 0;
    for (auto i : card_amount) total += i;
    return total;
}();
const string original_mountain = []() {               // 建立原始牌山，將每種牌按照其數量依次添加到牌山中
    string mountain = "";
    for (int i = 0; i < card_type_count; i++) {
        mountain += string(card_amount[i], 'A' + i);
    }
    return mountain;
}();


int ask_num_of_player() {                             // 詢問遊戲人數
    int num;
    do {
        cin >> num;
        if (num < 2 || num > 6) {
            cout << "Invalid number of players. "
                 << "Please enter a number between 2 and 6: ";
        }
    } while (num < 2 || num > 6);
    return num;
}


int main() {
    CardMountain mountain;

    cout << "original mountain: ";
    mountain.main.print();

    cout << "Enter the number of players: ";
    int num_players = ask_num_of_player();

    GameManager game;

    for (int i = 0; i < num_players; i++) {
        string name = "Player" + to_string(i + 1);
        game.players.emplace_back(name, i + 1);
    }

    game.distribute(mountain);

    for (auto &p : game.players) {
        p.display_hand();
    }

    cout << "mountain: ";
    mountain.main.print();

    cout << "dora: ";
    mountain.dora.print();

    cout << "ura_dora: ";
    mountain.ura_dora.print();

    // TODO: 開始遊戲流程

    return 0;
}