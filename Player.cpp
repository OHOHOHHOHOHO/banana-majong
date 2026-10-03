#include "Player.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <limits>

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
            throw std::invalid_argument("[ERROR] Invalid action parameter for Player::execute_action.");
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
    cout << name << "'s hand: ";
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
            message += (message.empty() ? "" : "、") + actions_chinese[i] + "(" + actions[i] + ")";
            available_actions += actions[i];
        }
    }

    cout << "你可以：" << (available_actions=="d" ? "丟牌" : message) << "\n";

    char input;
    if (available_actions=="d") {
        input = 'd';
    }
    else {
        do {
            cout << "選擇你要執行的動作代碼：";
            cin >> input;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            input = tolower(input);
            if (actions.find(input) == string::npos) {
                cout << "Invalid action!\n";
            }
            else if (available_actions.find(input) == string::npos) {
                cout << "You cannot do that action!\n";
            }
            else {
                break;
            }
        } while (true);
    }
    
    execute_action(input);
    hand.sort();
    cout << "After action, your hand: ";
    hand.print();
    cout << "\n\n";
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
    cout << "Throw a card, type an alphabet: ";

    char input;
    cin >> input;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    input = toupper(input);

    while (hand.cards.find(input) == string::npos){
        cout << "You don't have " << input << " in your hand." << endl;
        cout << "Throw a card, type an alphabet: ";
        cin >> input;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        input = toupper(input);
    }
    hand.remove(input);
    river.add(input, 'b');
    cout << "You threw: " << input << "\n";
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
    for (char letter = 'A'; letter <= 'Z'; letter ++){
        hand.add(letter);
        bool result = tsumouable();
        hand.remove(letter, 1);
        if (result){
            return true;
        }
    }
    
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