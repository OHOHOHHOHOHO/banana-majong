#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
using namespace std;


const vector<int> card_amount={13,3,3,6,18,3,4,3,12,2,2,5,3,8,11,3,2,9,6,9,6,3,3,2,3,2};
const int card_type_count = card_amount.size();
const int total_card_amount = []() {
    int total = 0;
    for (auto i : card_amount) total += i;
    return total;
}();
const string original_mountain = []() {
    string mountain = "";
    for (int i = 0; i < card_type_count; i++) {
        mountain += string(card_amount[i], 'A' + i);
    }
    return mountain;
}();



class CardSet {
private:
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
    int length;
    string cards;

    CardSet(string initial_cards="") {
        length = initial_cards.length();
        cards = initial_cards;
    }

    void sort() {
        std::sort(cards.begin(), cards.end());
    }

    void print() {
        cout << cards << endl;
    }

    void clear() {
        cards = "";
        length = 0;
    }

    void set(string new_cards) {
        cards = new_cards;
        length = cards.length();
    }

    void shuffle() {
        static random_device random_device;
        static mt19937 generator(random_device());
        std::shuffle(cards.begin(), cards.end(), generator);
    }

    void add(char card, char option = 'b') {
        cards = mod_cards(cards, string(1, card), option);
        length = cards.length();
    }
    void add(string new_cards, char option = 'b') {
        cards = mod_cards(cards, new_cards, option);
        length = cards.length();
    }

    void remove(char card, int count = 1) {
        cards = mod_cards(cards, string(1, card), 'r', count);
        length = cards.length();
    }

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



class CardMountain {
public:
    CardSet main;
    CardSet dora;
    CardSet ura_dora;
    int opened_dora_count;

    CardMountain() {
        regenerate();
    }

    void regenerate() {
        main.set(original_mountain);
        main.shuffle();
        ura_dora = CardSet(main.pop(5, 'b'));
        dora= CardSet(main.pop(5, 'b'));
        opened_dora_count = 1;
    }

    string get_dora() {
        return dora.cards.substr(0, opened_dora_count);
    }

    string get_ura_dora() {
        return ura_dora.cards.substr(0, opened_dora_count);
    }

    void open_dora(int count=1) {
        if (opened_dora_count < 5) opened_dora_count += count;
        else cout << "All dora indicators are already opened." << endl;
    }

};


void move_cards(CardSet &from, CardSet &to, char option_from='f', char option_to='b',int count=1) {
    string change=from.pop(count,option_from);
    to.add(change,option_to);
}

class Player{
public:
    string name;
    int pos; // order-> 1, 2, 3, 4, 1, ...
    int point = 35000;
    int status = 0; // 0 -> Menzenchin, 1 -> Reach, 2 -> exposed
    CardSet hand;

    Player(string _name, int _pos){
        name = _name;
        pos = _pos;
    }

    void intialization(CardMountain &cardmountain){
        hand.add(cardmountain.main.cards.substr(0, 13));
        cardmountain.main.pop(13, 'f');
        hand.sort();
    }

    void display_hand(){
        cout << name << "'s hand:\n";
        cout << hand.cards << '\n';
    }

    void draw(CardSet &cardmountain){
        hand.add(cardmountain.cards[0], 'b');
        cardmountain.pop();
        display_hand();
        selfcheck();

    }
    
    void selfcheck(){
        //check tsumou?, kang?, reach?
        string message = "";
        
        if(tsumouable()){
            message += (message==""? "tsumou" : ", tsumou");
        }
        if(kangable()){
            message += (message==""? "kang" : ", kang");
        }
        if(reachable()){
            message += (message==""? "reach" : ", reach");
        }

        if(message==""){
            cout << "You can only discard a card. Good luck!\n";
        }
        else {
            cout << "You can " << message << ", or discard a card. Let's Go!\n"; 
        }
        
    }
    void operationcheck(){
        
        //check long chi pong kang
        
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
    bool chiable(){
        return false;
    }
    bool pongable(){
        return false;
    }
    bool kangable(){
        return false;
    }
    bool longable(){
        return false;
    }
    bool reachable(){
        return false;
    }
    bool tsumouable(){
        return false;
    }


};


class GameManager{
public:
    int order=1;
    int stage=0;
    vector <Player> player;
    
    void __init__(Player &p1, Player &p2, Player &p3, Player &p4){
        player.push_back(p1);
        player.push_back(p2);
        player.push_back(p3);
        player.push_back(p4);
    }

    void distribute(CardMountain &cardmountain){
        for (int i=0; i<player.size(); i++){
            player[i].intialization(cardmountain); 
        }
    }

    
};



int main() {
    CardMountain mountain;
    mountain.main.print();
    mountain.dora.print();
    mountain.ura_dora.print();
    Player p1("kleeplayer", 1);
    Player p2("loliplayer", 2);
    Player p3("fireflyplayer", 3);
    Player p4("fubukiplayer", 4);
    GameManager gamemanager;


    gamemanager.__init__(p1, p2, p3, p4);
    gamemanager.distribute(mountain);

    for (int i=0; i<gamemanager.player.size(); i++){
        gamemanager.player[i].display_hand();
    }

    cout<<"mountain:";
    mountain.main.print();
    cout<<"dora:";
    mountain.dora.print();
    cout<<"ura_dora:";
    mountain.ura_dora.print();
    
    gamemanager.player[0].draw(mountain.main);
    return 0;
}