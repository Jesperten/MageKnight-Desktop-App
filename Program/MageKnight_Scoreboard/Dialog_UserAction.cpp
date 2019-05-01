#include <QFont>
#include <QtWidgets>
#include "Action.h"
#include "ui_Dialog_UserAction.h"
#include "Dialog_UserAction.h"
#include "GameEngine.h"

/** ****************************************************** PUBLIC METHODS ********************************************************/

Dialog_UserAction::Dialog_UserAction(QWidget *parent) : QDialog(parent), ui(new Ui::Dialog_UserAction) {
    ui->setupUi(this);

    // Const pointer to the Singleton instance of the GameEngine class (global object)
    const GameEngine* gameEngine = GameEngine::instance();

    // Reseed the random generator with the current time as parameter.
    // This is to ensure a more true random nature
    srand(unsigned(time(nullptr)));

    mSoundEffects[0].setSource(QUrl("qrc:/sounds/Sound1"));
    mSoundEffects[1].setSource(QUrl("qrc:/sounds/Sound2"));
    mSoundEffects[2].setSource(QUrl("qrc:/sounds/Sound3"));
    mSoundEffects[3].setSource(QUrl("qrc:/sounds/Sound4"));
    mSoundEffects[4].setSource(QUrl("qrc:/sounds/Sound5"));
    mSoundEffects[5].setSource(QUrl("qrc:/sounds/Sound6"));
    mSoundEffects[6].setSource(QUrl("qrc:/sounds/Sound7"));
    mSoundEffects[7].setSource(QUrl("qrc:/sounds/Sound8"));

    connect(gameEngine, &GameEngine::newMageKnightData, this, &Dialog_UserAction::on_newPlayerAndCityData );

    connect(gameEngine, &GameEngine::userActionDialogData, this      , &Dialog_UserAction::on_dialogSetupDataReceived );
    connect(gameEngine, &GameEngine::newTempPlayerData   , this      , &Dialog_UserAction::on_newTempPlayerData       );
    connect(this      , &Dialog_UserAction::newUserAction, gameEngine, &GameEngine::on_newUserAction                  );
}

Dialog_UserAction::~Dialog_UserAction() {
    delete ui;
}

/** ****************************************************** PRIVATE METHODS *******************************************************/

void Dialog_UserAction::ui_userActionSetupClean(void) {
    std::vector<QString> tableStatsItems(NUMBER_OF_SCORE_STATS);
    QStringList tableMonstersItems;

    mplayerAction.reset();
    ui->comboBox_playerList->clear();
    ui->comboBox_actionList->clear();

    // Setup the Player and Action combo boxes and add the action items
    // The player are added to the list using the addPlayer public method
    ui->comboBox_playerList->addItem("Select Player");
    for (unsigned int i = 0; i < mPlayerListCopy.size(); i++) {
        ui->comboBox_playerList->addItem(mPlayerListCopy.at(i).mName + " (" + QString::number(mPlayerListCopy.at(i).mId) + ")");
    }


    for (unsigned int i = 0; i < NUMBER_OF_ACTION_IDS; ++i) {
        ui->comboBox_actionList->addItem(mActionList.at(i));
    }


    // Setup the Action Stats table without actual headers -------------------------------------------------
    ui->tableWidget_actionStats->verticalHeader()->setVisible(false);
    ui->tableWidget_actionStats->horizontalHeader()->setVisible(false);
    ui->tableWidget_actionStats->setRowCount(2);
    ui->tableWidget_actionStats->setColumnCount(NUMBER_OF_SCORE_STATS);
    ui->tableWidget_actionStats->setItem(1, 0, new QTableWidgetItem("Value"));

    // Define the items to be written in the Action Stats table and write them into the first row
    tableStatsItems.at(SCORE_STAT_ID_STAT)     = "Stat";
    tableStatsItems.at(SCORE_STAT_ID_SCORE)    = "Score";
    tableStatsItems.at(SCORE_STAT_ID_FAME)     = "Fame";
    tableStatsItems.at(SCORE_STAT_ID_REP)      = "Reputation\nStep";
    tableStatsItems.at(SCORE_STAT_ID_LEVEL)    = "Level";
    tableStatsItems.at(SCORE_STAT_ID_AAC)      = "AAC";
    tableStatsItems.at(SCORE_STAT_ID_SPELL)    = "Spell";
    tableStatsItems.at(SCORE_STAT_ID_ARTIFACT) = "Artifact";
    tableStatsItems.at(SCORE_STAT_ID_CRYSTALS) = "Crystals";

    for (unsigned int i = 0; i < NUMBER_OF_SCORE_STATS; ++i) {
        ui->tableWidget_actionStats->setItem(0, int(i), new QTableWidgetItem(tableStatsItems.at(i)));
        ui->tableWidget_actionStats->setColumnWidth(int(i), 97);
    }

    ui->tableWidget_actionStats->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget_actionStats->setSelectionMode(QAbstractItemView::NoSelection);
    ui->tableWidget_actionStats->setFocusPolicy(Qt::NoFocus); // Avoid selecting and highlighting of cells


    // Setup the Monster table without actual headers ------------------------------------------------------
    ui->tableWidget_monsters->verticalHeader()->setVisible(false);
    ui->tableWidget_monsters->horizontalHeader()->setVisible(false);
    ui->tableWidget_monsters->setRowCount(1);
    ui->tableWidget_monsters->setColumnCount(4);

    tableMonstersItems << "Type" << "Fame" << "Special" << "Rampaging";
    for (int i = 0; i < tableMonstersItems.length(); ++i) {
        ui->tableWidget_monsters->setItem(0, i, new QTableWidgetItem(tableMonstersItems.at(i)));
    }

    ui->tableWidget_monsters->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget_monsters->setSelectionMode(QAbstractItemView::NoSelection);
    ui->tableWidget_monsters->setFocusPolicy(Qt::NoFocus); // Avoid selecting and highlighting of cells

    addMonster();
    ui->tableWidget_monsters->resizeColumnsToContents();
    removeMonster();


    // Setup the Units table without actual headers --------------------------------------------------------
    ui->tableWidget_units->verticalHeader()->setVisible(false);
    ui->tableWidget_units->horizontalHeader()->setVisible(false);
    ui->tableWidget_units->setRowCount(1);
    ui->tableWidget_units->setColumnCount(2);
    ui->tableWidget_units->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget_units->setSelectionMode(QAbstractItemView::NoSelection);
    ui->tableWidget_units->setFocusPolicy(Qt::NoFocus); // Avoid selecting and highlighting of cells

    ui->tableWidget_units->setItem(0,0, new QTableWidgetItem("Unit"));
    ui->tableWidget_units->setItem(0,1, new QTableWidgetItem("Wounded"));

    Unit unit;
    addUnit(unit);
    ui->tableWidget_units->resizeColumnsToContents();
    removeUnit();

    on_comboBox_playerList_currentIndexChanged(0);
}

void Dialog_UserAction::acceptButtonControlCheck(void)
{
    bool finalStatement = true;

    int comboPlayerListIndex = ui->comboBox_playerList->currentIndex();
    int comboActionListIndex = ui->comboBox_actionList->currentIndex();
    int comboSuccessListIndex = ui->comboBox_successFail->currentIndex();
    bool comboSuccessListState = ui->comboBox_successFail->isEnabled();
    int comboOptionalListIndex = ui->comboBox_optional->currentIndex();
    int reportedMonsters = ui->tableWidget_monsters->rowCount() - 1;
    int reportedNonRampagingMonsters = reportedMonsters;

    for (int i = 1; i <= reportedMonsters; ++i)
    {
        QCheckBox *isRampagingCB = qobject_cast<QCheckBox*>(ui->tableWidget_monsters->cellWidget(i,3));
        if (isRampagingCB->isChecked())
        {
            --reportedNonRampagingMonsters;
        }
    }

    QString comboSuccessListText = ui->comboBox_successFail->currentText();

    // Disable the accept button if any of the three essential comboboxes have not been set
    if (comboPlayerListIndex == 0)    finalStatement = false;
    if (comboActionListIndex == 0)    finalStatement = false;
    if ((comboSuccessListIndex == 0) && (comboSuccessListState))   finalStatement = false;

    // If a user reports a successful attack on a rampaging monster, at least one monster must be reported
    if ((comboActionListIndex == ACTION_ID_RAMPAGING_MONSTER) && (comboSuccessListText == "Success"))
    {
        if (reportedMonsters < 1) finalStatement = false;
    }

    // Reporting an assault on a city requires that the city is specified and this city is discovered and unconqored
    if (comboActionListIndex == ACTION_ID_CITY)
    {
        // Check if a city is specified at all
        if (comboOptionalListIndex > 0)
        {
            // Check the number of remaining monsters in the specified city
            int monstersRemaining = int(mCityListCopy.at(unsigned(comboOptionalListIndex-1)).mMonstersRemaining);
            if (monstersRemaining == 0) finalStatement = false;
            if (monstersRemaining < reportedNonRampagingMonsters) finalStatement = false;
        }
        else
        {
            finalStatement = false;
        }
    }

    if ((comboActionListIndex == ACTION_ID_DUNGEON) ||
        (comboActionListIndex == ACTION_ID_LABYRINTH) ||
        (comboActionListIndex == ACTION_ID_MAZE) ||
        (comboActionListIndex == ACTION_ID_MONSTER_DEN) ||
        (comboActionListIndex == ACTION_ID_RUINS) ||
        (comboActionListIndex == ACTION_ID_SPAWNING_GROUNDS))
    {
        // Check if no reward is selected at all
        if (comboOptionalListIndex == 0) finalStatement = false;
    }

    // Check each reported monster that the type and special is specified
    for (unsigned int i = 0; i<mplayerAction.mMonsters.size(); ++i)
    {
        if(mplayerAction.mMonsters.at(i).mType == "Select Type")       finalStatement = false;
        if(mplayerAction.mMonsters.at(i).mSpecial == "Select Special") finalStatement = false;
    }

    for (unsigned int i = 0; i<mplayerAction.mUnits.size(); ++i)
    {
        if(mplayerAction.mUnits.at(i).mName == "Select Unit (0)") finalStatement = false;
    }

    // Finally set the state of the accept button with respect to the finalStatement variable
    ui->pushButton_enterUserAction->setEnabled(finalStatement);
}

void Dialog_UserAction::setOptionalComboBox(int index) {
    ui->comboBox_optional->clear();

    switch (index) {
    case ACTION_ID_TEXT: // No Action
    case ACTION_ID_RAMPAGING_MONSTER:
    case ACTION_ID_INTERACTION:
    case ACTION_ID_PLUNDERED_VILLAGE:
    case ACTION_ID_MAGE_TOWER:
    case ACTION_ID_KEEP:
    case ACTION_ID_BURNED_MONASTERY:
    case ACTION_ID_TOMB:
        // Default setting when no optional parameters are available
        ui->comboBox_optional->addItem("Optional parameter");
        break;

    case ACTION_ID_CITY: // City
        // Set the optional combobox to include all revealed cities (revealed and listed in the Window_MainBoard scope

        ui->comboBox_optional->addItem("Select City");

        // Change to list of revealed cities
        for (unsigned int i = 0; i<mCityListCopy.size(); ++i) {
            if (mCityListCopy.at(i).mConquered == true)
                ui->comboBox_optional->addItem(mCityListCopy.at(i).mName + " (" + mCityListCopy.at(i).mColor + ")" + " [Conquered]");
            else
                ui->comboBox_optional->addItem(mCityListCopy.at(i).mName + " (" + mCityListCopy.at(i).mColor + ")" + " ["+ QString::number(mCityListCopy.at(i).mMonstersRemaining) +"]");
        }
        break;

    case ACTION_ID_DUNGEON:
        // Set the optional combobox to include all possible outcomes of clearing a dungeon.
        for (unsigned int i = 0; i<NUMBER_OF_OPTIONAL_DUNGEON_IDS; ++i) {
            ui->comboBox_optional->addItem(mOptionalListDungeon.at(i));
        }
        break;

    case ACTION_ID_LABYRINTH:
        // Set the optional combobox to include all possible outcomes of clearing a labyrinth.
        for (unsigned int i = 0; i<NUMBER_OF_OPTIONAL_LABYRINTH_IDS; ++i) {
            ui->comboBox_optional->addItem(mOptionalListLabyrinth.at(i));
        }
        break;

    case ACTION_ID_MAZE:
        // Set the optional combobox to include all possible outcomes of clearing a maze.
        for (unsigned int i = 0; i<NUMBER_OF_OPTIONAL_MAZE_IDS; ++i) {
            ui->comboBox_optional->addItem(mOptionalListMaze.at(i));
        }
        break;

    case ACTION_ID_MONSTER_DEN:
        // Set the optional combobox to include all possible outcomes of clearing a monster den.
        for (unsigned int i = 0; i<NUMBER_OF_OPTIONAL_MONSTER_DEN_IDS; ++i) {
            ui->comboBox_optional->addItem(mOptionalListMonsterDen.at(i));
        }
        break;

    case ACTION_ID_RUINS: // Ruins
        // Set the optional combobox to include all possible outcomes of clearing a ruin.
        for (unsigned int i = 0; i<NUMBER_OF_OPTIONAL_RUIN_IDS; ++i) {
            ui->comboBox_optional->addItem(mOptionalListRuins.at(i));
        }
        break;

    case ACTION_ID_SPAWNING_GROUNDS:
        // Set the optional combobox to include all possible outcomes of clearing a spawning grounds.
        for (unsigned int i = 0; i<NUMBER_OF_OPTIONAL_SPAWNING_GROUNDS_IDS; ++i) {
            ui->comboBox_optional->addItem(mOptionalListSpawningGrounds.at(i));
        }
        break;

    default:
        break;
    }
}

void Dialog_UserAction::clearScoreTable(void) {
    for (int i = 0; i < NUMBER_OF_SCORE_STATS; ++i) {
        ui->tableWidget_actionStats->setItem(1, i+1, new QTableWidgetItem()); // Load the cells with empty QTableWidgetItems
    }
}

void Dialog_UserAction::enableForm(bool enabled) {
    // Enable / disabled all comboboxes and spinboxes when the PlayerList index changes
    ui->comboBox_actionList->setEnabled(enabled);
    ui->comboBox_optional->setAutoCompletion(enabled);
    ui->comboBox_successFail->setEnabled(enabled);
    ui->spinBox_addFame->setEnabled(enabled);
    ui->spinBox_addRep->setEnabled(enabled);
    ui->spinBox_addCrystals->setEnabled(enabled);
    ui->spinBox_thrownAAC->setEnabled(enabled);
    ui->spinBox_thrownArtifacts->setEnabled(enabled);
    ui->spinBox_woundCounts->setEnabled(enabled);
    ui->pushButton_addMonster->setEnabled(enabled);
    ui->pushButton_addUnit->setEnabled(enabled);
}

void Dialog_UserAction::defineStringLists(void)
{
    mActionList.resize(NUMBER_OF_ACTION_IDS);
    mActionList[ACTION_ID_TEXT]              = "Select Action";
    mActionList[ACTION_ID_BURNED_MONASTERY]  = "Burned Monastery";
    mActionList[ACTION_ID_CITY]              = "City";
    mActionList[ACTION_ID_DUNGEON]           = "Dungeon";
    mActionList[ACTION_ID_INTERACTION]       = "Interaction";
    mActionList[ACTION_ID_KEEP]              = "Keep";
    mActionList[ACTION_ID_LABYRINTH]         = "Labyrinth";
    mActionList[ACTION_ID_MAZE]              = "Maze";
    mActionList[ACTION_ID_MAGE_TOWER]        = "Mage Tower";
    mActionList[ACTION_ID_MONSTER_DEN]       = "Monster Den";
    mActionList[ACTION_ID_PLUNDERED_VILLAGE] = "Plundered Village";
    mActionList[ACTION_ID_RAMPAGING_MONSTER] = "Rampaging Monster";
    mActionList[ACTION_ID_RUINS]             = "Ruins";
    mActionList[ACTION_ID_SPAWNING_GROUNDS]  = "Spawning Grounds";
    mActionList[ACTION_ID_TOMB]              = "Tomb";

    mOptionalListDungeon.resize(NUMBER_OF_OPTIONAL_DUNGEON_IDS);
    mOptionalListDungeon[OPTIONAL_DUNGEON_TEXT]     = "Select Reward";
    mOptionalListDungeon[OPTIONAL_DUNGEON_SPELL]    = "+1 Spell Card";
    mOptionalListDungeon[OPTIONAL_DUNGEON_ARTIFACT] = "+1 Artifact";

    mOptionalListLabyrinth.resize(NUMBER_OF_OPTIONAL_LABYRINTH_IDS);
    mOptionalListLabyrinth[OPTIONAL_LABYRINTH_TEXT]             = "Select Reward";
    mOptionalListLabyrinth[OPTIONAL_LABYRINTH_1_AAC_2_CRYSTALS] = "+1 AAC, +2 Crystals";
    mOptionalListLabyrinth[OPTIONAL_LABYRINTH_1_AAC_1_SPELL]    = "+1 AAC, +1 Spell Cards";
    mOptionalListLabyrinth[OPTIONAL_LABYRINTH_1_AAC_1_ARTIFACT] = "+1 AAC, +1 Artifact";

    mOptionalListMaze.resize(NUMBER_OF_OPTIONAL_MAZE_IDS);
    mOptionalListMaze[OPTIONAL_MAZE_TEXT]       = "Select Reward";
    mOptionalListMaze[OPTIONAL_MAZE_2_CRYSTALS] = "+2 Crystals";
    mOptionalListMaze[OPTIONAL_MAZE_1_SPELL]    = "+1 Spell Cards";
    mOptionalListMaze[OPTIONAL_MAZE_1_ARTIFACT] = "+1 Artifact";

    mOptionalListMonsterDen.resize(NUMBER_OF_OPTIONAL_MONSTER_DEN_IDS);
    mOptionalListMonsterDen[OPTIONAL_MONSTER_DEN_TEXT]             = "Select Reward";
    mOptionalListMonsterDen[OPTIONAL_MONSTER_DEN_2_CRYSTALS]       = "+2 Crystals";
    mOptionalListMonsterDen[OPTIONAL_MONSTER_DEN_1_CRYSTAL_1_FAME] = "+1 Crystal, +1 Fame";
    mOptionalListMonsterDen[OPTIONAL_MONSTER_DEN_2_FAME]           = "+2 Fame";

    mOptionalListRuins.resize(NUMBER_OF_OPTIONAL_RUIN_IDS);
    mOptionalListRuins[OPTIONAL_RUIN_TEXT]               = "Select Reward";
    mOptionalListRuins[OPTIONAL_RUIN_7_FAME]             = "+7 Fame";
    mOptionalListRuins[OPTIONAL_RUIN_10_FAME]            = "+10 Fame";
    mOptionalListRuins[OPTIONAL_RUIN_1_ARTIFACT]         = "+1 Artifact";
    mOptionalListRuins[OPTIONAL_RUIN_2_ARTIFACTS]        = "+2 Artifacts";
    mOptionalListRuins[OPTIONAL_RUIN_1_ARTIFACT_1_AAC]   = "+1 Artifact, +1 AAC";
    mOptionalListRuins[OPTIONAL_RUIN_1_ARTIFACT_1_SPELL] = "+1 Artifact, +1 Spell";
    mOptionalListRuins[OPTIONAL_RUIN_1_SPELL_CRYSTALS]   = "+1 Spell, +Crystals";
    mOptionalListRuins[OPTIONAL_RUIN_CRYSTALS]           = "+Crystals";
    mOptionalListRuins[OPTIONAL_RUIN_UNIT]               = "+1 Unit (selected later)";

    mOptionalListSpawningGrounds.resize(NUMBER_OF_OPTIONAL_SPAWNING_GROUNDS_IDS);
    mOptionalListSpawningGrounds[OPTIONAL_SPAWNING_GROUNDS_TEXT]                       = "Select Reward";
    mOptionalListSpawningGrounds[OPTIONAL_SPAWNING_GROUNDS_ARTIFACT_3_CRYSTALS]        = "+1 Artifact, +3 Crystals";
    mOptionalListSpawningGrounds[OPTIONAL_SPAWNING_GROUNDS_ARTIFACT_2_CRYSTALS_1_FAME] = "+1 Artifact, +2 Crystals, +1 Fame";
    mOptionalListSpawningGrounds[OPTIONAL_SPAWNING_GROUNDS_ARTIFACT_1_CRYSTALS_2_FAME] = "+1 Artifact, +1 Crystal, +2 Fame";
    mOptionalListSpawningGrounds[OPTIONAL_SPAWNING_GROUNDS_ARTIFACT_3_FAME]            = "+1 Artifact, +3 Fame";
}

void Dialog_UserAction::addMonster(void) {
    // The new number of rows in the monster table is increased by one
    int rows = ui->tableWidget_monsters->rowCount() + 1;

    mplayerAction.addMonster(1);
    ui->tableWidget_monsters->setRowCount(rows);

    monsterTableAddRow(unsigned(rows)); // Adds the widgets to the table cells
    monsterTableWidget_valueChanged();  // Triggers the parameter update to the action.monsters

    if (rows > 1) ui->pushButton_deleteMonster->setEnabled(true);
}

void Dialog_UserAction::removeMonster(void)
{
    // The new number of rows in the monster table is increased by one
    int rows = ui->tableWidget_monsters->rowCount() - 1;

    mplayerAction.removeMonster(1);
    ui->tableWidget_monsters->setRowCount(rows);

    if (rows <= 1) ui->pushButton_deleteMonster->setEnabled(false);
}

void Dialog_UserAction::addUnit(Unit unit)
{
    // The new number of rows in the monster table is increased by one
    int rows = ui->tableWidget_units->rowCount() + 1;

    mplayerAction.addUnit(1);
    ui->tableWidget_units->setRowCount(rows);

    unitTableAddRow(unsigned(rows), unit); // Adds the widgets to the table cells
    unitTableWidget_valueChanged();  // Triggers the parameter update to the action.monsters

    if (rows > 1) ui->pushButton_removeUnit->setEnabled(true);
}

void Dialog_UserAction::removeUnit(void)
{
    // The new number of rows in the monster table is increased by one
    int rows = ui->tableWidget_units->rowCount() - 1;

    mplayerAction.removeUnit(1);
    ui->tableWidget_units->setRowCount(rows);

    if (rows <= 1) ui->pushButton_removeUnit->setEnabled(false);
}

void Dialog_UserAction::monsterTableAddRow(unsigned int rows) {
    std::vector<QString> monsterTypes(NUMBER_OF_MONSTER_TYPES);
    std::vector<QColor> monsterTypeColors(NUMBER_OF_MONSTER_TYPES);

    //QStringList monsterTypes;
    QStringList monsterSpecials;

    QSpinBox* monsterFame = new QSpinBox;
    QComboBox* monsterType = new QComboBox;
    QComboBox* monsterSpecial = new QComboBox;
    QCheckBox* rampagingState = new QCheckBox;

    monsterTypes.at(MONSTER_TYPE_TEXT)          = "Select Type";
    monsterTypes.at(MONSTER_TYPE_MARAUDING_ORC) = "Marauding Orc";
    monsterTypes.at(MONSTER_TYPE_KEEP)          = "Keep";
    monsterTypes.at(MONSTER_TYPE_MAGE_TOWER)    = "Mage Tower";
    monsterTypes.at(MONSTER_TYPE_DUNGEON)       = "Dungeon";
    monsterTypes.at(MONSTER_TYPE_CITY)          = "City";
    monsterTypes.at(MONSTER_TYPE_DRACONUM)      = "Draconum";
    monsterTypes.at(MONSTER_TYPE_RUINS)         = "Ruins";

    monsterTypeColors.at(MONSTER_TYPE_TEXT)          = QColor(225, 225, 225);
    monsterTypeColors.at(MONSTER_TYPE_MARAUDING_ORC) = QColor(181, 207, 97 );
    monsterTypeColors.at(MONSTER_TYPE_KEEP)          = QColor(151, 154, 157);
    monsterTypeColors.at(MONSTER_TYPE_MAGE_TOWER)    = QColor(135, 104, 176);
    monsterTypeColors.at(MONSTER_TYPE_DUNGEON)       = QColor(191, 163, 117);
    monsterTypeColors.at(MONSTER_TYPE_CITY)          = QColor(245, 243, 243);
    monsterTypeColors.at(MONSTER_TYPE_DRACONUM)      = QColor(165, 78,  63 );
    monsterTypeColors.at(MONSTER_TYPE_RUINS)         = QColor(254, 226, 128);

    monsterSpecials << "Select Special"
                    << "None"
                    << "Heroes"
                    << "Thugs"
                    << "Elementalist"
                    << "Dark Crusader";

    for (unsigned int i = 0; i < NUMBER_OF_MONSTER_TYPES; ++i) {
        monsterType->addItem(monsterTypes.at(i));
        monsterType->setItemData(int(i), monsterTypeColors.at(i), Qt::BackgroundRole);
    }

    monsterSpecial->addItems(monsterSpecials);

    rampagingState->setEnabled(false);
    rampagingState->setChecked(false);

    ui->tableWidget_monsters->setCellWidget(int(rows)-1, 0, monsterType);
    ui->tableWidget_monsters->setCellWidget(int(rows)-1, 1, monsterFame);
    ui->tableWidget_monsters->setCellWidget(int(rows)-1, 2, monsterSpecial);
    ui->tableWidget_monsters->setCellWidget(int(rows)-1, 3, rampagingState);

    ui->tableWidget_monsters->resizeColumnsToContents();

    connect(monsterFame,    SIGNAL(valueChanged(int)),        this, SLOT(monsterTableWidget_valueChanged()));
    connect(monsterType,    SIGNAL(currentIndexChanged(int)), this, SLOT(monsterTableWidget_valueChanged()));
    connect(monsterSpecial, SIGNAL(currentIndexChanged(int)), this, SLOT(monsterTableWidget_valueChanged()));
    connect(rampagingState, SIGNAL(stateChanged(int)),        this, SLOT(monsterTableWidget_valueChanged()));
}

void Dialog_UserAction::unitTableAddRow(unsigned int rows, Unit unit) {
    QComboBox* unitType = new QComboBox;
    QCheckBox* woundState = new QCheckBox;

    std::vector<QString> unitTypes(NUMBER_OF_UNIT_TYPES);          std::vector<unsigned int> unitLevels(NUMBER_OF_UNIT_TYPES);

    unitTypes.at(UNIT_TYPE_TEXT)             = "Select Unit";
    unitTypes.at(UNIT_TYPE_FORESTERS)        = "Foresters";        unitLevels.at(UNIT_TYPE_FORESTERS)        = 1;
    unitTypes.at(UNIT_TYPE_HERBALISTS)       = "Herbalists";       unitLevels.at(UNIT_TYPE_HERBALISTS)       = 1;
    unitTypes.at(UNIT_TYPE_PEASANTS)         = "Peasants";         unitLevels.at(UNIT_TYPE_PEASANTS)         = 1;
    unitTypes.at(UNIT_TYPE_SCOUTS)           = "Scouts";           unitLevels.at(UNIT_TYPE_SCOUTS)           = 1;
    unitTypes.at(UNIT_TYPE_THUGS)            = "Thugs";            unitLevels.at(UNIT_TYPE_THUGS)            = 1;
    unitTypes.at(UNIT_TYPE_GUARDIAN_GOLEMS)  = "Guardian Golems";  unitLevels.at(UNIT_TYPE_GUARDIAN_GOLEMS)  = 2;
    unitTypes.at(UNIT_TYPE_ILLUSIONISTS)     = "Illusionists";     unitLevels.at(UNIT_TYPE_ILLUSIONISTS)     = 2;
    unitTypes.at(UNIT_TYPE_MAGIC_FAMILIARS)  = "Magic Familiars";  unitLevels.at(UNIT_TYPE_MAGIC_FAMILIARS)  = 2;
    unitTypes.at(UNIT_TYPE_NORTHERN_MONKS)   = "Northern Monks";   unitLevels.at(UNIT_TYPE_NORTHERN_MONKS)   = 2;
    unitTypes.at(UNIT_TYPE_RED_CAPE_MONKS)   = "Red Cape Monks";   unitLevels.at(UNIT_TYPE_RED_CAPE_MONKS)   = 2;
    unitTypes.at(UNIT_TYPE_SAVAGE_MONKS)     = "Savage Monks";     unitLevels.at(UNIT_TYPE_SAVAGE_MONKS)     = 2;
    unitTypes.at(UNIT_TYPE_SHOCKTROOPS)      = "Shocktroops";      unitLevels.at(UNIT_TYPE_SHOCKTROOPS)      = 2;
    unitTypes.at(UNIT_TYPE_UTEM_CROSSBOWMEN) = "Utem Crossbowmen"; unitLevels.at(UNIT_TYPE_UTEM_CROSSBOWMEN) = 2;
    unitTypes.at(UNIT_TYPE_UTEM_GUARDSMEN)   = "Utem Guardsmen";   unitLevels.at(UNIT_TYPE_UTEM_GUARDSMEN)   = 2;
    unitTypes.at(UNIT_TYPE_UTEM_SWORDSMEN)   = "Utem Swordsmen";   unitLevels.at(UNIT_TYPE_UTEM_SWORDSMEN)   = 2;
    unitTypes.at(UNIT_TYPE_AMOTEP_FREEZERS)  = "Amotep Freezers";  unitLevels.at(UNIT_TYPE_AMOTEP_FREEZERS)  = 3;
    unitTypes.at(UNIT_TYPE_AMOTEP_GUNNERS)   = "Amotep Gunners";   unitLevels.at(UNIT_TYPE_AMOTEP_GUNNERS)   = 3;
    unitTypes.at(UNIT_TYPE_CATAPULTS)        = "Catapults";        unitLevels.at(UNIT_TYPE_CATAPULTS)        = 3;
    unitTypes.at(UNIT_TYPE_ICE_GOLEMS)       = "Ice Golems";       unitLevels.at(UNIT_TYPE_ICE_GOLEMS)       = 3;
    unitTypes.at(UNIT_TYPE_ICE_MAGES)        = "Ice Mages";        unitLevels.at(UNIT_TYPE_ICE_MAGES)        = 3;
    unitTypes.at(UNIT_TYPE_FIRE_GOLEMS)      = "Fire Golems";      unitLevels.at(UNIT_TYPE_FIRE_GOLEMS)      = 3;
    unitTypes.at(UNIT_TYPE_FIRE_MAGES)       = "Fire Mages";       unitLevels.at(UNIT_TYPE_FIRE_MAGES)       = 3;
    unitTypes.at(UNIT_TYPE_HEROES_BLUE)      = "Heroes (blue)";    unitLevels.at(UNIT_TYPE_HEROES_BLUE)      = 3;
    unitTypes.at(UNIT_TYPE_HEROES_GREEN)     = "Heroes (green)";   unitLevels.at(UNIT_TYPE_HEROES_GREEN)     = 3;
    unitTypes.at(UNIT_TYPE_HEROES_RED)       = "Heroes (red)";     unitLevels.at(UNIT_TYPE_HEROES_RED)       = 3;
    unitTypes.at(UNIT_TYPE_HEROES_WHITE)     = "Heroes (white)";   unitLevels.at(UNIT_TYPE_HEROES_WHITE)     = 3;
    unitTypes.at(UNIT_TYPE_SORCERERS)        = "Sorcerers";        unitLevels.at(UNIT_TYPE_SORCERERS)        = 3;
    unitTypes.at(UNIT_TYPE_ALTEM_GUARDIANS)  = "Altem Guardians";  unitLevels.at(UNIT_TYPE_ALTEM_GUARDIANS)  = 4;
    unitTypes.at(UNIT_TYPE_ALTEM_MAGES)      = "Altem Mages";      unitLevels.at(UNIT_TYPE_ALTEM_MAGES)      = 4;
    unitTypes.at(UNIT_TYPE_DELPHANA_MASTERS) = "Delphana Masters"; unitLevels.at(UNIT_TYPE_DELPHANA_MASTERS) = 4;

    ui->tableWidget_units->setCellWidget(int(rows)-1, 0, unitType);
    ui->tableWidget_units->setCellWidget(int(rows)-1, 1, woundState);

    unitType->addItem(unitTypes.at(UNIT_TYPE_TEXT));

    unsigned int unitId = 0;

    for (unsigned int i = 1; i < NUMBER_OF_UNIT_TYPES; ++i) {
        QString unitText = unitTypes.at(i) + " (" + QString::number(int(unitLevels.at(i))) + ")";

        unitType->addItem(unitText);

        if (unit.mName == unitText) {
            unitId = i;
        }
    }

    // Check if the specified Unit is an actual unit, or just a placeholder (new unit)
    if (unitId > 0) {
        unitType->setCurrentIndex(int(unitId));
        woundState->setChecked(unit.mWounded);
    }

    ui->tableWidget_units->resizeColumnsToContents();

    connect(unitType,   SIGNAL(currentIndexChanged(int)), this, SLOT(unitTableWidget_valueChanged()));
    connect(woundState, SIGNAL(stateChanged(int)),        this, SLOT(unitTableWidget_valueChanged()));
}

/** ********************
  * USER ACTION EVENTS *
  * AKA. PRIVATE SLOTS *
  **********************/

void Dialog_UserAction::on_dialogSetupDataReceived(const std::vector<Player>& playerList, const std::vector<City>& cityList) {
    // Store the transmitted values as local copies
    mPlayerListCopy = playerList;
    mCityListCopy = cityList;

    defineStringLists(); // Declares the string lists used in the different comboboxes
    ui_userActionSetupClean();

    // Const pointer to the Singleton instance of the GameEngine class (global object)
    const GameEngine* gameEngine = GameEngine::instance();
    connect(this, &Dialog_UserAction::newTestUserAction, gameEngine, &GameEngine::on_newTestUserAction);
}

void Dialog_UserAction::on_newPlayerAndCityData(const std::vector<Player>& playerList, const std::vector<City>& cityList) {
    // Store the new transmitted values as local copies
    mPlayerListCopy = playerList;
    mCityListCopy = cityList;

    emit newTestUserAction(mplayerAction);
}

void Dialog_UserAction::on_newTempPlayerData(const Player& player, const Player& playerTemp) {
    std::vector<QString> actionResultList(NUMBER_OF_SCORE_STATS);

             int deltaScore    = playerTemp.mScore             - player.mScore;
    unsigned int deltaFame     = playerTemp.mFame              - player.mFame;
             int deltaRep      = playerTemp.mReputation        - player.mReputation;
             int deltaRepStep  = playerTemp.mRepStep           - player.mRepStep;
    unsigned int deltaLevel    = playerTemp.mLevel             - player.mLevel;
             int deltaAAC      = int(playerTemp.mAActionCards) - int(player.mAActionCards);
             int deltaSpell    = int(playerTemp.mSpellCards)   - int(player.mSpellCards);
             int deltaArtifact = int(playerTemp.mArtifacts)    - int(player.mArtifacts);
             int deltaCrystals = int(playerTemp.mCrystals)     - int(player.mCrystals);

    actionResultList.at(SCORE_STAT_ID_SCORE)    = QString::number(playerTemp.mScore)        + "(" + QString::number(deltaScore)    + ")";
    actionResultList.at(SCORE_STAT_ID_FAME)     = QString::number(playerTemp.mFame)         + "(" + QString::number(deltaFame)     + ")";
    actionResultList.at(SCORE_STAT_ID_REP)      = QString::number(playerTemp.mReputation)   + "(" + QString::number(deltaRep)      + ")" + "\n" + QString::number(int(playerTemp.mRepStep) - 7) + "(" + QString::number(deltaRepStep)      + ")";
    actionResultList.at(SCORE_STAT_ID_LEVEL)    = QString::number(playerTemp.mLevel)        + "(" + QString::number(deltaLevel)    + ")";
    actionResultList.at(SCORE_STAT_ID_AAC)      = QString::number(playerTemp.mAActionCards) + "(" + QString::number(deltaAAC)      + ")";
    actionResultList.at(SCORE_STAT_ID_SPELL)    = QString::number(playerTemp.mSpellCards)   + "(" + QString::number(deltaSpell)    + ")";
    actionResultList.at(SCORE_STAT_ID_ARTIFACT) = QString::number(playerTemp.mArtifacts)    + "(" + QString::number(deltaArtifact) + ")";
    actionResultList.at(SCORE_STAT_ID_CRYSTALS) = QString::number(playerTemp.mCrystals)     + "(" + QString::number(deltaCrystals) + ")";

    for (unsigned int i = 0; i < NUMBER_OF_SCORE_STATS; ++i) {
        ui->tableWidget_actionStats->setItem(1, int(i), new QTableWidgetItem(actionResultList.at(i)));

        if ((i == SCORE_STAT_ID_LEVEL) && (deltaLevel > 0)) {
            QColor playerColor = player.mPlayerColor;
            ui->tableWidget_actionStats->item(1, int(i))->setBackground(QBrush(playerColor));
        }
    }
}

void Dialog_UserAction::on_pushButton_enterUserAction_clicked() {
    emit newUserAction(mplayerAction);
    this->accept();
}

void Dialog_UserAction::on_pushButton_cancel_clicked() {
    this->reject();
}

void Dialog_UserAction::on_pushButton_addMonster_clicked()
{
    addMonster(); // Adds a monster to the action monster list and displays it on the monster table
    acceptButtonControlCheck();

    unsigned int i = rand() % 8;
    mSoundEffects[i].play();
}

void Dialog_UserAction::on_pushButton_deleteMonster_clicked()
{
    removeMonster(); // Adds a monster to the action monster list and displays it on the monster table
    emit newTestUserAction(mplayerAction);
    //updateResults();
    acceptButtonControlCheck();
}

void Dialog_UserAction::on_pushButton_addUnit_clicked()
{
    Unit unit;
    addUnit(unit);
    acceptButtonControlCheck();
}

void Dialog_UserAction::on_pushButton_removeUnit_clicked() {
    removeUnit(); // Adds a monster to the action monster list and displays it on the monster table
    emit newTestUserAction(mplayerAction);
    acceptButtonControlCheck();
}

void Dialog_UserAction::on_comboBox_playerList_currentIndexChanged(int index) {
    // Clear the score view, monsterlist and unitList and reset the action instance
    clearScoreTable();

    // Clear the monster and unit lists
    ui->tableWidget_monsters->setRowCount(1);
    ui->tableWidget_units->setRowCount(1);
    ui->pushButton_deleteMonster->setEnabled(false);
    ui->pushButton_removeUnit->setEnabled(false);

    mplayerAction.reset();

    if (index > 0) {
        mplayerAction.mPlayerID = unsigned(index);
        emit newTestUserAction(mplayerAction);
        //updateResults();
        enableForm(true);

        unsigned int nUnits = mPlayerListCopy.at(unsigned(index-1)).mUnits.size();
        for (unsigned int i = 0; i < nUnits; ++i) {
            addUnit(mPlayerListCopy.at(unsigned(index-1)).mUnits.at(i));
        }
    }
    else {
        enableForm(false);
    }

    acceptButtonControlCheck();
}

void Dialog_UserAction::on_comboBox_actionList_currentIndexChanged(int index) {
    mplayerAction.mPlayerID = unsigned(ui->comboBox_playerList->currentIndex());
    mplayerAction.mMainActionID = unsigned(index);

    clearScoreTable();

    ui->comboBox_successFail->setCurrentIndex(0);
    setOptionalComboBox(index);

    switch (index) {
    case ACTION_ID_TEXT: // No Action
    case ACTION_ID_RAMPAGING_MONSTER:
    case ACTION_ID_INTERACTION:
    case ACTION_ID_PLUNDERED_VILLAGE:
        ui->comboBox_optional->setEnabled(false);    // Not used
        ui->comboBox_successFail->setEnabled(false); // Not used
        break;

    case ACTION_ID_MAGE_TOWER:
    case ACTION_ID_KEEP:
    case ACTION_ID_BURNED_MONASTERY:
    case ACTION_ID_TOMB:
        ui->comboBox_optional->setEnabled(false);    // Not used
        ui->comboBox_successFail->setEnabled(true);  // Used to define success/fail state
        break;

    case ACTION_ID_DUNGEON:
    case ACTION_ID_LABYRINTH:
    case ACTION_ID_MAZE:
    case ACTION_ID_MONSTER_DEN:
    case ACTION_ID_RUINS:
    case ACTION_ID_SPAWNING_GROUNDS:
        ui->comboBox_optional->setEnabled(true);     // Used to set reward if succeeded
        ui->comboBox_successFail->setEnabled(true);  // Used to define success/fail state
        break;

    case ACTION_ID_CITY: // City
        ui->comboBox_optional->setEnabled(true);     // Used to select attacked city
        ui->comboBox_successFail->setEnabled(false); // Not used
        break;

    default:
        break;
    }

    monsterTableWidget_valueChanged(); // Trigger a check of the specified monsters using the new action id.
    acceptButtonControlCheck();        // Check if the accept button can be enabled (all necessary settings set)
    emit newTestUserAction(mplayerAction);
}

void Dialog_UserAction::on_comboBox_optional_currentIndexChanged(int index)
{
    mplayerAction.mOptionalID = unsigned(index);

    if ((ui->comboBox_actionList->currentIndex() == ACTION_ID_CITY) && (index > 0)) {
        mplayerAction.mCityID = unsigned(index-1);
    }

    acceptButtonControlCheck();
    emit newTestUserAction(mplayerAction);
    //updateResults();
}

void Dialog_UserAction::on_comboBox_successFail_currentIndexChanged(const QString &arg1)
{
    if      (arg1 == "Success") mplayerAction.mSuccessFail = true;
    else if (arg1 == "Fail")    mplayerAction.mSuccessFail = false;
    else                        mplayerAction.mSuccessFail = false;

    acceptButtonControlCheck();
    emit newTestUserAction(mplayerAction);
    // updateResults();
}

void Dialog_UserAction::on_spinBox_woundCounts_valueChanged(int arg1)
{
    unsigned int playerIndex = mplayerAction.mPlayerID - 1;
    unsigned int availableWounds = mPlayerListCopy.at(playerIndex).mWounds;

    if(-1 * int(availableWounds) > arg1)
    {
        mplayerAction.mWounds = int(availableWounds);
        ui->spinBox_woundCounts->setValue(-1 * int(availableWounds));
    }
    else
    {
        mplayerAction.mWounds = arg1;
    }

    acceptButtonControlCheck();
    emit newTestUserAction(mplayerAction);
    //updateResults();
}

void Dialog_UserAction::on_spinBox_addRep_valueChanged(int arg1)
{
    mplayerAction.mAddRepStep = arg1;

    acceptButtonControlCheck();
    emit newTestUserAction(mplayerAction);
    //updateResults();
}

void Dialog_UserAction::on_spinBox_addFame_valueChanged(int arg1)
{
    mplayerAction.mAddFame = unsigned(arg1);

    acceptButtonControlCheck();
    emit newTestUserAction(mplayerAction);
    //updateResults();
}

void Dialog_UserAction::on_spinBox_addCrystals_valueChanged(int arg1)
{
    unsigned int playerIndex = mplayerAction.mPlayerID - 1;
    int availableCrystals = int(mPlayerListCopy.at(playerIndex).mCrystals);

    if ((availableCrystals + arg1) <= 0)
    {
        mplayerAction.mCrystals = -1 * availableCrystals;
        ui->spinBox_addCrystals->setValue(-1 * availableCrystals);
    }
    else
    {
        mplayerAction.mCrystals = unsigned(arg1);
    }

    acceptButtonControlCheck();
    emit newTestUserAction(mplayerAction);
    //updateResults();
}

void Dialog_UserAction::on_spinBox_thrownArtifacts_valueChanged(int arg1)
{
    unsigned int playerIndex = mplayerAction.mPlayerID - 1;
    unsigned int availableArtifacts = mPlayerListCopy.at(playerIndex).mArtifacts;

    if(int(availableArtifacts) < arg1)
    {
        mplayerAction.mThrownArtifacts = availableArtifacts;
        ui->spinBox_thrownArtifacts->setValue(int(availableArtifacts));
    }
    else
    {
        mplayerAction.mThrownArtifacts = unsigned(arg1);
    }

    acceptButtonControlCheck();
    emit newTestUserAction(mplayerAction);
    //updateResults();
}

void Dialog_UserAction::on_spinBox_thrownAAC_valueChanged(int arg1)
{
    unsigned int playerIndex = mplayerAction.mPlayerID - 1;
    unsigned int availableAAC = mPlayerListCopy.at(playerIndex).mAActionCards;

    if (int(availableAAC) < arg1)
    {
        mplayerAction.mThrownAAC = availableAAC;
        ui->spinBox_thrownAAC->setValue(int(availableAAC));
    }
    else
    {
        mplayerAction.mThrownAAC = unsigned(arg1);
    }


    acceptButtonControlCheck();
    emit newTestUserAction(mplayerAction);
    //updateResults();
}

void Dialog_UserAction::monsterTableWidget_valueChanged(void)
{
    int nMonsters = ui->tableWidget_monsters->rowCount()-1;

    for (int i = 1; i<=nMonsters; ++i)
    {
        QComboBox *typeCB = qobject_cast<QComboBox*>(ui->tableWidget_monsters->cellWidget(i, 0));
        QSpinBox *fameSB = qobject_cast<QSpinBox*>(ui->tableWidget_monsters->cellWidget(i, 1));
        QComboBox *specialCB = qobject_cast<QComboBox*>(ui->tableWidget_monsters->cellWidget(i, 2));
        QCheckBox *isRampagingCB = qobject_cast<QCheckBox*>(ui->tableWidget_monsters->cellWidget(i,3));

        QString typeText = typeCB->currentText();

        if (typeText == "Draconum" || typeText == "Marauding Orc")
        {
            // When the specified action is Rampaging monster, the specified Orcs and/or Draconum can only be Rampaging
            // Thus, disable the checkbox control and set the state to checked = true.
            if (ui->comboBox_actionList->currentIndex() == ACTION_ID_RAMPAGING_MONSTER)
            {
                isRampagingCB->setEnabled(false);
                isRampagingCB->setChecked(true);

            }
            else // For any other action, the checkbox is enabled for editing
            {
                isRampagingCB->setEnabled(true);
            }
        }
        else // When the specified monster is any other type than Orcs and Draconum, then disable and uncheck the checkbox
        {
            isRampagingCB->setEnabled(false);
            isRampagingCB->setChecked(false);
        }

        unsigned int fameNumber = unsigned(fameSB->value());
        QString specialText = specialCB->currentText();
        bool rampagingState = isRampagingCB->isChecked();

        mplayerAction.mMonsters.at(unsigned(i-1)).mType = typeText;
        mplayerAction.mMonsters.at(unsigned(i-1)).mFame = fameNumber;
        mplayerAction.mMonsters.at(unsigned(i-1)).mSpecial = specialText;
        mplayerAction.mMonsters.at(unsigned(i-1)).mRampaging = rampagingState;
    }

    acceptButtonControlCheck();
    emit newTestUserAction(mplayerAction);
    //updateResults();
}

void Dialog_UserAction::unitTableWidget_valueChanged(void)
{
    int nUnits = ui->tableWidget_units->rowCount()-1;

    for (int i = 0; i<nUnits; ++i)
    {
        QComboBox *typeCB = qobject_cast<QComboBox*>(ui->tableWidget_units->cellWidget(i+1,0));
        QCheckBox *woundedCB = qobject_cast<QCheckBox*>(ui->tableWidget_units->cellWidget(i+1,1));

        QString typeText = typeCB->currentText();
        bool woundedState = woundedCB->isChecked();

        mplayerAction.mUnits.at(unsigned(i)).mName = typeText;

        if (typeText.at(typeText.length()-2).isDigit())
        {
            mplayerAction.mUnits.at(unsigned(i)).mLevel = unsigned(typeText.at(typeText.length()-2).digitValue());
        }
        else
        {
            mplayerAction.mUnits.at(unsigned(i)).mLevel = 0;
        }

        mplayerAction.mUnits.at(unsigned(i)).mWounded = woundedState;
    }

    acceptButtonControlCheck();
    emit newTestUserAction(mplayerAction);
    //updateResults();
}
