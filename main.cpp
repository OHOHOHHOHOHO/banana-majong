#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <string>
#include <utility>

#include "CardSet.h"
#include "CardMountain.h"

using namespace std;


const vector<int> card_amount={13,3,3,6,18,3,4,3,12,2,2,5,3,8,11,3,2,9,6,9,6,3,3,2,3,2};  //各種牌的數量
const int card_type_count = card_amount.size();  //牌的種類數量，即card_amount的大小
const int total_card_amount = []() {  //計算總牌數量，將card_amount中的所有數量相加
    int total = 0;
    for (auto i : card_amount) total += i;
    return total;
}();
const string original_mountain = []() {  //建立原始牌山，將每種牌按照其數量依次添加到牌山中
    string mountain = "";
    for (int i = 0; i < card_type_count; i++) {
        mountain += string(card_amount[i], 'A' + i);
    }
    return mountain;
}();


//建立一個Player類別，包含玩家的位置、分數、狀態以及手牌。該類別提供了初始化玩家、抽牌、檢查長槓、操作檢查、丟牌以及各種操作（吃、碰、槓、長槓、立直、自摸）的功能。
class Player {
private:
    void execute_action(char action) {
        switch (action) {
            case 'c':
                chii();
                break;
            case 'p':
                pon();
                break;
            case 'k':
                kan();
                break;
            case 'r':
                reach();
                break;
            case 't':
                tsumo();
                break;
            case 'd':
                throw_card();
                break;
            default:
                cout << "Invalid operation while choosing action." << endl;
        }
    }
public:
    string name;
    int position;
    int point;
    int status;  // 0 = Menzenchin, 1 = Reach, 2 = Exposed

    CardSet hand;
    vector<pair<CardSet, int>> fuuro;  // 副露牌組 + 餵牌玩家位置
    CardSet river;                     // 牌河

    Player(string _name = "", int _position = 0, int _point = 35000, int _status = 0) {
        name = _name;
        position = _position;
        point = _point;
        status = _status;
    }

    void initialization(CardMountain &mountain) {
        take(mountain, 13);
        hand.sort();
    }

    void display_hand() {
        cout << name << "'s hand:\n";
        hand.print();
    }

    void take(CardMountain &mountain, int count = 1) {
        hand.add(mountain.pop(count), 'b');
    }

    void action_choose() {
        const string actions = "cpkrttd";
        const string actions_chinese[7] = {"吃", "碰", "槓", "立直", "自摸", "和", "丟牌"};
        bool is_action_available[7] = {chiiable(), ponable(), kanable(), reachable(), tsumouable(), ronable(), true};

        string message = "";
        string available_actions = "";

        for (int i = 0; i < 7; i++) {
            if (is_action_available[i]) {
                message += (message.empty() ? "" : ", ") + actions_chinese[i];
                available_actions += actions[i];
            }
        }

        cout << "You can " << message << "\n";
        cout << "Please input what you want to do? \n";
        cout << "(吃->c, 碰->p, 槓->k, 立直->r, 自摸/和->t, 丟牌->d): ";

        char input;
        do {
            cin >> input;
            if (actions.find(input) == string::npos) {
                cout << "Invalid action! Please try again: ";
            }
            else if (available_actions.find(input) == string::npos) {
                cout << "You cannot do that action! Please try again: ";
            }
            else {
                break;
            }
        } while (true);

        execute_action(input);
    }

    void action_check() {
        /*
            check:
            吃
            碰
            槓
            立直
            和
        */
    }

    void throw_card() {
        cout << "Throw a card, type the alphabet: ";

        char input;
        cin >> input;

        if (hand.cards.find(input) != string::npos) {
            hand.remove(input);
            river.add(input, 'b');
            cout << "You threw: " << input << endl;
        }
        else {
            cout << "You don't have that card in your hand." << endl;
        }
    }

    void chii() {
    }

    void pon() {
    }

    void kan() {
    }

    void ron_nya() {
    }

    void reach() {
    }

    void tsumo() {
    }

    bool chiiable() {
        return false;
    }

    bool ponable() {
        return false;
    }

    bool kanable() {
        return false;
    }

    bool ronable() {
        return false;
    }

    bool reachable() {
        return false;
    }

    bool tsumouable() {
        int oc[26] = {0};
        for (int i = 0; i < 14; i++) {
            oc[int(hand.cards[i] - 'A')]++;
        }

        int ch[26];        
        for (int i = 0; i < 26; i++) {
            int group = 0;
            for (int k = 0; k < 26; k++)
                ch[k] = oc[k];
            if (ch[i] >= 2) {
                ch[i] -= 2;
                group++;
                for (int j = 0; j < 26; j++) {
                    while (ch[j] >= 3) {
                        ch[j] -= 3;
                        group++;
                    }
                }
                for (int j = 0; j < 24; j++) {
                    while (ch[j] != 0) {
                        if (ch[j] * ch[j+1] * ch[j+2] != 0) {
                            ch[j]--;
                            ch[j+1]--;
                            ch[j+2]--;
                            group++;
                        }
                        else {
                            break;
                        }
                    }
                }
                if (group == 5) {
                    return true;
                }
            }
        }
    
        int pair7 = 0;
        for (int i = 0; i < 26; i++) {
            if (oc[i] == 2) {
                pair7++;
            }
        }
        if (pair7 == 7) {
            return true;
        }
        return false;
    }
};


class GameManager {
public:
    int order = 1;
    int stage = 0;
    vector<Player> players;

    void distribute(CardMountain &mountain) {
        for (auto &p : players) {
            p.initialization(mountain);
        }
    }
};


int ask_num_of_player() {
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