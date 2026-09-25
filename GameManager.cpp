#include "GameManager.h"

using namespace std;


// Public
void GameManager::distribute(CardMountain &mountain) {
    for (auto &p : players) {
        p.initialization(mountain);
    }
}