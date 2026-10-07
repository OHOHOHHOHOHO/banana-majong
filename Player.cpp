#include "Player.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <limits>
#include <utility>

using namespace std;


// Private
char Player::execute_action(char action, ActionParam param) {
    switch (action) {
        case 'c':
            return chii();
        case 'p':
            return pon();
        case 'k':
            return kan(get<string>(param));
        case 'r':
            return reach(get<std::vector<std::pair<char, std::string>>>(param));
        case 't':
            return tsumo();
        case 'd':
            return throw_card();
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

bool Player::is_fuuroed() {
    return !fuuro.empty();
}

// 預設拿一張牌
void Player::take(CardMountain &mountain, int count) {
    hand.add(mountain.pop(count), 'b');
}

char Player::action_choose() {
    const string actions = "krtd";                                                       // 動作代碼
    const string actions_chinese[4] = {"暗槓", "立直", "自摸", "丟牌"};                   // 中文動作名稱
    bool is_action_available[4] = {kanable('@'), reachable().first, tsumouable(), true};   // 動作可行性判定

    string message = "";               // 用於告知可行動作的訊息
    string available_actions = "";     // 用於存儲可行動作的代碼

    for (int i = 0; i < 4; i++) {
        if (is_action_available[i]) {
            message += (message.empty() ? "" : "、") + actions_chinese[i] + "(" + actions[i] + ")";
            available_actions += actions[i];
        }
    }

    cout << "你可以：" << (available_actions == "d" ? "丟牌" : message) << "\n";
    char input;
    if (available_actions == "d") {
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
    
    char thrown_card = execute_action(input);   // 執行動作，並取得丟出的牌
    hand.sort();
    return thrown_card;
}

char Player::naki_action_ask(char buffer_card) {
    if (buffer_card == '.') {
        throw std::invalid_argument("[ERROR] Missing buffer_card parameter for Player::naki_action_ask.");
    }
    if (status != 0) {
        return 'd';                    // 如果玩家已經立直或和牌，則不能鳴牌，直接回傳'd'
    }

    const string actions = "cpktd";                                                      // 動作代碼
    const string actions_chinese[5] = {"吃", "碰", "槓", "和", "取消"};                   // 中文動作名稱
    bool is_action_available[5] = {chiiable(buffer_card), ponable(buffer_card), kanable(buffer_card), ronable(buffer_card), true};   // 動作可行性判定

    string message = "";               // 用於告知可行動作的訊息
    string available_actions = "";     // 用於存儲可行動作的代碼

    for (int i = 0; i < 5; i++) {
        if (is_action_available[i]) {
            message += (message.empty() ? "" : "、") + actions_chinese[i] + "(" + actions[i] + ")";
            available_actions += actions[i];
        }
    }
    if (available_actions == "d") {
        return 'd';                     // 如果沒有鳴牌動作可行，直接回傳'd'
    }
    
    cout << "你可以：" << message << "\n";
    char input;
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
    
    return input;
}

char Player::throw_card() {
    cout << "Throw a card, type an alphabet: ";

    char input;
    cin >> input;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    input = toupper(input);

    while (hand.cards.find(input) == string::npos) {
        cout << "You don't have " << input << " in your hand." << endl;
        cout << "Throw a card, type an alphabet: ";
        cin >> input;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        input = toupper(input);
    }
    hand.remove(input);
    return input;
}

char Player::chii() {
    return ' '; // 這裡應該要有吃牌的邏輯，但目前還沒實作
}

char Player::pon() {
    return ' '; // 這裡應該要有碰牌的邏輯，但目前還沒實作
}

char Player::kan(string kanable_cards) {
    return ' '; // 這裡應該要有槓牌的邏輯，但目前還沒實作
}

char Player::ron_nya() {
    return '0'; // 這裡應該要有和牌的邏輯，但目前還沒實作
}

char Player::reach(std::vector<std::pair<char, std::string>> reachable_cards) {
    return ' '; // 這裡應該要有立直的邏輯，但目前還沒實作
}

char Player::tsumo() {
    return '0'; // 這裡應該要有自摸的邏輯，但目前還沒實作
}

bool Player::chiiable(char buffer_card) {
    if (buffer_card == '.') {
        throw std::invalid_argument("[ERROR] Missing buffer_card parameter for Player::chiiable.");
    }

    // 這裡應該要有吃牌的判斷邏輯，但目前還沒實作
    
    return false;
}

bool Player::ponable(char buffer_card) {
    if (buffer_card == '.') {
        throw std::invalid_argument("[ERROR] Missing buffer_card parameter for Player::ponable.");
    }
    
    // 這裡應該要有碰牌的判斷邏輯，但目前還沒實作
    
    return false;
}

bool Player::kanable(char buffer_card) {
    if (buffer_card == '.') {
        throw std::invalid_argument("[ERROR] Missing buffer_card parameter for Player::kanable.");
    }

    // 這裡應該要有槓牌和暗槓的判斷邏輯，但目前還沒實作
    
    return false;
}

bool Player::ronable(char buffer_card) {
    if (buffer_card == '.') {
        throw std::invalid_argument("[ERROR] Missing buffer_card parameter for Player::ronable.");
    }

    // 這裡應該要有和牌的判斷邏輯與振聽的檢查邏輯，但目前還沒實作

    return false;
}

pair<bool, vector<pair<char, string>>> Player::reachable() {
    CardSet temp_hand = hand;                      // 用於測試的手牌副本
    vector<pair<char, string>> reachable_cards;    // 用於存儲可立直的捨牌及其對應的聽牌組合

    // 如果玩家已經有副露，則不能立直
    if (is_fuuroed()) {
        return {false, reachable_cards};
    }

    for (char org_letter = 'A'; org_letter <= 'Z'; org_letter++) {
        // 如果手牌中沒有這張牌則跳過
        if (temp_hand.cards.find(org_letter) == string::npos) {
            continue;
        }
        for (char new_letter = 'A'; new_letter <= 'Z'; new_letter++) {
            temp_hand.replace(org_letter, new_letter);
            if (tsumouable(temp_hand)) {
                if (reachable_cards.empty() || reachable_cards.back().first != org_letter) {
                    reachable_cards.emplace_back(org_letter, string(1, new_letter));
                }
                else {
                    reachable_cards.back().second += new_letter;
                }
            }
            temp_hand.replace(new_letter, org_letter);
        }
    }

    return {!reachable_cards.empty(), reachable_cards};
}

bool Player::tsumouable(CardSet card_set) {
    if (card_set.length() == 0) {
        card_set = hand;
    }
    if (card_set.length() != 14) {
        return false;
    }
    vector<int> num_of_cards(26, 0); // 記錄每張牌的數目
    for (int i = 0; i < 14; i++) {
        num_of_cards[int(card_set.cards[i] - 'A')]++;
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

    for (int head = 0; head < 26; head++) {
        // 先找雀頭，並將他從牌堆中移除
        if (num_of_cards[head] < 2) {
            continue;
        }
        num_of_cards[head] -= 2;

        // 接著判斷順子，然後是刻子
        vector <int> num_of_cards_tmp = num_of_cards;
        int connected_groups = 0;        
        for (int letter = 0; letter < 26; letter++) {
            if (num_of_cards_tmp[letter] > 0) {
                if (letter <= 23) {
                    while (num_of_cards_tmp[letter] > 0 && num_of_cards_tmp[letter + 1] > 0 && num_of_cards_tmp[letter + 2] > 0) {
                        num_of_cards_tmp[letter] -= 1;
                        num_of_cards_tmp[letter + 1] -= 1;
                        num_of_cards_tmp[letter + 2] -= 1;
                        connected_groups += 1;
                    }
                }
                if (num_of_cards_tmp[letter] >= 3) {
                    connected_groups += num_of_cards_tmp[letter] / 3;
                    num_of_cards_tmp[letter] %= 3;
                }

                if (num_of_cards_tmp[letter] != 0) {
                    connected_groups = 0; // 此為找尋不成功的標記，並非找到組數為0
                    break;
                }
            }
        }

        //記得把雀頭加回來
        num_of_cards[head] += 2;

        if (connected_groups == 4) {
            return true;
        }
    }
    

    // 判斷2：先從右往左判順子，再刻子
    for (int head = 0; head < 26; head++) {
        // 先找雀頭，並將他從牌堆中移除
        if (num_of_cards[head] < 2) {
            continue;
        }
        num_of_cards[head] -= 2;

        // 接著判斷順子，然後是刻子
        vector<int> num_of_cards_tmp = num_of_cards;

        //這次從右到左，所以將陣列反轉
        reverse(num_of_cards_tmp.begin(), num_of_cards_tmp.end());

        int connected_groups = 0;        
        for (int letter = 0; letter < 26; letter++) {
            if (num_of_cards_tmp[letter] > 0) {
                if (letter <= 23) {
                    while (num_of_cards_tmp[letter] > 0 && num_of_cards_tmp[letter + 1] > 0 && num_of_cards_tmp[letter + 2] > 0) {
                        num_of_cards_tmp[letter] -= 1;
                        num_of_cards_tmp[letter + 1] -= 1;
                        num_of_cards_tmp[letter + 2] -= 1;
                        connected_groups += 1;
                    }
                }
                if (num_of_cards_tmp[letter] >= 3) {
                    connected_groups += num_of_cards_tmp[letter] / 3;
                    num_of_cards_tmp[letter] %= 3;
                }

                if (num_of_cards_tmp[letter] != 0) {
                    connected_groups = 0; // 此為找尋不成功的標記，並非找到組數為0
                    break;
                }
            }
        }

        //記得把雀頭加回來
        num_of_cards[head] += 2;

        if (connected_groups == 4) {
            return true;
        }
    }


    // 判斷3：先抓完刻子再順子
    for (int head = 0; head < 26; head++) {
        // 先找雀頭，並將他從牌堆中移除
        if (num_of_cards[head] < 2) {
            continue;
        }
        num_of_cards[head] -= 2;

        // 接著判斷刻子，然後是順子(左到右跟右到左是一樣的)
        vector<int> num_of_cards_tmp = num_of_cards;
        int connected_groups = 0;        
        for (int letter = 0; letter < 26; letter++) {
            if (num_of_cards_tmp[letter] > 0) {
                if (num_of_cards_tmp[letter] >= 3) {
                    connected_groups += num_of_cards_tmp[letter] / 3;
                    num_of_cards_tmp[letter] %= 3;
                }

                if (letter <= 23) {
                    while (num_of_cards_tmp[letter] > 0 && num_of_cards_tmp[letter + 1] > 0 && num_of_cards_tmp[letter + 2] > 0) {
                        num_of_cards_tmp[letter] -= 1;
                        num_of_cards_tmp[letter + 1] -= 1;
                        num_of_cards_tmp[letter + 2] -= 1;
                        connected_groups += 1;
                    }
                }

                if (num_of_cards_tmp[letter] != 0) {
                    connected_groups = 0; // 此為找尋不成功的標記，並非找到組數為0
                    break;
                }
            }
        }

        //記得把雀頭加回來
        num_of_cards[head] += 2;

        if (connected_groups == 4) {
            return true;
        }
    }

    return false;
}