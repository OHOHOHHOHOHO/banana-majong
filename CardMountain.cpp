#include "CardMountain.h"
#include <iostream>

using namespace std;


// Public
CardMountain::CardMountain() {
    regenerate();
}

// 洗牌後將後5張牌分別設置為寶牌指示牌和裡寶指示牌，並將已開啟的寶牌指示牌數設置為1
void CardMountain::regenerate() {
    main.set(original_mountain);
    main.shuffle();
    ura_dora = CardSet(main.pop(5, 'b'));
    dora = CardSet(main.pop(5, 'b'));
    opened_dora_count = 1;
}

string CardMountain::get_dora() {
    return dora.cards.substr(0, opened_dora_count);
}

string CardMountain::get_ura_dora() {
    return ura_dora.cards.substr(0, opened_dora_count);
}

// 預設一次開1張
void CardMountain::open_dora(int count) {
    if (opened_dora_count < 5) {
        opened_dora_count += count;
    }
    else {
        cout << "All dora indicators are already opened." << endl;
    }
}

// 預設彈出1張牌，選項為'f'(預設)表示從牌組前面彈出，'b'表示從牌組後面彈出
string CardMountain::pop(int count, char option) {
    return main.pop(count, option);
}