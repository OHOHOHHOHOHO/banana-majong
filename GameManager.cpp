#include "GameManager.h"

using namespace std;

GameManager::GameManager(CardMountain _mountain, vector<Player> _players){
    mountain = _mountain;
    players = _players;
}

// Public
void GameManager::distribute() {
    for (auto &p : players) {
        p.initialization(mountain);
    }
}