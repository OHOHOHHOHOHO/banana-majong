#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <string>
#include <utility>
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


//建立一個CardSet類別，根據不同的選項可以添加、刪除或排序牌組。另外還有一些基本操作，如排序、輸出、清空、洗牌、添加和移除牌等功能。
class CardSet {
private:
    //根據不同的選項修改牌組的函數，選項包括在牌組前面或後面添加牌、排序牌組、刪除指定數量的牌。
    //option: 'b' = add to back, 'f' = add to front, 's' = sort, 'r' = remove
    string mod_cards(string original_string, string target_string, char option, int count = 1) {
        if (option == 'b') {
            original_string += target_string;
        }
        else if (option == 'f') {
            original_string = target_string + original_string;
        }
        else if (option == 's') {
            original_string += target_string;
            std::sort(original_string.begin(), original_string.end());
        }
        else if (option == 'r') {
            string result = "";
            bool remove_all = (count == -1);
            for (char c : original_string) {
                if (c == target_string[0] && (count > 0 || remove_all)) count--;
                else result += c;
            }
            if (count > 0 && !remove_all) {
                cout << "Warning: Not enough " << target_string[0] << " to remove." << endl;
            }
            original_string = result;
        }
        else {
            cout << "Invalid option parameter for mod_cards." << endl;
        }
        return original_string;
    }
public:
    int length;  //牌組的長度
    string cards;  //牌組的內容

    //建構函數，初始化牌組的長度和內容
    CardSet(string initial_cards="") {
        length = initial_cards.length();
        cards = initial_cards;
    }

    //排序
    void sort() {
        std::sort(cards.begin(), cards.end());
    }

    //輸出到終端
    void print() {
        cout << cards << endl;
    }

    //清空
    void clear() {
        cards = "";
        length = 0;
    }

    //設置牌組內容
    void set(string new_cards) {
        cards = new_cards;
        length = cards.length();
    }

    //洗牌
    void shuffle() {
        static random_device random_device;
        static mt19937 generator(random_device());
        std::shuffle(cards.begin(), cards.end(), generator);
    }

    //添加指定字母牌至牌組，默認選項為'b'，表示將新的字母牌添加到牌組的末尾
    void add(char card, char option = 'b') {
        cards = mod_cards(cards, string(1, card), option);
        length = cards.length();
    }
    void add(string new_cards, char option = 'b') {
        cards = mod_cards(cards, new_cards, option);
        length = cards.length();
    }

    //刪除牌組內容，默認刪除1張指定字母的牌
    void remove(char card, int count = 1) {
        cards = mod_cards(cards, string(1, card), 'r', count);
        length = cards.length();
    }

    //將一段連續牌彈出成字串，並從原牌組移除，默認彈出1張牌，選項為'f'表示從牌組前面彈出，'b'表示從牌組後面彈出
    string pop(int count = 1, char option = 'f') {
        if (count > length) {
            cout << "Warning: Not enough cards to pop." << endl;
            count = length;
        }
        if (option == 'f') {
            string popped_cards = cards.substr(0, count);
            cards = cards.substr(count);
            length = cards.length();
            return popped_cards;
        }
        else if (option == 'b') {
            string popped_cards = cards.substr(length - count, count);
            cards = cards.substr(0, length - count);
            length = cards.length();
            return popped_cards;
        }
        else {
            cout << "Invalid option parameter for pop." << endl;
            return "";
        }
    }
};


//建立一個CardMountain類別，包含主要的牌山、寶牌指示牌、裡寶指示牌以及已開啟的寶牌指示牌數。該類別提供了重新生成牌山、獲取寶牌和裡寶指示牌以及開啟寶牌指示牌的功能。
class CardMountain {
public:
    CardSet main;  //主牌山
    CardSet dora;  //寶牌指示牌
    CardSet ura_dora;  //裡寶指示牌
    int opened_dora_count;  //已開啟的寶牌指示牌數

    //建構函數
    CardMountain() {
        regenerate();
    }

    //重生牌山，洗牌後將後5張牌分別設置為寶牌指示牌和裡寶指示牌，並將已開啟的寶牌指示牌數設置為1
    void regenerate() {
        main.set(original_mountain);
        main.shuffle();
        ura_dora = CardSet(main.pop(5, 'b'));
        dora= CardSet(main.pop(5, 'b'));
        opened_dora_count = 1;
    }

    //獲取已開啟的寶牌指示牌
    string get_dora() {
        return dora.cards.substr(0, opened_dora_count);
    }
    //獲取已開啟的裡寶指示牌
    string get_ura_dora() {
        return ura_dora.cards.substr(0, opened_dora_count);
    }

    //開啟寶牌指示牌，最多可以開啟5張
    void open_dora(int count=1) {
        if (opened_dora_count < 5) opened_dora_count += count;
        else cout << "All dora indicators are already opened." << endl;
    }

    //從主牌山中彈出指定數量的牌，默認彈出1張牌，選項為'f'表示從牌組前面彈出，'b'表示從牌組後面彈出
    string pop(int count = 1) {
        return main.pop(count, 'f');
    }
};


//建立一個Player類別，包含玩家的位置、分數、狀態以及手牌。該類別提供了初始化玩家、抽牌、檢查長槓、操作檢查、丟牌以及各種操作（吃、碰、槓、長槓、立直、自摸）的功能。
class Player {
public:
    string name;
    int position;
    int point;
    int status;  // 0 = Menzenchin, 1 = Reach, 2 = Exposed

    CardSet hand;
    vector<pair<CardSet, int>> fuuro;  // 副露牌組 + 餵牌玩家位置
    CardSet river;                     // 牌河
    bool is_riichi;

    Player(string _name = "", int _position = 0, int _point = 35000, int _status = 0) {
        name = _name;
        position = _position;
        point = _point;
        status = _status;
        is_riichi = false;
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

    void selfcheck() {
        string message = "";

        bool tsumou = tsumouable();
        bool kang = kangable();
        bool reach = reachable();

        if (tsumou) {
            message += (message == "" ? "tsumou" : ", tsumou");
        }

        if (kang) {
            message += (message == "" ? "kang" : ", kang");
        }

        if (reach) {
            message += (message == "" ? "reach" : ", reach");
        }

        if (message == "") {
            cout << "You can only discard a card. Good luck!\n";
        }
        else {
            cout << "You can " << message
                << ", or discard a card. Let's Go!\n";
        }

        cout << '\n';

        cout << "Please input what you want to do? "
            << "(tsumou->t, kang->k, reach->r, discard->d): ";

        char input;
        cin >> input;

        while (input != 't' && input != 'k' && input != 'r' && input != 'd') {
            cout << "Invalid action! Please try again: ";
            cin >> input;
        }

        // TODO: 根據 input 執行對應操作
    }

    void operationcheck() {
        /*
            check:
            吃
            碰
            槓
            立直
            和
        */
    }

    void throwcard() {
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

    void riichi() {
    }

    void tsumo() {
    }

    bool chiable() {
        return false;
    }

    bool pongable() {
        return false;
    }

    bool kangable() {
        return false;
    }

    bool longable() {
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

int main() {
    CardMountain mountain;

    cout << "original mountain: ";
    mountain.main.print();

    cout << "Enter the number of players: ";

    int num_players;
    do {
        cin >> num_players;

        if (num_players < 2 || num_players > 6) {
            cout << "Invalid number of players. "
                 << "Please enter a number between 2 and 6: ";
        }
    } while (num_players < 2 || num_players > 6);

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