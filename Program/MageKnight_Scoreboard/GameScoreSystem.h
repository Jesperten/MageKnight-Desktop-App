#ifndef GAMESCORESYSTEM_H
#define GAMESCORESYSTEM_H

#include "City.h"
#include "Player.h"
#include "Action.h"

void addPlayerAction(Action playerAction, Player* player);
void setLevel(unsigned int fame, Player* player);
int repStepToRep(unsigned int repStep);
void updateBasicScores(std::vector<Player>* playerList);
void updateFinalScores(std::vector<Player>* playerList);
void updateCityStats(Action playerAction, City* city, Player* player);

void findCityLeader(City* city, std::vector<Player>* playerList);
void findGreatestTitles(std::vector<Player>* playerList, std::vector<QString>* greatestTitlesPlayers=nullptr);
void findRanks(std::vector<Player>* playerList);

void setOptionalDungeonBonus(unsigned optionalID, Player* player);
void setOptionalLabyrinthBonus(unsigned optionalID, Player* player);
void setOptionalMazeBonus(unsigned optionalID, Player* player);
void setOptionalMonsterDenBonus(unsigned optionalID, Player* player);
void setOptionalRuinBonus(unsigned optionalID, Player* player);
void setOptionalSpawningGroundsBonus(unsigned optionalID, Player* player);

void greatestTitlesScoreSetupClean(void);

static std::vector<std::vector<int>> greatestValues; // 2D vector of greatest titles scores

const unsigned int SCORE_PER_AAC = 1;
const unsigned int SCORE_PER_SPELL = 2;
const unsigned int SCORE_PER_ARTIFACT = 2;
const unsigned int SCORE_PER_TWO_CRYSTALS = 1;
const unsigned int SCORE_PER_UNIT_LEVEL = 1;
const unsigned int SCORE_PER_CONQUERING = 2;     // Keep, Mage Tower and Monastery
const unsigned int SCORE_PER_ADVENTURE_SITE = 2; // Dungeon, Tomb, Monster Den, Spawning Grounds, Ruins
const          int SCORE_PER_WOUND = -2;

const unsigned int SCORE_PER_CITY_TOKEN = 0;
const unsigned int SCORE_PER_CITY_PARTITION = 4;
const unsigned int SCORE_PER_CITY_LEADERSHIP_TIED = 4;
const unsigned int SCORE_PER_CITY_LEADERSHIP = 7;

#endif // GAMESCORESYSTEM_H
