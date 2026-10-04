#include "GameManager.h"

using namespace std;

GameManager::GameManager(CardMountain &_mountain, vector<Player> &_players){
    mountain = _mountain;
    players = _players;
}

// Public
void GameManager::distribute() {
    for (auto &p : players) {
        p.initialization(mountain);
    }
}

void GameManager::start() {
    while(stage != 2){
        Player &rec_player = players[order];    // 當前進行動作的玩家
        rec_player.take(mountain, 1);
        rec_player.display_hand();
        cout << "Dora: " << mountain.get_dora() << "      Cards left: " << mountain.main.length() << "\n\n";
        card_buffer = rec_player.action_choose();
        cout << "You threw: " << card_buffer << "\n";
        cout << "After action, your hand: " << rec_player.hand.cards << "\n\n\n\n";
        
        stage = 1;
        order = (order + 1) % players.size();
        stage = 0;
    }
    

}