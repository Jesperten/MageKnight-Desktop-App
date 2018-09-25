#include <vector>
#include <QVector>
#include "MainBoard.h"
#include "ui_MainBoard.h"
#include "Player.h"
#include "AreYouSureDialog.h"
#include "UserActionDialog.h"
#include "AddCityDialog.h"
#include "City.h"

MainBoard::MainBoard(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainBoard)
{
    ui->setupUi(this);

    playerTableSetupClean();
    cityTableSetupClean();
    greatestTitleTableSetupClean();
    graphWidgetSetupClean();

    connect(mpTimer, SIGNAL(gameTimerUpdate(unsigned int, unsigned int, unsigned int)), this, SLOT(timerUpdate(unsigned int, unsigned int, unsigned int)));
    mpTimer->startTimer();
}

MainBoard::~MainBoard()
{
    delete ui;
}

void MainBoard::allocatePlayers(unsigned int playerCount)
{
    MageKnight_Player.resize(playerCount);
    PlayerRank.resize(playerCount);
    ui->tableWidget_players->setColumnCount(int(playerCount)+1); // +1 for the header column in the left side (column 0)

    // Find playCount different distinct colors using the hsv scale
    int hStep = 360/(playerCount+1); // Linearly distribute the h values from 0 to 360 degrees (h = 360 and h = 0 gives the same result)

    for (unsigned int i = 0; i < playerCount; ++i)
    {
        int h = hStep * int(i);       // Hue defines the color-space which is mapped across a circle
        int s = 90 + (rand() % 110);  // "saturation" represent the perceived saturation of the color (0 is all white, while 255 is max saturation)
        int v = 100 + (rand() % 100); // "value" represents the perceived brightness (0 is all black, while 255 is all dark)
        MageKnight_Player.at(i).mPlayerColor = QColor::fromHsv(h, s, v, 255);
    }
}

void MainBoard::addPlayer(unsigned int ID, QString playerName, QString playerCharacter)
{
    MageKnight_Player.at(ID-1).setName(playerName);
    MageKnight_Player.at(ID-1).setCharacter(playerCharacter);

    updatePlayerStats(); // Show the new player stats in the table

    ui->customPlot->addGraph();
    ui->customPlot->graph(int(ID)-1)->setPen(QPen(MageKnight_Player.at(ID-1).mPlayerColor));
    ui->customPlot->graph(int(ID)-1)->setName(MageKnight_Player.at(ID-1).mName);
}

void MainBoard::allocateCities(unsigned int cityCount)
{
    if (cityCount == 0)
    {
        ui->pushButton_cityDiscovered->setEnabled(false);
        ui->pushButton_cityDiscovered->setStyleSheet(mStyle_PushButton_disabled);
        ui->tableWidget_cities->hide();
    }

    else
    {
        MageKnight_Cities.resize(cityCount);
        ui->tableWidget_cities->setRowCount(int(cityCount) + 1);

        for (unsigned int i = 0; i < MageKnight_Player.size(); ++i)
        {
            MageKnight_Player.at(i).setNumberOfCities(cityCount);
        }

        updateCityList();
    }
}

void MainBoard::setDayCount(unsigned int dayCount)
{
    mDayCount = dayCount;
}

void MainBoard::updatePlayerStats(void)
{
    std::vector <QString> playerStats;
    playerStats.resize(NUMBER_OF_TABLE_ITEMS);
    QColor playerColor;

    unsigned int playerIndex = 0;

    updatePlayerRanks(); // This updates the rank for ordering the players w.r.t. their scores

    for (unsigned int i = 0; i < MageKnight_Player.size(); ++i)
    {    
        playerIndex = PlayerRank.at(i);

        playerStats.at(TABLE_ITEM_NAME_ID)         = MageKnight_Player.at(playerIndex).mName + " (" + QString::number(playerIndex+1) + ")";
        playerStats.at(TABLE_ITEM_CHARACTER)       = MageKnight_Player.at(playerIndex).mCharacter;
        playerStats.at(TABLE_ITEM_SCORE)           = QString::number(MageKnight_Player.at(playerIndex).mScore);
        playerStats.at(TABLE_ITEM_LEVEL)           = QString::number(MageKnight_Player.at(playerIndex).mLevel);
        playerStats.at(TABLE_ITEM_FAME)            = QString::number(MageKnight_Player.at(playerIndex).mFame);
        playerStats.at(TABLE_ITEM_REPUTATION)      = QString::number(MageKnight_Player.at(playerIndex).mReputation);
        playerStats.at(TABLE_ITEM_AAC)             = QString::number(MageKnight_Player.at(playerIndex).mAActionCards);
        playerStats.at(TABLE_ITEM_SPELLS)          = QString::number(MageKnight_Player.at(playerIndex).mSpellCards);
        playerStats.at(TABLE_ITEM_ARTIFACTS)       = QString::number(MageKnight_Player.at(playerIndex).mArtifacts);
        playerStats.at(TABLE_ITEM_CRYSTALS)        = QString::number(MageKnight_Player.at(playerIndex).mCrystals);
        playerStats.at(TABLE_ITEM_MONSTERS)        = QString::number(MageKnight_Player.at(playerIndex).mMonsters.size());
        playerStats.at(TABLE_ITEM_WOUNDS)          = QString::number(MageKnight_Player.at(playerIndex).mWounds);
        playerStats.at(TABLE_ITEM_KNOWLEDGE)       = QString::number(MageKnight_Player.at(playerIndex).mKnowledgePoints);
        playerStats.at(TABLE_ITEM_LOOT)            = QString::number(MageKnight_Player.at(playerIndex).mLootPoints);
        playerStats.at(TABLE_ITEM_LEADER)          = QString::number(MageKnight_Player.at(playerIndex).mLeaderPoints);
        playerStats.at(TABLE_ITEM_CONQUERER)       = QString::number(MageKnight_Player.at(playerIndex).mConquerorPoints);
        playerStats.at(TABLE_ITEM_CITY_CONQUERER)  = QString::number(MageKnight_Player.at(playerIndex).mCityPoints);
        playerStats.at(TABLE_ITEM_ADVENTURER)      = QString::number(MageKnight_Player.at(playerIndex).mAdventurerPoints);
        playerStats.at(TABLE_ITEM_BEATING)         = QString::number(MageKnight_Player.at(playerIndex).mBeatingPoints);

        // Display the values in the table using the corresponding playerColor
        playerColor = MageKnight_Player.at(playerIndex).mPlayerColor;
        int R = playerColor.red();
        int G = playerColor.green();
        int B = playerColor.blue();

        for (unsigned int j = 0; j < NUMBER_OF_TABLE_ITEMS; ++j)
        {
            ui->tableWidget_players->setItem(int(j), int(i)+1, new QTableWidgetItem(playerStats.at(j)));

            greatest_title_t titleState = GREATEST_TITLE_NONE;

            switch (j) {
            case (TABLE_ITEM_KNOWLEDGE):
                titleState = MageKnight_Player.at(playerIndex).mGreatestTitleStats.at(TABLE_ITEM_GREATEST_KNOWLEDGE);
                break;

            case (TABLE_ITEM_LOOT):
                titleState = MageKnight_Player.at(playerIndex).mGreatestTitleStats.at(TABLE_ITEM_GREATEST_LOOT);
                break;

            case (TABLE_ITEM_LEADER):
                titleState = MageKnight_Player.at(playerIndex).mGreatestTitleStats.at(TABLE_ITEM_GREATEST_LEADER);
                break;

            case (TABLE_ITEM_CONQUERER):
                titleState = MageKnight_Player.at(playerIndex).mGreatestTitleStats.at(TABLE_ITEM_GREATEST_CONQUEROR);
                break;

            case (TABLE_ITEM_CITY_CONQUERER):
                titleState = MageKnight_Player.at(playerIndex).mGreatestTitleStats.at(TABLE_ITEM_GREATEST_CITY_CONQUEROR);
                break;

            case (TABLE_ITEM_ADVENTURER):
                titleState = MageKnight_Player.at(playerIndex).mGreatestTitleStats.at(TABLE_ITEM_GREATEST_ADVENTURER);
                break;

            case (TABLE_ITEM_BEATING):
                titleState = MageKnight_Player.at(playerIndex).mGreatestTitleStats.at(TABLE_ITEM_GREATEST_BEATING);
                break;

            default:
                break;
            }

            if ((titleState == GREATEST_TITLE_WIN_TIED) || (titleState == GREATEST_TITLE_WIN))
            {
                // Show stat with highlighted (non-transparent) background, if leader position is recognized
                ui->tableWidget_players->item(int(j), int(i)+1)->setBackground(QBrush(QColor(R, G, B, 255)));
            }
            else
            {
                ui->tableWidget_players->item(int(j), int(i)+1)->setBackground(QBrush(QColor(R, G, B, 128)));
            }
        }
    }

    ui->tableWidget_players->resizeColumnsToContents();
}

void MainBoard::updateCityStats(Action action)
{
    if (action.mMainActionID == ACTION_ID_CITY)
    {
        unsigned int cityID = action.mCityID;
        unsigned int playerID = action.mPlayerID - 1;
        unsigned int monsterCount = action.mMonsters.size();

        for (unsigned int i = 0; i < action.mMonsters.size(); ++i)
        {
            if (action.mMonsters.at(i).mRampaging) --monsterCount;
        }

        MageKnight_Cities.at(cityID).mMonstersRemaining -= monsterCount; // Find the number of remaining monsters in the given city
        MageKnight_Player.at(playerID).mCityTokens.at(cityID) += monsterCount; // Assign the number of tokens in the city for the given player

        if (MageKnight_Cities.at(cityID).mMonstersRemaining == 0)
        {
            MageKnight_Cities.at(cityID).mConquered = true;
            findCityLeader(cityID);
        }

        updateCityList();
    }
}

void MainBoard::updatePlayerRanks(void)
{
    unsigned int maxValue = 0;
    unsigned int maxIndex = 0;

    // Create a list of the player scores
    std::vector<unsigned int> playerScores(PlayerRank.size());

    // Assign the player scores to the list
    for (unsigned int i = 0; i < PlayerRank.size(); ++i)
    {
        playerScores.at(i) = MageKnight_Player.at(i).mScore + 1; // Plus one is a small trick to ignore values set to 0 in the sorting algorithm
    }

    // Start sorting w.r.t. the score values in the list
    for (unsigned int i = 0; i < PlayerRank.size(); ++i)
    {
        maxIndex = 0;
        maxValue = playerScores.at(maxIndex);

        for (unsigned int j = 0; j < PlayerRank.size(); ++j)
        {
            if (playerScores.at(j) > maxValue)
            {
                maxIndex = j;
                maxValue = playerScores.at(maxIndex);
            }
        }

        playerScores.at(maxIndex) = 0; // Clear the value (set to zero should result in ignoring it from here)
        PlayerRank.at(i) = maxIndex;
    }
}

void MainBoard::updateCityList(void)
{
    QStringList cityStats;

    for (unsigned int i = 0; i < MageKnight_Cities.size(); ++i)
    {
        cityStats.clear();

        cityStats << MageKnight_Cities.at(i).mName
                  << MageKnight_Cities.at(i).mColor
                  << QString::number(MageKnight_Cities.at(i).mLevel)
                  << QString::number(MageKnight_Cities.at(i).mMonstersRemaining) + "(" + QString::number(MageKnight_Cities.at(i).mMonsters) + ")"
                  << MageKnight_Cities.at(i).mCityOwner;

        for (int j = 0; j < cityStats.length(); ++j)
        {
            ui->tableWidget_cities->setItem(int(i)+1, j, new QTableWidgetItem(cityStats.at(j)));
        }
    }

    ui->tableWidget_cities->resizeColumnsToContents();
}

void MainBoard::findGreatestTitles(void)
{
    for (unsigned int i = 0; i < NUMBER_OF_GREATEST_TITLES; ++i)
    {
        unsigned int maxScore = 0;
        unsigned int playerScore = 0;
        bool tied = false;
        bool firstNameWritten = false;

        // Search through all players and determine the maxScore and if it is tied between two or more players
        for (unsigned int j = 0; j < MageKnight_Player.size(); ++j)
        {
            playerScore = MageKnight_Player.at(j).mGreatestScoreValues.at(i);
            if (playerScore > maxScore)
            {
                maxScore = playerScore;
                tied = false;
            }
            else if (playerScore == maxScore)
            {
                tied = true;
            }
        }

        // Use the found maxscore to set the city relation status for all players
        for (unsigned int j = 0; j < MageKnight_Player.size(); ++j)
        {
            playerScore = MageKnight_Player.at(j).mGreatestScoreValues.at(i);

            if (playerScore < maxScore || maxScore == 0)
            {
                MageKnight_Player.at(j).setGreatestTitleState(greatest_title_items_t(i), GREATEST_TITLE_NONE);

                // There can be no leader if the found max score is 0.
                if (maxScore == 0)
                {
                    greatestTitlesPlayers.at(i) = "Volkare Biddybob";
                }
            }
            else if (playerScore == maxScore)
            {
                if (tied)
                {
                    MageKnight_Player.at(j).setGreatestTitleState(greatest_title_items_t(i), GREATEST_TITLE_WIN_TIED);

                    if (firstNameWritten)
                    {
                        greatestTitlesPlayers.at(i) += ", " + MageKnight_Player.at(j).mName;
                    }
                }

                else
                {
                    MageKnight_Player.at(j).setGreatestTitleState(greatest_title_items_t(i), GREATEST_TITLE_WIN);
                }

                if (firstNameWritten == false)
                {
                    greatestTitlesPlayers.at(i) = MageKnight_Player.at(j).mName;
                    firstNameWritten = true;
                }

            }

            MageKnight_Player.at(j).updateScore();
        }
    }
}

void MainBoard::updateGreatestTitleList(void)
{
    for (unsigned int i = 0; i < NUMBER_OF_GREATEST_TITLES; ++i)
    {
        ui->tableWidget_greatestTitles->setItem(int(i), 1, new QTableWidgetItem(greatestTitlesPlayers.at(i)));
    }

    ui->tableWidget_greatestTitles->resizeColumnsToContents();
}

void MainBoard::findCityLeader(unsigned int cityID)
{
    if (MageKnight_Cities.at(cityID).mConquered)
    {
        unsigned int maxScore = 0;
        unsigned int playerScore = 0;
        bool tied = false;
        std::vector <unsigned int> cityLeaderIDs;
        QString cityOwner = "";

        // Search through all players and determine the maxScore (number of tokens) and if it is tied between two or more players
        for (unsigned int i = 0; i< MageKnight_Player.size(); ++i)
        {
            playerScore = MageKnight_Player.at(i).mCityTokens.at(cityID);
            if (playerScore > maxScore)
            {
                maxScore = playerScore;
                tied = false;
            }
            else if (playerScore == maxScore)
            {
                tied = true;
            }
        }

        // Use the found maxscore to set the city relation status for all players
        for (unsigned int i = 0; i< MageKnight_Player.size(); ++i)
        {
            playerScore = MageKnight_Player.at(i).mCityTokens.at(cityID);

            if (playerScore == maxScore)
            {
                // Expand the list of city leader IDs by one
                unsigned int nLeaders = cityLeaderIDs.size();
                cityLeaderIDs.resize(nLeaders + 1);

                // Add the current playerID to the city leader IDs list
                cityLeaderIDs.at(nLeaders) = i;

                if (tied)
                {
                    MageKnight_Player.at(i).mCityRelations.at(cityID) = CITY_RELATION_IS_TIED_LEADER;
                }

                else
                {
                    MageKnight_Player.at(i).mCityRelations.at(cityID) = CITY_RELATION_IS_LEADER;
                }
            }

            else if (playerScore > 0)
            {
                MageKnight_Player.at(i).mCityRelations.at(cityID) = CITY_RELATION_HAS_TOKENS;
            }

            else
            {
                MageKnight_Player.at(i).mCityRelations.at(cityID) = CITY_RELATION_NONE;
            }

            MageKnight_Player.at(i).updateScore();
        }

        if (tied)
        {
            cityOwner = MageKnight_Player.at(cityLeaderIDs.at(0)).mName;

            for (unsigned int i = 1; i < cityLeaderIDs.size(); ++i)
            {
                cityOwner += ", ";
                cityOwner += MageKnight_Player.at(cityLeaderIDs.at(i)).mName;
            }
            cityOwner += " (tied)";
        }
        else
        {
            cityOwner = MageKnight_Player.at(cityLeaderIDs.at(0)).mName;
        }

        MageKnight_Cities.at(cityID).mCityOwner = cityOwner;
    }
}


void MainBoard::playerTableSetupClean(void)
{
    std::vector <QString> playerTableHeader;
    playerTableHeader.resize(NUMBER_OF_TABLE_ITEMS);

    playerTableHeader.at(TABLE_ITEM_NAME_ID)         = "Name (ID)";
    playerTableHeader.at(TABLE_ITEM_CHARACTER)       = "Character";
    playerTableHeader.at(TABLE_ITEM_SCORE)           = "Score";
    playerTableHeader.at(TABLE_ITEM_LEVEL)           = "Level";
    playerTableHeader.at(TABLE_ITEM_FAME)            = "Fame";
    playerTableHeader.at(TABLE_ITEM_REPUTATION)      = "Reputation";
    playerTableHeader.at(TABLE_ITEM_AAC)             = "Advanced Action Cards";
    playerTableHeader.at(TABLE_ITEM_SPELLS)          = "Spell Cards";
    playerTableHeader.at(TABLE_ITEM_ARTIFACTS)       = "Artifacts";
    playerTableHeader.at(TABLE_ITEM_CRYSTALS)        = "Crystals";
    playerTableHeader.at(TABLE_ITEM_MONSTERS)        = "Monster Kills";
    playerTableHeader.at(TABLE_ITEM_WOUNDS)          = "Wounds";
    playerTableHeader.at(TABLE_ITEM_KNOWLEDGE)       = "Knowledge Points";
    playerTableHeader.at(TABLE_ITEM_LOOT)            = "Loot Points";
    playerTableHeader.at(TABLE_ITEM_LEADER)          = "Leader Points";
    playerTableHeader.at(TABLE_ITEM_CONQUERER)       = "Conqueror Points";
    playerTableHeader.at(TABLE_ITEM_CITY_CONQUERER)  = "City Conqueror Points";
    playerTableHeader.at(TABLE_ITEM_ADVENTURER)      = "Adventurer Points";
    playerTableHeader.at(TABLE_ITEM_BEATING)         = "Beating Points";

    ui->tableWidget_players->setRowCount(NUMBER_OF_TABLE_ITEMS);
    ui->tableWidget_players->setColumnCount(1);

    for (unsigned int i=0; i<NUMBER_OF_TABLE_ITEMS; ++i)
    {
        ui->tableWidget_players->setItem(int(i), 0, new QTableWidgetItem(playerTableHeader.at(i)));
        ui->tableWidget_players->item(int(i),0)->setFont(QFont("MS Shell Dlg 2", 10, QFont::Bold));
    }

    ui->tableWidget_players->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget_players->setSelectionMode(QAbstractItemView::NoSelection);
    ui->tableWidget_players->setShowGrid(true);
    ui->tableWidget_players->setFocusPolicy(Qt::NoFocus); // Avoid selecting and highlighting of cells

    ui->tableWidget_players->resizeColumnsToContents();
}

void MainBoard::cityTableSetupClean(void)
{
    QStringList cityTableHeader;

    cityTableHeader
            <<"Name"
            <<"Color"
            <<"Level"
            <<"Monster"
            <<"City Owner";

    int columnCounter = cityTableHeader.length();;

    ui->tableWidget_cities->setColumnCount(columnCounter);
    ui->tableWidget_cities->setRowCount(1);

    for (int i=0; i<columnCounter; ++i)
    {
        ui->tableWidget_cities->setItem(0, i, new QTableWidgetItem(cityTableHeader.at(i)));
        ui->tableWidget_cities->item(0,i)->setFont(QFont("MS Shell Dlg 2", 10, QFont::Bold));
    }

    ui->tableWidget_cities->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget_cities->setSelectionMode(QAbstractItemView::NoSelection);
    ui->tableWidget_cities->setShowGrid(true);
    ui->tableWidget_cities->setFocusPolicy(Qt::NoFocus); // Avoid selecting and highlighting of cells
}

void MainBoard::greatestTitleTableSetupClean(void)
{
    ui->tableWidget_greatestTitles->setRowCount(NUMBER_OF_GREATEST_TITLES);
    ui->tableWidget_greatestTitles->setColumnCount(2);

    greatestTitlesPlayers.resize(NUMBER_OF_GREATEST_TITLES);
    greatestTitles.resize(NUMBER_OF_GREATEST_TITLES);

    // Specify the titles to be written in the table view
    greatestTitles.at(TABLE_ITEM_GREATEST_KNOWLEDGE)      = "The Greatest Knowledge";
    greatestTitles.at(TABLE_ITEM_GREATEST_LOOT)           = "The Greatest Loot";
    greatestTitles.at(TABLE_ITEM_GREATEST_LEADER)         = "The Greatest Leader";
    greatestTitles.at(TABLE_ITEM_GREATEST_CONQUEROR)      = "The Greatest Conqueror";
    greatestTitles.at(TABLE_ITEM_GREATEST_CITY_CONQUEROR) = "The Greatest City Conqueror";
    greatestTitles.at(TABLE_ITEM_GREATEST_ADVENTURER)     = "The Greatest Adventurer";
    greatestTitles.at(TABLE_ITEM_GREATEST_BEATING)        = "The Greatest Beating";

    // Write the specified title strings in the first column
    for (unsigned int i = 0; i < NUMBER_OF_GREATEST_TITLES; ++i)
    {
        greatestTitlesPlayers.at(i) = "Volkare Biddybob";
        ui->tableWidget_greatestTitles->setItem(int(i), 0, new QTableWidgetItem(greatestTitles.at(i)));
    }

    // Set the focus policy to deisabled, and ignore selections
    ui->tableWidget_greatestTitles->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget_greatestTitles->setSelectionMode(QAbstractItemView::NoSelection);
    ui->tableWidget_greatestTitles->setShowGrid(true);
    ui->tableWidget_greatestTitles->setFocusPolicy(Qt::NoFocus); // Avoid selecting and highlighting of cells

    updateGreatestTitleList();
}

void MainBoard::graphWidgetSetupClean(void)
{
    QColor plotAxisColor = QColor(190, 157, 56, 192);

    ui->customPlot->setBackground(QBrush(QColor(0, 0, 0, 192)));
    ui->customPlot->xAxis->setRange(0, mGraphTimerMaxRange);
    ui->customPlot->yAxis->setRange(-1, mGraphPointMaxRange);

    ui->customPlot->xAxis->setBasePen(QPen(plotAxisColor));
    ui->customPlot->xAxis->setTickLabelColor(plotAxisColor);
    ui->customPlot->xAxis->setSubTickPen(QPen(plotAxisColor));
    ui->customPlot->xAxis->grid()->setPen(QPen(QColor(0,0,0,255)));
    ui->customPlot->xAxis->grid()->setZeroLinePen(QPen(QColor(0,0,0,255)));

    ui->customPlot->yAxis->setBasePen(QPen(plotAxisColor));
    ui->customPlot->yAxis->setTickLabelColor(plotAxisColor);
    ui->customPlot->yAxis->setSubTickPen(QPen(plotAxisColor));
    ui->customPlot->yAxis->grid()->setPen(QPen(QColor(0,0,0,255)));
    ui->customPlot->yAxis->grid()->setZeroLinePen(QPen(QColor(0,0,0,255)));

    // configure bottom axis to show date instead of number:
    QSharedPointer<QCPAxisTickerDateTime> dateTicker(new QCPAxisTickerDateTime);
    dateTicker->setDateTimeFormat("hh:mm:ss");
    dateTicker->setDateTimeSpec(Qt::UTC);
    ui->customPlot->xAxis->setTicker(dateTicker);
}

void MainBoard::updateGraphPlots(void)
{
    unsigned int Nplayers = MageKnight_Player.size();

    for (unsigned int i = 0; i < Nplayers; ++i)
    {
        ui->customPlot->graph(int(i))->setData(MageKnight_Player.at(i).mTimeData, MageKnight_Player.at(i).mPointData);
    }

    ui->customPlot->replot();
}

void MainBoard::timerUpdate(unsigned int h, unsigned int m, unsigned int s)
{
    QString buttonText = "Play/Pause Game\n Game Time: ";

    if (h < 10) buttonText += "0";
    buttonText += QString::number(h) + ":";
    if (m < 10) buttonText += "0";
    buttonText += QString::number(m) + ":";
    if (s < 10) buttonText += "0";
    buttonText += QString::number(s);

    ui->pushButton_playPauseGame->setText(buttonText);



    // Update all player time time data to update the graph plots
    unsigned long nTicks = s + 60*m + 3600*h;

    if (nTicks > mGraphTimerMaxRange)
    {
        mGraphTimerMaxRange = mGraphTimerMaxRange * 2;
        ui->customPlot->xAxis->setRange(0, mGraphTimerMaxRange);
    }

    unsigned int Nplayers = MageKnight_Player.size();

    for (unsigned int i = 0; i < Nplayers; ++i)
    {
        int endPoint = MageKnight_Player.at(i).mTimeData.size();
        MageKnight_Player.at(i).mTimeData[endPoint-1] = nTicks;
    }

    updateGraphPlots();
}

void MainBoard::on_pushButton_endGame_clicked()
{
    AreYourSureDialog sureDialog(this);

    if(sureDialog.exec() == QDialog::Accepted)
    {
        this->close();
    }
}

void MainBoard::on_pushButton_enterUserAction_clicked()
{
    UserActionDialog actionDialog(this);

    unsigned int playerCount = MageKnight_Player.size();
    unsigned int cityCount = MageKnight_Cities.size();

    actionDialog.allocatePlayers(playerCount);
    actionDialog.allocateCities(cityCount);

    for (unsigned int i = 0; i < playerCount; ++i)
    {
        actionDialog.addPlayer(i+1, MageKnight_Player.at(i));
    }

    for (unsigned int i = 0; i < cityCount; ++i)
    {
        actionDialog.addCity(i+1, MageKnight_Cities.at(i));
    }

    if (actionDialog.exec() == QDialog::Accepted)
    {
        Action PlayerAction = actionDialog.mplayerAction;  // Receive a copy of the action from the dialog
        PlayerAction.mTime = mpTimer->mTotalTimeInSeconds; // Add a time-stamp to the action

        MageKnight_Player.at(PlayerAction.mPlayerID-1).addAction(PlayerAction); // Add the action to the given player
        updateCityStats(PlayerAction);
        findGreatestTitles();

        // Loop through all players and check if their scores have changed
        for (unsigned int i = 0; i < MageKnight_Player.size(); ++i)
        {
            int playerScore = MageKnight_Player.at(i).mScore;
            int NdataPoints = MageKnight_Player.at(i).mPointData.size();

            if (playerScore != int(MageKnight_Player.at(i).mPointData[NdataPoints-1]))
            {
                MageKnight_Player.at(i).mPointData.resize(NdataPoints+2);
                MageKnight_Player.at(i).mTimeData.resize(NdataPoints+2);

                MageKnight_Player.at(i).mPointData[NdataPoints] = playerScore;
                MageKnight_Player.at(i).mPointData[NdataPoints+1] = playerScore;

                MageKnight_Player.at(i).mTimeData[NdataPoints] = mpTimer->mTotalTimeInSeconds;
                MageKnight_Player.at(i).mTimeData[NdataPoints+1] = mpTimer->mTotalTimeInSeconds;
            }

            if (playerScore > int(mGraphPointMaxRange))
            {
                // If the current player score exeeds the current range on the graph, then reset the range.
                // The new max is found as the next nearest 25 (e.g. 215 --> newYMax = 225)
                mGraphPointMaxRange = 25 * unsigned(ceil(playerScore/25.0));
                ui->customPlot->yAxis->setRange(-1,mGraphPointMaxRange);
            }
        }

        updateGreatestTitleList();
        updatePlayerStats(); // Update the player list view on the MainBoard screen
    }
}

void MainBoard::on_pushButton_playPauseGame_clicked()
{
    if (mpTimer->isTimerActive())
    {
        // Stop the timer and disable the "Enter User Action" button.
        mpTimer->stopTimer();
        ui->pushButton_enterUserAction->setEnabled(false);
        ui->pushButton_enterUserAction->setStyleSheet(mStyle_PushButton_disabled);
    }

    else
    {
        // Restart the timer and enable the "Enter User Action" button.
        // The timer is restarted from 0 ms. Thus, up to 999 ms time tracking can be lost.
        mpTimer->startTimer();
        ui->pushButton_enterUserAction->setEnabled(true);
        ui->pushButton_enterUserAction->setStyleSheet(mStyle_PushButton_normal);
    }
}

void MainBoard::on_pushButton_cityDiscovered_clicked()
{
    AddCityDialog cityDialog(this);

    if (cityDialog.exec() == QDialog::Accepted)
    {       
        City city = cityDialog.mCity;   // Receive a copy of the city from the dialog

        unsigned int cityCount = MageKnight_Cities.size();

        // Find next "Unknown" city in the list
        for (unsigned int i = 0; i < cityCount; ++i)
        {
            if (MageKnight_Cities.at(i).mDiscovered == false)
            {
                MageKnight_Cities.at(i) = city; // Add the city to the list of discovered cities

                // Disable the "Discovered City" button, when the last city is discovered.
                if (i+1 == cityCount)
                {
                    ui->pushButton_cityDiscovered->setEnabled(false);
                    ui->pushButton_cityDiscovered->setStyleSheet(mStyle_PushButton_disabled);
                }
                break;
            }
        }

        updateCityList();
    }
}

void MainBoard::on_pushButton_enterUserAction_pressed()
{
    ui->pushButton_enterUserAction->setStyleSheet(mStyle_PushButton_pressed);
}

void MainBoard::on_pushButton_enterUserAction_released()
{
    ui->pushButton_enterUserAction->setStyleSheet(mStyle_PushButton_normal);
}

void MainBoard::on_pushButton_cityDiscovered_pressed()
{
    ui->pushButton_cityDiscovered->setStyleSheet(mStyle_PushButton_pressed);
}

void MainBoard::on_pushButton_cityDiscovered_released()
{
    ui->pushButton_cityDiscovered->setStyleSheet(mStyle_PushButton_normal);
}

void MainBoard::on_pushButton_playPauseGame_pressed()
{
    ui->pushButton_playPauseGame->setStyleSheet(mStyle_PushButton_pressed);
}

void MainBoard::on_pushButton_playPauseGame_released()
{
    ui->pushButton_playPauseGame->setStyleSheet(mStyle_PushButton_normal);
}

void MainBoard::on_pushButton_endGame_pressed()
{
    ui->pushButton_endGame->setStyleSheet(mStyle_PushButton_pressed);
}

void MainBoard::on_pushButton_endGame_released()
{
    ui->pushButton_endGame->setStyleSheet(mStyle_PushButton_normal);
}
