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
        Player &rec_player = players[order];
        rec_player.take(mountain, 1);
        rec_player.display_hand();
        rec_player.action_choose();
        stage = 1;
        order = (order + 1) % players.size();
        stage = 0;
    }
    

}