#include "Player.h"
#include <iostream>

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