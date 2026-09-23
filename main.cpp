#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
using namespace std;


const vector<int> card_amount={13,3,3,6,18,3,4,3,12,2,2,5,3,8,11,3,2,9,6,9,6,3,3,2,3,2};  //牌的種類數量，對應於A~Z的26種牌，每種牌的數量不同
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
            if (count > 0 && !remove_all) cout << "Warning: Not enough " << target_string[0] << " to remove." << endl;
            original_string = result;
        }
        else cout << "Invalid option parameter for mod_cards." << endl;
        return original_string;
    }
public:
    int length;//牌組的長度
    string cards;//牌組的內容

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

    //將一段連續牌彈出成字串，默認彈出1張牌，選項為'f'表示從牌組前面彈出，'b'表示從牌組後面彈出
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

};


class Player{
public:
    int pos;
    int point;
    int status;
    vector<char> Hand;

    void __init__(int _pos, int _point=35000, int _status=0){
        pos = _pos;
        point = _point;
        status = _status;
    }

    void take(CardSet &mountain){
        Hand.push_back(mountain.cards[0]);
        mountain.pop();
    }
    void takecheck(){
        /*
        check long kang
        */
    }
    void operationcheck(){
        /*
        check long chi pong kang
        */
        }
    void throwcard(){
        cout<<"Throw a card, type the place!";
        char input;
        cin >> input;
        while(input<1 || input>14){
            cout<<"f**k type 1~14 u gay!";
            cin>>input;
        }

        

    }
    void chi(){

    }
    void pong(){

    }
    void kang(){

    }
    void long_nia(){

    }
    void reach(){

    }
    void tsumou(){

    }
};


class manager{
public:
    int now;


};


void distribute(CardMountain &moutain , vector<char> &v1 ,  vector<char> &v2  , vector<char> &v3 ,  vector<char> &v4){
    for(int i=0;i<13;i++){
        v1.push_back(moutain.main.cards[i]);
        moutain.main.pop();
    }
    for(int i=13;i<26;i++){
        v2.push_back(moutain.main.cards[i]);
        moutain.main.pop();
    }
    for(int i=26;i<39;i++){
        v3.push_back(moutain.main.cards[i]);
        moutain.main.pop();
    }
    for(int i=39;i<52;i++){
        v4.push_back(moutain.main.cards[i]);
        moutain.main.pop();
    }
    sort(v1.begin(),v1.end());
    sort(v2.begin(),v2.end());
    sort(v3.begin(),v3.end());
    sort(v4.begin(),v4.end());
}

int main() {
    CardMountain mountain;
    //mountain.main.print();
    //mountain.dora.print();
    //mountain.ura_dora.print();
    Player player1;
    Player player2;
    Player player3;
    Player player4;
    distribute(mountain , player1.Hand , player2.Hand , player3.Hand , player4.Hand);
    
    cout<<"player1's handcard"<<":";
    for(int i=0;i<13;i++){
        cout<<player1.Hand[i];
    }
    cout<<'\n';
    cout<<"player2's handcard"<<":";
    for(int i=0;i<13;i++){
        cout<<player2.Hand[i];
    }
    cout<<'\n';
    cout<<"player3's handcard"<<":";
    for(int i=0;i<13;i++){
        cout<<player3.Hand[i];
    }
    cout<<'\n';
    cout<<"player4's handcard"<<":";
    for(int i=0;i<13;i++){
        cout<<player4.Hand[i];
    }
    cout<<'\n';
    cout<<"mountain:";
    mountain.main.print();
    cout<<"dora:";
    mountain.dora.print();
    cout<<"ura_dora:";
    mountain.ura_dora.print();
    
    return 0;
}