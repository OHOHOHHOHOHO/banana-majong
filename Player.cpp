#include "Player.h"
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;


// Private
void Player::execute_action(char action) {
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


// Public
// 預設：無名，0號位，35000點，狀態0
Player::Player(string _name, int _position, int _point, int _status) {
    name = _name;
    position = _position;
    point = _point;
    status = _status;
}

void Player::initialization(CardMountain &mountain) {
    take(mountain, 13);
    hand.sort();
}

void Player::display_hand() {
    cout << name << "'s hand:\n";
    hand.print();
}

// 預設拿一張牌
void Player::take(CardMountain &mountain, int count) {
    hand.add(mountain.pop(count), 'b');
}

void Player::action_choose() {
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

void Player::action_check() {
    /*
        check:
        吃
        碰
        槓
        立直
        和
    */
}

void Player::throw_card() {
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

void Player::chii() {
}

void Player::pon() {
}

void Player::kan() {
}

void Player::ron_nya() {
}

void Player::reach() {
}

void Player::tsumo() {
}

bool Player::chiiable() {
    return false;
}

bool Player::ponable() {
    return false;
}

bool Player::kanable() {
    return false;
}

bool Player::ronable() {
    return false;
}

bool Player::reachable() {
    return false;
}

bool Player::tsumouable() {
    vector<int> num_of_cards(26, 0); // 記錄每張牌的數目
    for (int i = 0; i < 14; i++) {
        num_of_cards[int(hand.cards[i] - 'A')]++;
    }

    int num_of_pairs = 0; // 對子數量
    for (int i = 0; i < 26; i++) {
        if (num_of_cards[i] == 2) {
            num_of_pairs++;
        }
    }
    if (num_of_pairs == 7) {
        return true;
    }    

    // 註：此和牌檢驗法需要三次判定，如果之後有更好檢驗法，可從此行下方開始更改

    // 判斷1：先從左往右判順子，再刻子

    for (int head = 0; head < 26; head++){
        // 先找雀頭，並將他從牌堆中移除
        if (num_of_cards[head] < 2){
            continue;
        }
        num_of_cards[head] -= 2;

        // 接著判斷順子，然後是刻子
        vector <int> num_of_cards_tmp = num_of_cards;
        int connected_groups = 0;        
        for (int letter = 0; letter < 26; letter++) {
            if(num_of_cards_tmp[letter] > 0){
                if(letter<=23){
                    while(num_of_cards_tmp[letter] > 0 && num_of_cards_tmp[letter + 1] > 0 && num_of_cards_tmp[letter + 2] > 0){
                        num_of_cards_tmp[letter] -= 1;
                        num_of_cards_tmp[letter + 1] -= 1;
                        num_of_cards_tmp[letter + 2] -= 1;
                        connected_groups += 1;
                    }
                }
                if(num_of_cards_tmp[letter] >= 3){
                    connected_groups += num_of_cards_tmp[letter] / 3;
                    num_of_cards_tmp[letter] %= 3;
                }

                if(num_of_cards_tmp[letter]!=0){
                    connected_groups = 0; // 此為找尋不成功的標記，並非找到組數為0
                    break;
                }
            }
        }

        //記得把雀頭加回來
        num_of_cards[head] += 2;

        if(connected_groups == 4){
            return true;
        }
    }
    

    // 判斷2：先從右往左判順子，再刻子
    for (int head = 0; head < 26; head++){
        // 先找雀頭，並將他從牌堆中移除
        if (num_of_cards[head] < 2){
            continue;
        }
        num_of_cards[head] -= 2;

        // 接著判斷順子，然後是刻子
        vector <int> num_of_cards_tmp = num_of_cards;

        //這次從右到左，所以將陣列反轉
        reverse(num_of_cards_tmp.begin(), num_of_cards_tmp.end());

        int connected_groups = 0;        
        for (int letter = 0; letter < 26; letter++) {
            if(num_of_cards_tmp[letter] > 0){
                if(letter<=23){
                    while(num_of_cards_tmp[letter] > 0 && num_of_cards_tmp[letter + 1] > 0 && num_of_cards_tmp[letter + 2] > 0){
                        num_of_cards_tmp[letter] -= 1;
                        num_of_cards_tmp[letter + 1] -= 1;
                        num_of_cards_tmp[letter + 2] -= 1;
                        connected_groups += 1;
                    }
                }
                if(num_of_cards_tmp[letter] >= 3){
                    connected_groups += num_of_cards_tmp[letter] / 3;
                    num_of_cards_tmp[letter] %= 3;
                }

                if(num_of_cards_tmp[letter]!=0){
                    connected_groups = 0; // 此為找尋不成功的標記，並非找到組數為0
                    break;
                }
            }
        }

        //記得把雀頭加回來
        num_of_cards[head] += 2;

        if(connected_groups == 4){
            return true;
        }
    }


    // 判斷3：先抓完刻子再順子
    for (int head = 0; head < 26; head++){
        // 先找雀頭，並將他從牌堆中移除
        if (num_of_cards[head] < 2){
            continue;
        }
        num_of_cards[head] -= 2;

        // 接著判斷刻子，然後是順子(左到右跟右到左是一樣的)
        vector <int> num_of_cards_tmp = num_of_cards;
        int connected_groups = 0;        
        for (int letter = 0; letter < 26; letter++) {
            if(num_of_cards_tmp[letter] > 0){
                if(num_of_cards_tmp[letter] >= 3){
                    connected_groups += num_of_cards_tmp[letter] / 3;
                    num_of_cards_tmp[letter] %= 3;
                }
                
                if(letter<=23){
                    while(num_of_cards_tmp[letter] > 0 && num_of_cards_tmp[letter + 1] > 0 && num_of_cards_tmp[letter + 2] > 0){
                        num_of_cards_tmp[letter] -= 1;
                        num_of_cards_tmp[letter + 1] -= 1;
                        num_of_cards_tmp[letter + 2] -= 1;
                        connected_groups += 1;
                    }
                }

                if(num_of_cards_tmp[letter]!=0){
                    connected_groups = 0; // 此為找尋不成功的標記，並非找到組數為0
                    break;
                }
            }
        }

        //記得把雀頭加回來
        num_of_cards[head] += 2;

        if(connected_groups == 4){
            return true;
        }
    }

    return false;
}