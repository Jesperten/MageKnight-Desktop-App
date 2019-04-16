#include <vector>
#include <QVector>
#include "ui_Window_MainBoard.h"
#include "Window_MainBoard.h"
#include "Player.h"
#include "Dialog_EndGame.h"
#include "Dialog_UserAction.h"
#include "Dialog_AddCity.h"
#include "City.h"

static std::vector<Player> pMageKnightPlayers;

Window_MainBoard::Window_MainBoard(QWidget *parent) : QMainWindow(parent), ui(new Ui::Window_MainBoard) {
    ui->setupUi(this);

    playerTableSetupClean();
    cityTableSetupClean();
    greatestTitleTableSetupClean();
    graphWidgetSetupClean();
}

Window_MainBoard::~Window_MainBoard() {
    delete ui;
}

void Window_MainBoard::setPlayerGraphics(const std::vector<Player>& playerList) {
    int playerCount = playerList.size();

    ui->tableWidget_players->setColumnCount(playerCount+1); // +1 for the header column in the left side (column 0)

    int graphCount = ui->customPlot->graphCount();

    // With the given number of players, check that the number of graphs matches
    if (graphCount < playerCount) {
        // Add (playerCount - graphCount) graphs
        for(int i = 0; i < (playerCount - graphCount); i++) {
            ui->customPlot->addGraph();
        }
    }
    else if (graphCount > playerCount) {
        //Remove the (graphCount - playerCount) last graphs
        for(int i = 0; i < (graphCount - playerCount); i++) {
            ui->customPlot->removeGraph(graphCount-i-1);
        }
    }

    for(int i = 0; i < playerCount; i++) {
        // Redefine graph colors and legends to match the player list
        ui->customPlot->graph(i)->setPen(QPen(playerList.at(i).mPlayerColor));
        ui->customPlot->graph(i)->setName(playerList.at(i).mName);
    }
}

void Window_MainBoard::allocateCities(unsigned int cityCount)
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

void Window_MainBoard::setDayCount(unsigned int dayCount)
{
    mDayCount = dayCount;
}

void Window_MainBoard::updatePlayerStats(const std::vector<Player>& playerList) {
    std::vector <QString> playerStats;
    QColor playerColor;

    playerStats.resize(NUMBER_OF_TABLE_ITEMS);

    for (unsigned int i = 0; i < playerList.size(); ++i) {
        playerStats.at(TABLE_ITEM_NAME_ID)         = playerList.at(i).mName + " (" + QString::number(i+1) + ")";
        playerStats.at(TABLE_ITEM_CHARACTER)       = playerList.at(i).mCharacter;
        playerStats.at(TABLE_ITEM_SCORE)           = QString::number(playerList.at(i).mScore);
        playerStats.at(TABLE_ITEM_LEVEL)           = QString::number(playerList.at(i).mLevel);
        playerStats.at(TABLE_ITEM_FAME)            = QString::number(playerList.at(i).mFame);
        playerStats.at(TABLE_ITEM_REPUTATION)      = QString::number(playerList.at(i).mReputation);
        playerStats.at(TABLE_ITEM_AAC)             = QString::number(playerList.at(i).mAActionCards);
        playerStats.at(TABLE_ITEM_SPELLS)          = QString::number(playerList.at(i).mSpellCards);
        playerStats.at(TABLE_ITEM_ARTIFACTS)       = QString::number(playerList.at(i).mArtifacts);
        playerStats.at(TABLE_ITEM_CRYSTALS)        = QString::number(playerList.at(i).mCrystals);
        playerStats.at(TABLE_ITEM_MONSTERS)        = QString::number(playerList.at(i).mMonsters.size());
        playerStats.at(TABLE_ITEM_WOUNDS)          = QString::number(playerList.at(i).mWounds);
        playerStats.at(TABLE_ITEM_KNOWLEDGE)       = QString::number(playerList.at(i).mBasicScoreValues.at(TITLE_GREATEST_KNOWLEDGE));
        playerStats.at(TABLE_ITEM_LOOT)            = QString::number(playerList.at(i).mBasicScoreValues.at(TITLE_GREATEST_LOOT));
        playerStats.at(TABLE_ITEM_LEADER)          = QString::number(playerList.at(i).mBasicScoreValues.at(TITLE_GREATEST_LEADER));
        playerStats.at(TABLE_ITEM_CONQUERER)       = QString::number(playerList.at(i).mBasicScoreValues.at(TITLE_GREATEST_CONQUEROR));
        playerStats.at(TABLE_ITEM_CITY_CONQUERER)  = QString::number(playerList.at(i).mBasicScoreValues.at(TITLE_GREATEST_CITY_CONQUEROR));
        playerStats.at(TABLE_ITEM_ADVENTURER)      = QString::number(playerList.at(i).mBasicScoreValues.at(TITLE_GREATEST_ADVENTURER));
        playerStats.at(TABLE_ITEM_BEATING)         = QString::number(playerList.at(i).mBasicScoreValues.at(TITLE_GREATEST_BEATING));

        // Display the values in the table using the corresponding playerColor
        playerColor = playerList.at(i).mPlayerColor;

        for (unsigned int j = 0; j < NUMBER_OF_TABLE_ITEMS; ++j) {
            ui->tableWidget_players->setItem(int(j), playerList.at(i).mRank, new QTableWidgetItem(playerStats.at(j)));

            // This remaining part is used to highlight achieved Greatest titles in the list
            greatest_title_t titleState = GREATEST_TITLE_NONE;

            switch (j) {
            case (TABLE_ITEM_KNOWLEDGE):
                titleState = playerList.at(i).mGreatestTitleStats.at(TITLE_GREATEST_KNOWLEDGE);
                break;

            case (TABLE_ITEM_LOOT):
                titleState = playerList.at(i).mGreatestTitleStats.at(TITLE_GREATEST_LOOT);
                break;

            case (TABLE_ITEM_LEADER):
                titleState = playerList.at(i).mGreatestTitleStats.at(TITLE_GREATEST_LEADER);
                break;

            case (TABLE_ITEM_CONQUERER):
                titleState = playerList.at(i).mGreatestTitleStats.at(TITLE_GREATEST_CONQUEROR);
                break;

            case (TABLE_ITEM_CITY_CONQUERER):
                titleState = playerList.at(i).mGreatestTitleStats.at(TITLE_GREATEST_CITY_CONQUEROR);
                break;

            case (TABLE_ITEM_ADVENTURER):
                titleState = playerList.at(i).mGreatestTitleStats.at(TITLE_GREATEST_ADVENTURER);
                break;

            case (TABLE_ITEM_BEATING):
                titleState = playerList.at(i).mGreatestTitleStats.at(TITLE_GREATEST_BEATING);
                break;

            default:
                break;
            }

            if ((titleState == GREATEST_TITLE_WIN_TIED) || (titleState == GREATEST_TITLE_WIN))
                playerColor.setAlpha(230); // Set the intensity high to highlight a won or tied greatest title
            else
                playerColor.setAlpha(112); // Set the intensity low as default

            ui->tableWidget_players->item(int(j), playerList.at(i).mRank)->setBackground(QBrush(playerColor));
        }
    }

    ui->tableWidget_players->resizeColumnsToContents();
}

/*void Window_MainBoard::updateCityStats(Action action)
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
}*/

void Window_MainBoard::updateCityList(void)
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


/*void Window_MainBoard::updateGreatestTitleList(void)
{
    for (unsigned int i = 0; i < NUMBER_OF_GREATEST_TITLES; ++i)
    {
        ui->tableWidget_greatestTitles->setItem(int(i), 1, new QTableWidgetItem(greatestTitlesPlayers.at(i)));
    }

    ui->tableWidget_greatestTitles->resizeColumnsToContents();
}*/

/*void Window_MainBoard::findCityLeader(unsigned int cityID)
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
}*/

void Window_MainBoard::playerTableSetupClean(void) {
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

    for (unsigned int i=0; i<NUMBER_OF_TABLE_ITEMS; ++i) {
        ui->tableWidget_players->setItem(int(i), 0, new QTableWidgetItem(playerTableHeader.at(i)));
        ui->tableWidget_players->item(int(i),0)->setFont(QFont("MS Shell Dlg 2", 10, QFont::Bold));
    }

    ui->tableWidget_players->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget_players->setSelectionMode(QAbstractItemView::NoSelection);
    ui->tableWidget_players->setShowGrid(true);
    ui->tableWidget_players->setFocusPolicy(Qt::NoFocus); // Avoid selecting and highlighting of cells

    ui->tableWidget_players->resizeColumnsToContents();
}

void Window_MainBoard::cityTableSetupClean(void) {
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

void Window_MainBoard::greatestTitleTableSetupClean(void)
{
    ui->tableWidget_greatestTitles->setRowCount(NUMBER_OF_GREATEST_TITLES);
    ui->tableWidget_greatestTitles->setColumnCount(2);

    //greatestTitlesPlayers.resize(NUMBER_OF_GREATEST_TITLES);
    greatestTitles.resize(NUMBER_OF_GREATEST_TITLES);

    // Specify the titles to be written in the table view
    greatestTitles.at(TITLE_GREATEST_KNOWLEDGE)      = "The Greatest Knowledge";
    greatestTitles.at(TITLE_GREATEST_LOOT)           = "The Greatest Loot";
    greatestTitles.at(TITLE_GREATEST_LEADER)         = "The Greatest Leader";
    greatestTitles.at(TITLE_GREATEST_CONQUEROR)      = "The Greatest Conqueror";
    greatestTitles.at(TITLE_GREATEST_CITY_CONQUEROR) = "The Greatest City Conqueror";
    greatestTitles.at(TITLE_GREATEST_ADVENTURER)     = "The Greatest Adventurer";
    greatestTitles.at(TITLE_GREATEST_BEATING)        = "The Greatest Beating";

    // Write the specified title strings in the first column
    for (unsigned int i = 0; i < NUMBER_OF_GREATEST_TITLES; ++i)
    {
        //greatestTitlesPlayers.at(i) = "Volkare Biddybob";
        ui->tableWidget_greatestTitles->setItem(int(i), 0, new QTableWidgetItem(greatestTitles.at(i)));
    }

    // Set the focus policy to deisabled, and ignore selections
    ui->tableWidget_greatestTitles->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget_greatestTitles->setSelectionMode(QAbstractItemView::NoSelection);
    ui->tableWidget_greatestTitles->setShowGrid(true);
    ui->tableWidget_greatestTitles->setFocusPolicy(Qt::NoFocus); // Avoid selecting and highlighting of cells

    //updateGreatestTitleList();
}

void Window_MainBoard::graphWidgetSetupClean(void) {
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

void Window_MainBoard::on_newPlayerData(const std::vector<Player>& playerList, const GameTimer& gameTimer) {
    QString buttonText = "Play/Pause Game\n Game Time: ";

    int Nplayers = playerList.size();

    unsigned int hours   = gameTimer.mTicks/3600;
    unsigned int minutes = (gameTimer.mTicks - hours*3600)/60;
    unsigned int seconds = (gameTimer.mTicks - hours*3600 - minutes*60);

    if (hours < 10) buttonText += "0";
    buttonText += QString::number(hours) + ":";
    if (minutes < 10) buttonText += "0";
    buttonText += QString::number(minutes) + ":";
    if (seconds < 10) buttonText += "0";
    buttonText += QString::number(seconds);

    ui->pushButton_playPauseGame->setText(buttonText);

    // Check if the time-axis of the graph needs to be rescaled
    if (gameTimer.mTicks > mGraphTimerMaxRange) {
        mGraphTimerMaxRange = mGraphTimerMaxRange * 1.5; // Extend the time range with 50%
        ui->customPlot->xAxis->setRange(0, mGraphTimerMaxRange);
    }

    // Safety mechanism to verify that no users have been added or removed
    // without resetting the graphcount and playerTable column count.
    if ((Nplayers != ui->customPlot->graphCount()) || ((Nplayers+1) != ui->tableWidget_players->columnCount())) {
        setPlayerGraphics(playerList);
    }

    // Update the playerTable data display
    updatePlayerStats(playerList);

    // Plot the player data on the customplot graphs.
    for (int i = 0; i < Nplayers; ++i) {
        ui->customPlot->graph(int(i))->setData(playerList.at(i).mTimeData, playerList.at(i).mPointData);
    }

    ui->customPlot->replot();
}

void Window_MainBoard::on_newPlayerAndCityData(const std::vector<Player>& playerList, const std::vector<City>& cityList) {
    // Forward the event to the Action Dialog
    emit newPlayerAndCityGameEngineData(playerList, cityList);
}

void Window_MainBoard::on_newTestUserAction(Action testAction) {
    // Forward the event from Action Dialog to the GameEngine
    emit newTestUserAction(testAction);
}

void Window_MainBoard::on_newTempPlayerData(const Player& player, const Player& playerTemp) {
    // Forward the event from the Game Engine to the Action Dialog
    emit newTempPlayerData(player, playerTemp);
}

void Window_MainBoard::on_pushButton_endGame_clicked()
{
    Dialog_EndGame sureDialog(this);

    if(sureDialog.exec() == QDialog::Accepted)
    {
        QCoreApplication::exit(0);
    }
}

void Window_MainBoard::on_pushButton_enterUserAction_clicked() {
    Dialog_UserAction actionDialog(this);
    //actionDialog.show();

    connect(this,          SIGNAL(newPlayerAndCityGameEngineData(const std::vector<Player>&, const std::vector<City>&)), &actionDialog, SLOT(on_newPlayerAndCityGameEngineData(const std::vector<Player>&, const std::vector<City>&)));
    connect(this,          SIGNAL(newTempPlayerData(const Player&, const Player&)),                                      &actionDialog, SLOT(on_newTempPlayerData(const Player&, const Player&)));
    connect(&actionDialog, SIGNAL(newTestUserAction(Action)),                                                            this,          SLOT(on_newTestUserAction(Action)));

    emit userActionDialogOpened(); // Request for player and city data from gameEngine

    if (actionDialog.exec() == QDialog::Accepted)
    {
        emit newUserAction(actionDialog.mplayerAction);
    }
}

void Window_MainBoard::on_pushButton_playPauseGame_clicked()
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

void Window_MainBoard::on_pushButton_cityDiscovered_clicked()
{
    Dialog_AddCity cityDialog(this);

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

void Window_MainBoard::on_pushButton_enterUserAction_pressed()
{
    ui->pushButton_enterUserAction->setStyleSheet(mStyle_PushButton_pressed);
}

void Window_MainBoard::on_pushButton_enterUserAction_released()
{
    ui->pushButton_enterUserAction->setStyleSheet(mStyle_PushButton_normal);
}

void Window_MainBoard::on_pushButton_cityDiscovered_pressed()
{
    ui->pushButton_cityDiscovered->setStyleSheet(mStyle_PushButton_pressed);
}

void Window_MainBoard::on_pushButton_cityDiscovered_released()
{
    ui->pushButton_cityDiscovered->setStyleSheet(mStyle_PushButton_normal);
}

void Window_MainBoard::on_pushButton_playPauseGame_pressed()
{
    ui->pushButton_playPauseGame->setStyleSheet(mStyle_PushButton_pressed);
}

void Window_MainBoard::on_pushButton_playPauseGame_released()
{
    ui->pushButton_playPauseGame->setStyleSheet(mStyle_PushButton_normal);
}

void Window_MainBoard::on_pushButton_endGame_pressed()
{
    ui->pushButton_endGame->setStyleSheet(mStyle_PushButton_pressed);
}

void Window_MainBoard::on_pushButton_endGame_released()
{
    ui->pushButton_endGame->setStyleSheet(mStyle_PushButton_normal);
}
