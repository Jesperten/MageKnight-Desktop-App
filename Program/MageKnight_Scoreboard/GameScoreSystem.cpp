#include "GameScoreSystem.h"


void addPlayerAction(Action playerAction, Player* player) {

    int tempRepStep = int(player->mRepStep);

    switch (playerAction.mMainActionID) {

    case(ACTION_ID_MAGE_TOWER):
        tempRepStep -= 1;

        if (playerAction.mSuccessFail == true) {
            player->mConquerings++;
            player->mSpellCards += 1;
        }

        break;

    case(ACTION_ID_KEEP):
        tempRepStep -= 1;

        if (playerAction.mSuccessFail == true) {
            player->mConquerings++;
        }

        break;

    case(ACTION_ID_CITY):
        tempRepStep -= 1;
        break;

    case(ACTION_ID_DUNGEON):

        if (playerAction.mSuccessFail == true) {
            player->mAdventureSites++;
            setOptionalDungeonBonus(playerAction.mOptionalID, player);
        }

        break;

    case(ACTION_ID_TOMB):

        if (playerAction.mSuccessFail == true) {
            player->mAdventureSites++;
            player->mSpellCards++;
            player->mArtifacts++;
        }

        break;

    case(ACTION_ID_MONSTER_DEN):

        if (playerAction.mSuccessFail == true) {
            player->mAdventureSites++;
            setOptionalMonsterDenBonus(playerAction.mOptionalID, player);
        }

        break;

    case(ACTION_ID_SPAWNING_GROUNDS):

        if (playerAction.mSuccessFail == true) {
            player->mAdventureSites++;
            setOptionalSpawningGroundsBonus(playerAction.mOptionalID, player);
        }

        break;

    case(ACTION_ID_RUINS):

        if (playerAction.mSuccessFail == true) {
            player->mAdventureSites++;
            setOptionalRuinBonus(playerAction.mOptionalID, player);
        }

        break;

    case(ACTION_ID_MAZE):

        if (playerAction.mSuccessFail == true) {
            player->mAdventureSites++;
            setOptionalMazeBonus(playerAction.mOptionalID, player);
        }

        break;

    case(ACTION_ID_LABYRINTH):

        if (playerAction.mSuccessFail == true) {
            player->mAdventureSites++;
            setOptionalLabyrinthBonus(playerAction.mOptionalID, player);
        }

        break;

    case(ACTION_ID_BURNED_MONASTERY):
        tempRepStep -= 3;

        if (playerAction.mSuccessFail == true) {
            player->mConquerings++;
            player->mArtifacts += 1;
        }

        break;

    case(ACTION_ID_PLUNDERED_VILLAGE):
        tempRepStep -= 1;
        break;

    default:
        break;
    }

    // Add all the additional parts from the action (achieved in other ways than from the basic action itself)
    player->mAActionCards += playerAction.mAddAAC;
    player->mArtifacts    += playerAction.mAddArtifacts;
    player->mCrystals     += playerAction.mAddCrystals;
    player->mFame         += playerAction.mAddFame;
    tempRepStep           += playerAction.mAddRepStep;
    player->mSpellCards   += playerAction.mAddSpells;
    player->mWounds       += playerAction.mWounds;


    // The monsters killed during the action give fame and reputation as follows
    for (unsigned int i = 0; i < playerAction.mMonsters.size(); ++i) {
        player->addMonster(playerAction.mMonsters.at(i));

        player->mFame += playerAction.mMonsters.at(i).mFame;

        if (player->mMonsters.at(i).mSpecial == "Heroes") {
            tempRepStep -= 1;
        }
        else if (player->mMonsters.at(i).mSpecial == "Thugs") {
            tempRepStep += 1;
        }
        else if (player->mMonsters.at(i).mSpecial == "Elementalist") {
            player->mFame += 1;
        }
        else if (player->mMonsters.at(i).mSpecial == "Dark Crusader") {
            player->mFame += 1;
        }

        if (playerAction.mMonsters.at(i).mRampaging) {
            if      (playerAction.mMonsters.at(i).mType == "Marauding Orc") tempRepStep += 1;
            else if (playerAction.mMonsters.at(i).mType == "Draconum")      tempRepStep += 2;
        }
    }

    // Given the (new) calculated Fame, the level of the player should be updated
    setLevel(player->mFame, player);

         if (tempRepStep < 0)  player->mRepStep = 0;
    else if (tempRepStep > 14) player->mRepStep = 14;
    else                       player->mRepStep = unsigned(tempRepStep);

    player->mReputation = repStepToRep(player->mRepStep);

    // Clear Unit vector before (re)adding the units reported in the action
    // This includes all old and new units with updated wound states.
    player->mUnits.resize(0);

    for (unsigned int i = 0; i < playerAction.mUnits.size(); ++i) {
        player->addUnit(playerAction.mUnits.at(i));
    }
}


void setLevel(unsigned int fame, Player* player) {

    unsigned int tempLevel;
    unsigned int deltaLevel;

    if      (fame > 98) tempLevel = 10;
    else if (fame > 79) tempLevel = 9;
    else if (fame > 62) tempLevel = 8;
    else if (fame > 47) tempLevel = 7;
    else if (fame > 34) tempLevel = 6;
    else if (fame > 23) tempLevel = 5;
    else if (fame > 14) tempLevel = 4;
    else if (fame > 7)  tempLevel = 3;
    else if (fame > 2)  tempLevel = 2;
    else                tempLevel = 1;

    deltaLevel = tempLevel - player->mLevel;

    // Check if the level has increased
    if (deltaLevel > 0) {
        // This loop is done to enable multiple added AAC in case of multi-level increase
        for (unsigned int i = 1; i <= deltaLevel; ++i) {
            if ((player->mLevel + i) % 2 == 0) player->mAActionCards += 1; // Gain an advanced action card at each even level (2, 4, 6, 8 etc.)
        }

        player->mLevel = tempLevel;
    }
}

int repStepToRep(unsigned int repStep) {
    // Repstep                       0,  1,  2,  3,  4,  5, 6, 7, 8, 9,10,11,12,13,14
    const std::vector<int> rep = {-100, -5, -3, -2, -1, -1, 0, 0, 0, 1, 1, 2, 2, 3, 5};

   if (repStep > 14) repStep = 14;

    return rep.at(repStep);
}


void updateBasicScores(std::vector<Player>* playerList) {
    for (unsigned int i = 0; i < playerList->size(); i++) {
        // Calculate the leader scores based on the Player Units vector
        unsigned int leaderPoints = 0;

        for (unsigned int j = 0; j < playerList->at(i).mUnits.size(); j++) {
            leaderPoints += playerList->at(i).mUnits.at(j).getScore();
        }

        // Calculate the City points based on the Player City status vector
        unsigned int cityPoints = 0;

        for (unsigned int j = 0; j < playerList->at(i).mCityRelations.size(); ++j)
        {
            switch (playerList->at(i).mCityRelations.at(j)) {
            case CITY_RELATION_NONE:
                break;

            case CITY_RELATION_HAS_TOKENS:
                cityPoints += SCORE_PER_CITY_PARTITION;
                break;

            case CITY_RELATION_IS_TIED_LEADER:
                cityPoints += SCORE_PER_CITY_LEADERSHIP_TIED;
                break;

            case CITY_RELATION_IS_LEADER:
                cityPoints += SCORE_PER_CITY_LEADERSHIP;
                break;
            }
        }


        // Calculate and assign all the basic scores based on the indivudal score ratings/weights
        playerList->at(i).mBasicScoreValues.at(TITLE_GREATEST_KNOWLEDGE)      =    (playerList->at(i).mSpellCards     * SCORE_PER_SPELL   ) + (playerList->at(i).mAActionCards * SCORE_PER_AAC);
        playerList->at(i).mBasicScoreValues.at(TITLE_GREATEST_LOOT)           =    (playerList->at(i).mArtifacts      * SCORE_PER_ARTIFACT) + (playerList->at(i).mCrystals/2   * SCORE_PER_TWO_CRYSTALS);
        playerList->at(i).mBasicScoreValues.at(TITLE_GREATEST_CONQUEROR)      =     playerList->at(i).mConquerings    * SCORE_PER_CONQUERING;
        playerList->at(i).mBasicScoreValues.at(TITLE_GREATEST_ADVENTURER)     =     playerList->at(i).mAdventureSites * SCORE_PER_ADVENTURE_SITE;
        playerList->at(i).mBasicScoreValues.at(TITLE_GREATEST_BEATING)        = int(playerList->at(i).mWounds)        * SCORE_PER_WOUND;
        playerList->at(i).mBasicScoreValues.at(TITLE_GREATEST_LEADER)         =     leaderPoints                      * SCORE_PER_UNIT_LEVEL;
        playerList->at(i).mBasicScoreValues.at(TITLE_GREATEST_CITY_CONQUEROR) =     cityPoints;
    }
}

void updateFinalScores(std::vector<Player>* playerList) {
    int totalScore;

    for (unsigned int i = 0; i < playerList->size(); i++) {
        totalScore = playerList->at(i).mFame;

        for (unsigned int j = 0; j < NUMBER_OF_GREATEST_TITLES; j++) {
            totalScore += playerList->at(i).mBasicScoreValues.at(j);
            totalScore += playerList->at(i).mGreatestTitleScores.at(j);
        }

        // If the temp value is different from the player score, then update the score and add the data to the player score-plot array
        if (totalScore != playerList->at(i).mScore) {
            playerList->at(i).mScore = totalScore;

            // Add two data points in order to keep the step-shape of the graphs
            playerList->at(i).mPointData.push_back(totalScore);
            playerList->at(i).mPointData.push_back(totalScore);
            playerList->at(i).mTimeData.push_back(playerList->at(i).mTimeData.back());
            playerList->at(i).mTimeData.push_back(playerList->at(i).mTimeData.back());
        }
    }
}

void updateCityStats(Action playerAction, City* city, Player* player) {

    unsigned int monsterCount = playerAction.mMonsters.size();

    // Subtract all the rampaging monsters from the count before subtracting from the city monster count
    for (unsigned int i = 0; i < playerAction.mMonsters.size(); ++i) {
        if (playerAction.mMonsters.at(i).mRampaging) --monsterCount;
    }

    city->mMonstersRemaining -= monsterCount;       // Find the number of remaining monsters in the given city
    player->mCityTokens.at(city->mId) += monsterCount; // Assign the number of tokens in the city for the given player
}

void findCityLeader(City* city, std::vector<Player>* playerList) {
    unsigned int maxScore = 0;
    unsigned int playerScore = 0;
    bool tied = false;
    std::vector <unsigned int> cityLeaderIDs;
    QString cityOwner = "";

    // Search through all players and determine the maxScore (number of tokens) and if it is tied between two or more players
    for (unsigned int i = 0; i < playerList->size(); ++i) {
        playerScore = playerList->at(i).mCityTokens.at(city->mId);
        if (playerScore > maxScore) {
            maxScore = playerScore;
            tied = false;
        }
        else if (playerScore == maxScore)
            tied = true;
    }

    // Use the found maxscore to set the city relation status for all players
    for (unsigned int i = 0; i < playerList->size(); ++i) {
        playerScore = playerList->at(i).mCityTokens.at(city->mId);

        if (playerScore == maxScore) {
            // Add the current playerID to the city leader IDs list
            cityLeaderIDs.push_back(i);

            if (tied)
                playerList->at(i).mCityRelations.at(city->mId) = CITY_RELATION_IS_TIED_LEADER;
            else
                playerList->at(i).mCityRelations.at(city->mId) = CITY_RELATION_IS_LEADER;
        }

        else if (playerScore > 0)
            playerList->at(i).mCityRelations.at(city->mId) = CITY_RELATION_HAS_TOKENS;
        else
            playerList->at(i).mCityRelations.at(city->mId) = CITY_RELATION_NONE;
    }

    if (tied) {
        cityOwner = playerList->at(cityLeaderIDs.at(0)).mName;

        for (unsigned int i = 1; i < cityLeaderIDs.size(); ++i) {
            cityOwner += ", ";
            cityOwner += playerList->at(cityLeaderIDs.at(i)).mName;
        }
        cityOwner += " (tied)";
    }
    else {
        cityOwner = playerList->at(cityLeaderIDs.at(0)).mName;
    }

    city->mCityOwner = cityOwner;
}

void findGreatestTitles(std::vector<Player>* playerList, std::vector<QString>* greatestTitlesPlayers) {
    // greatestTitlesPlayers argument is optional. By default this is set as a nullptr.
    // in case greatestTitlesPlayers == nullptr, the string vector will not be manipulated

    unsigned int maxScore = 0;
    unsigned int playerScore = 0;
    bool tied = false;
    bool firstNameWritten = false;

    for (unsigned int i = 0; i < NUMBER_OF_GREATEST_TITLES; ++i) {
        maxScore = 0;
        playerScore = 0;
        tied = false;
        firstNameWritten = false;

        // Search through all players and determine the maxScore and if it is tied between two or more players
        for (unsigned int j = 0; j < playerList->size(); ++j) {
            playerScore = abs(playerList->at(j).mBasicScoreValues.at(i));
            if (playerScore > maxScore) {
                maxScore = playerScore;
                tied = false;
            }
            else if (playerScore == maxScore) {
                tied = true;
            }
        }

        // Use the found maxscore to set the greatest title status for all players
        for (unsigned int j = 0; j < playerList->size(); ++j) {
            playerScore = abs(playerList->at(j).mBasicScoreValues.at(i));

            if (playerScore < maxScore || maxScore == 0) {
                playerList->at(j).setGreatestTitleState(greatest_title_items_t(i), GREATEST_TITLE_NONE, greatestValues.at(i).at(GREATEST_TITLE_NONE));

                // There can be no leader if the found max score is 0.
                if ((maxScore == 0) && (greatestTitlesPlayers != nullptr)) {
                    greatestTitlesPlayers->at(i) = "";
                }
            }
            else if (playerScore == maxScore) {
                if (tied) {
                    playerList->at(j).setGreatestTitleState(greatest_title_items_t(i), GREATEST_TITLE_WIN_TIED, greatestValues.at(i).at(GREATEST_TITLE_WIN_TIED));

                    if ((firstNameWritten) && (greatestTitlesPlayers != nullptr)) {
                        greatestTitlesPlayers->at(i) += ", " + playerList->at(j).mName;
                    }
                }

                else {
                    playerList->at(j).setGreatestTitleState(greatest_title_items_t(i), GREATEST_TITLE_WIN, greatestValues.at(i).at(GREATEST_TITLE_WIN));
                }

                if ((firstNameWritten == false) && (greatestTitlesPlayers != nullptr)) {
                    greatestTitlesPlayers->at(i) = playerList->at(j).mName;
                    firstNameWritten = true;
                }
            }
        }
    }
}

void findRanks(std::vector<Player>* playerList) {
    int maxValue;
    int minValue = 0;
    unsigned int maxIndex;

    // Create a temporary list of the player scores which can be manipulated without affecting the actual scores
    std::vector<int> playerScores(playerList->size());

    // Assign the player scores to the list and find the minimum score
    for (unsigned int i = 0; i < playerList->size(); ++i) {
        playerScores.at(i) = playerList->at(i).mScore;

        if (playerScores.at(i) < minValue) {
            minValue = playerScores.at(i);
        }
    }

    // Find the order/rank of all players by finding the max score
    for (unsigned int i = 0; i < playerList->size(); ++i) {

        // It is important that the init value of maxValue is lower than the found minimum value of all players
        // In this way, all player scores will be found through the loop, as all scores are higher than the init value.
        //maxValue = (minValue <= 0) ? (minValue - 1) : 0; // Set maxValue init value to 0 or minValue if this is lower than 1
        maxValue = minValue - 1;
        maxIndex = 0;

        for (unsigned int j = 0; j < playerList->size(); j++) {
            if (playerScores.at(j) > maxValue) {
                maxValue = playerScores.at(j);
                maxIndex = j;
            }
        }

        // Set the rank to the player with the found index
        // and then set the value below the minimum (init value of maxValue)
        // to avoid choosing this value again in the sorting algorithm
        playerList->at(maxIndex).mRank = i+1; // Best rank is #1
        playerScores.at(maxIndex) = minValue - 1;
    }
}


void setOptionalDungeonBonus(unsigned optionalID, Player* player) {
    switch(optionalID) {
    case (OPTIONAL_DUNGEON_SPELL): // "+1 Spell Card"
        player->mSpellCards += 1;
        break;

    case OPTIONAL_DUNGEON_ARTIFACT: // "+1 Artifact"
        player->mArtifacts += 1;
        break;

    default:
        break;
    }
}

void setOptionalLabyrinthBonus(unsigned optionalID, Player* player) {
    player->mAActionCards += 1;

    switch(optionalID) {
    case (OPTIONAL_LABYRINTH_1_AAC_2_CRYSTALS): // "+2 Crystals"
        player->mCrystals += 2;
        break;

    case OPTIONAL_LABYRINTH_1_AAC_1_SPELL: // "+1 Spell Card"
        player->mSpellCards += 1;
        break;

    case OPTIONAL_LABYRINTH_1_AAC_1_ARTIFACT: // "+1 Artifact"
        player->mArtifacts += 1;
        break;

    default:
        break;
    }
}

void setOptionalMazeBonus(unsigned optionalID, Player* player) {
    switch(optionalID) {
    case (OPTIONAL_MAZE_2_CRYSTALS): // "+2 Crystals"
        player->mCrystals += 2;
        break;

    case OPTIONAL_MAZE_1_SPELL: // "+1 Spell Card"
        player->mSpellCards += 1;
        break;

    case OPTIONAL_MAZE_1_ARTIFACT: // "+1 Artifact"
        player->mArtifacts += 1;
        break;

    default:
        break;
    }
}

void setOptionalMonsterDenBonus(unsigned optionalID, Player* player) {
    switch(optionalID) {
    case (OPTIONAL_MONSTER_DEN_2_CRYSTALS): // "+2 Crystals"
        player->mCrystals += 2;
        break;

    case OPTIONAL_MONSTER_DEN_1_CRYSTAL_1_FAME: // "+1 Crystal, +1 Fame"
        player->mCrystals += 1;
        player->mFame += 1;
        break;

    case OPTIONAL_MONSTER_DEN_2_FAME: // "+2 Fame"
        player->mFame += 2;
        break;

    default:
        break;
    }
}

void setOptionalRuinBonus(unsigned optionalID, Player* player) {
    switch(optionalID) {
    case (OPTIONAL_RUIN_7_FAME): // "+7 Fame"
        player->mFame += 7;
        break;

    case OPTIONAL_RUIN_10_FAME: // "+10 Fame"
        player->mFame += 10;
        break;

    case OPTIONAL_RUIN_1_ARTIFACT: // "+1 Artifact"
        player->mArtifacts += 1;
        break;

    case OPTIONAL_RUIN_2_ARTIFACTS: // "+2 Artifacts"
        player->mArtifacts += 2;
        break;

    case OPTIONAL_RUIN_1_ARTIFACT_1_AAC: // "+1 Artifact, +1 AAC"
        player->mArtifacts += 1;
        player->mAActionCards += 1;
        break;

    case OPTIONAL_RUIN_1_ARTIFACT_1_SPELL: // "+1 Artifact, +1 Spell"
        player->mArtifacts += 1;
        player->mSpellCards += 1;
        break;

    case OPTIONAL_RUIN_1_SPELL_CRYSTALS: // "+1 Spell, +Crystals"
        player->mSpellCards += 1;
        player->mCrystals += 3;
        break;

    case OPTIONAL_RUIN_CRYSTALS: // "+Crystals"
        player->mCrystals += 3;
        break;

    default:
        break;
    }
}

void setOptionalSpawningGroundsBonus(unsigned optionalID, Player* player) {
    player->mArtifacts += 1;

    switch(optionalID) {
    case (OPTIONAL_SPAWNING_GROUNDS_ARTIFACT_3_CRYSTALS): // "+1 Artifact, +3 Crystals"
        player->mCrystals += 3;
        break;

    case OPTIONAL_SPAWNING_GROUNDS_ARTIFACT_2_CRYSTALS_1_FAME: // "+1 Artifact, +2 Crystals, +1 Fame"
        player->mCrystals += 2;
        player->mFame += 1;
        break;

    case OPTIONAL_SPAWNING_GROUNDS_ARTIFACT_1_CRYSTALS_2_FAME: // "+1 Artifact, +1 Crystals, 2 Fame"
        player->mCrystals += 1;
        player->mFame += 3;
        break;

    case OPTIONAL_SPAWNING_GROUNDS_ARTIFACT_3_FAME: // "+1 Artifact, +3 Fame"
        player->mFame += 3;
        break;

    default:
        break;
    }
}



void greatestTitlesScoreSetupClean(void) {
    // Greatest title score definitions are setup in a matrix.

    // TODO: This must be possible to do in a cleaner fashion, by defining it as constant.

    // Resize the matrix to match the "number of titles" x "number of state per title" (7x3)
    greatestValues.resize(NUMBER_OF_GREATEST_TITLES);

    for (unsigned int i = 0; i < NUMBER_OF_GREATEST_TITLES; ++i) {
        greatestValues.at(i).resize(NUMBER_OF_GREATEST_STATES);
    }

    // Define the values in each element manually.
    // ---------------------------------------------------------------------------------------- GREATEST KNOWLEDGE
    greatestValues.at(TITLE_GREATEST_KNOWLEDGE).at(GREATEST_TITLE_NONE)          =  0; // NONE
    greatestValues.at(TITLE_GREATEST_KNOWLEDGE).at(GREATEST_TITLE_WIN_TIED)      =  1; // WIN (TIED)
    greatestValues.at(TITLE_GREATEST_KNOWLEDGE).at(GREATEST_TITLE_WIN)           =  3; // WIN

    // ---------------------------------------------------------------------------------------- GREATEST LOOT
    greatestValues.at(TITLE_GREATEST_LOOT).at(GREATEST_TITLE_NONE)               =  0; // NONE
    greatestValues.at(TITLE_GREATEST_LOOT).at(GREATEST_TITLE_WIN_TIED)           =  1; // WIN (TIED)
    greatestValues.at(TITLE_GREATEST_LOOT).at(GREATEST_TITLE_WIN)                =  3; // WIN

    // ---------------------------------------------------------------------------------------- GREATEST LEADER
    greatestValues.at(TITLE_GREATEST_LEADER).at(GREATEST_TITLE_NONE)             =  0; // NONE
    greatestValues.at(TITLE_GREATEST_LEADER).at(GREATEST_TITLE_WIN_TIED)         =  1; // WIN (TIED)
    greatestValues.at(TITLE_GREATEST_LEADER).at(GREATEST_TITLE_WIN)              =  3; // WIN

    // ---------------------------------------------------------------------------------------- GREATEST CONQUEROR
    greatestValues.at(TITLE_GREATEST_CONQUEROR).at(GREATEST_TITLE_NONE)          =  0; // NONE
    greatestValues.at(TITLE_GREATEST_CONQUEROR).at(GREATEST_TITLE_WIN_TIED)      =  1; // WIN (TIED)
    greatestValues.at(TITLE_GREATEST_CONQUEROR).at(GREATEST_TITLE_WIN)           =  3; // WIN

    // ---------------------------------------------------------------------------------------- GREATEST CITY CONQUEROR
    greatestValues.at(TITLE_GREATEST_CITY_CONQUEROR).at(GREATEST_TITLE_NONE)     =  0; // NONE
    greatestValues.at(TITLE_GREATEST_CITY_CONQUEROR).at(GREATEST_TITLE_WIN_TIED) =  2; // WIN (TIED)
    greatestValues.at(TITLE_GREATEST_CITY_CONQUEROR).at(GREATEST_TITLE_WIN)      =  5; // WIN

    // ---------------------------------------------------------------------------------------- GREATEST ADVENTURER
    greatestValues.at(TITLE_GREATEST_ADVENTURER).at(GREATEST_TITLE_NONE)         =  0; // NONE
    greatestValues.at(TITLE_GREATEST_ADVENTURER).at(GREATEST_TITLE_WIN_TIED)     =  1; // WIN (TIED)
    greatestValues.at(TITLE_GREATEST_ADVENTURER).at(GREATEST_TITLE_WIN)          =  3; // WIN

    // ---------------------------------------------------------------------------------------- GREATEST BEATING
    greatestValues.at(TITLE_GREATEST_BEATING).at(GREATEST_TITLE_NONE)            =  0; // NONE
    greatestValues.at(TITLE_GREATEST_BEATING).at(GREATEST_TITLE_WIN_TIED)        = -1; // WIN (TIED)
    greatestValues.at(TITLE_GREATEST_BEATING).at(GREATEST_TITLE_WIN)             = -3; // WIN
}
