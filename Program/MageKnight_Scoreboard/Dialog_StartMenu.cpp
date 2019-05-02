#include "ui_Dialog_StartMenu.h"
#include "Dialog_StartMenu.h"
#include "CharacterDescriptionTexts.h"

#define STYLE_CHARACTER_CLEAN     " QFrame {background-color: rgb(0, 0, 0, 20%); border: 1px solid white;}"
#define STYLE_CHARACTER_ARYTHEA   " QFrame {border-image: url(:/images/Image_Character_Arythea);   border: 1px solid white;}"
#define STYLE_CHARACTER_BREAVELAR " QFrame {border-image: url(:/images/Image_Character_Braevelar); border: 1px solid white;}"
#define STYLE_CHARACTER_KRANG     " QFrame {border-image: url(:/images/Image_Character_Krang);     border: 1px solid white;}"
#define STYLE_CHARACTER_GOLDYX    " QFrame {border-image: url(:/images/Image_Character_Goldyx);    border: 1px solid white;}"
#define STYLE_CHARACTER_NOROWAS   " QFrame {border-image: url(:/images/Image_Character_Norowas);   border: 1px solid white;}"
#define STYLE_CHARACTER_TOVAK     " QFrame {border-image: url(:/images/Image_Character_Tovak);     border: 1px solid white;}"
#define STYLE_CHARACTER_WOLFHAWK  " QFrame {border-image: url(:/images/Image_Character_Wolfhawk);  border: 1px solid white;}"

Dialog_StartMenu::Dialog_StartMenu(QWidget *parent) : QDialog(parent), ui(new Ui::Dialog_StartMenu) {
    ui->setupUi(this);

    // Set initial index on the stacked widget
    ui->stackedWidget->setCurrentIndex(0);

    ui->label_GeneralVolkare->setToolTip("<font color='black'>Not much is known about Volkare, the High Priest of Høxies Kræstendom. Whether he is good or bad, or whether he follows higher goals or his own personal agenda</font>");
    ui->label_GeneralVolkare->setToolTipDuration(15000);

    Volkare.mColor = "1337BiddybobFirkant";
    Volkare.mName = "Volkare";
}

Dialog_StartMenu::~Dialog_StartMenu() {
    delete ui;
}

// Public Slots
void Dialog_StartMenu::on_playerListUpdated(const std::vector<Player>& mageKnight_Players) {
    // When the player list on the GameEngine changes, the updated list is sent to
    // the StartMenu to display all players on the dedicated list
    unsigned int nPlayers = unsigned(mageKnight_Players.size());

    ui->tableWidget_listOfPlayers->setRowCount(int(nPlayers));

    for (unsigned int i = 0; i<nPlayers; i++) {
        //TODO: add a button on each row to delete a character?
        ui->tableWidget_listOfPlayers->setItem(int(i), 0, new QTableWidgetItem(QString::number(mageKnight_Players.at(i).mId)));
        ui->tableWidget_listOfPlayers->setItem(int(i), 1, new QTableWidgetItem(mageKnight_Players.at(i).mName));
        ui->tableWidget_listOfPlayers->setItem(int(i), 2, new QTableWidgetItem(mageKnight_Players.at(i).mCharacter));

        ui->tableWidget_listOfPlayers->item(i, 0)->setTextColor(mageKnight_Players.at(i).mPlayerColor);
        ui->tableWidget_listOfPlayers->item(i, 1)->setTextColor(mageKnight_Players.at(i).mPlayerColor);
        ui->tableWidget_listOfPlayers->item(i, 2)->setTextColor(mageKnight_Players.at(i).mPlayerColor);
    }

    ui->tableWidget_listOfPlayers->resizeColumnsToContents();

    startButtonControlCheck();
}

// Private Slots
void Dialog_StartMenu::on_pushButton_newGame_clicked() {
    ui_newGameSetupClean(); // Create a clean view for the "New Game" menu
    ui->stackedWidget->setCurrentIndex(1); // Go to the "New Game" menu view

    /*
    // Test purpose only (for faster game start)
    emit playerAdded("Lars", "Arythea");
    emit playerAdded("John", "Braevelar");
    emit playerAdded("Will", "Goldyx");
    emit playerAdded("Bent", "Wolfhawk");
    emit playerAdded("Finn", "Tovak");
    emit playerAdded("Mark", "Norowas");
    */
}

void Dialog_StartMenu::on_pushButton_loadGame_clicked() {

}

void Dialog_StartMenu::on_pushButton_Exit_clicked() {
    this->close();
}

void Dialog_StartMenu::on_pushButton_returnToStart_clicked() {
    // Clear all created players and clear the table
    ui->tableWidget_listOfPlayers->clear();
    ui->stackedWidget->setCurrentIndex(0); // Go back to the "Start" menu view
    emit resetGame();
}

void Dialog_StartMenu::on_pushButton_startGame_clicked() {
    emit startNewGame(ui->checkBox_Volkare->isChecked(), Volkare);
    this->accept();
}

void Dialog_StartMenu::on_pushButton_addPlayer_clicked() {
    QString playerName = ui->lineEdit_playerName->text();
    QString playerCharacter = ui->comboBox_playerCharacter->currentText();

    ui->comboBox_playerCharacter->setCurrentIndex(0);
    ui->lineEdit_playerName->clear();

    // Send the new player to the GameEngine.
    emit playerAdded(playerName, playerCharacter);
}

void Dialog_StartMenu::on_comboBox_playerCharacter_currentIndexChanged(const QString &arg1) {
    if ((arg1 == "Select Character") || (ui->lineEdit_playerName->text() == ""))
        ui->pushButton_addPlayer->setEnabled(false);
    else
        ui->pushButton_addPlayer->setEnabled(true);

    bool nameCheck = false;

    for (int i = 0; i< ui->comboBox_playerCharacter->count(); ++i) {
        QString playerName = ui->lineEdit_playerName->text();
        QString Character = ui->comboBox_playerCharacter->itemText(i);

        if(playerName == Character)
            nameCheck = true;
    }


    if ((ui->lineEdit_playerName->text() == "" || nameCheck) && (arg1 != "Select Character"))
        ui->lineEdit_playerName->setText(arg1);

    // Change the appearing character image and description
    if (arg1 == "Arythea") {
        ui->frameCharacter->setStyleSheet(STYLE_CHARACTER_ARYTHEA);
        ui->textBrowser_characterDescription->setText(CHARACTER_DESCRIPTION_ARYTHEA);
    }
    else if (arg1 == "Braevalar") {
        ui->frameCharacter->setStyleSheet(STYLE_CHARACTER_BREAVELAR);
        ui->textBrowser_characterDescription->setText(CHARACTER_DESCRIPTION_BRAEVALAR);
    }
    else if (arg1 == "Goldyx") {
        ui->frameCharacter->setStyleSheet(STYLE_CHARACTER_GOLDYX);
        ui->textBrowser_characterDescription->setText(CHARACTER_DESCRIPTION_GOLDYX);
    }
    else if (arg1 == "Krang") {
        ui->frameCharacter->setStyleSheet(STYLE_CHARACTER_KRANG);
        ui->textBrowser_characterDescription->setText(CHARACTER_DESCRIPTION_KRANG);
    }
    else if (arg1 == "Norowas") {
        ui->frameCharacter->setStyleSheet(STYLE_CHARACTER_NOROWAS);
        ui->textBrowser_characterDescription->setText(CHARACTER_DESCRIPTION_NOROWAS);
    }
    else if (arg1 == "Tovak") {
        ui->frameCharacter->setStyleSheet(STYLE_CHARACTER_TOVAK);
        ui->textBrowser_characterDescription->setText(CHARACTER_DESCRIPTION_TOVAK);
    }
    else if (arg1 == "Wolfhawk") {
        ui->frameCharacter->setStyleSheet(STYLE_CHARACTER_WOLFHAWK);
        ui->textBrowser_characterDescription->setText(CHARACTER_DESCRIPTION_WOLFHAWK);
    }
    else {
        ui->frameCharacter->setStyleSheet(STYLE_CHARACTER_CLEAN);
        ui->textBrowser_characterDescription->setText("");
    }
}

void Dialog_StartMenu::on_lineEdit_playerName_textChanged(const QString &arg1) {
    if ((arg1 == "") || (ui->comboBox_playerCharacter->currentText() == "Select Character"))
        ui->pushButton_addPlayer->setEnabled(false);
    else
        ui->pushButton_addPlayer->setEnabled(true);
}

// Private methods
void Dialog_StartMenu::ui_newGameSetupClean() {
    ui->tableWidget_listOfPlayers->setRowCount(0);
    ui->tableWidget_listOfPlayers->setColumnCount(3);
    m_TableHeader<<"#"<<"Name"<<"Character";
    ui->tableWidget_listOfPlayers->setHorizontalHeaderLabels(m_TableHeader);
    ui->tableWidget_listOfPlayers->verticalHeader()->setVisible(false);
    ui->tableWidget_listOfPlayers->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget_listOfPlayers->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget_listOfPlayers->setSelectionMode(QAbstractItemView::NoSelection);
    ui->tableWidget_listOfPlayers->setShowGrid(false);
    ui->tableWidget_listOfPlayers->setFocusPolicy(Qt::NoFocus);

    ui->tableWidget_listOfPlayers->resizeColumnsToContents();
    ui->lineEdit_playerName->setPlaceholderText("Name");
    ui->textBrowser_characterDescription->setText("");
    ui->checkBox_Volkare->setChecked(true);
    startButtonControlCheck();
}

void Dialog_StartMenu::startButtonControlCheck() {
    // Check the setup status before enabling the "Start Game" Button
    bool finalStatement = true;
    QString button_toolTip = "";

    if (ui->tableWidget_listOfPlayers->rowCount() == 0) {
        finalStatement = false;
        button_toolTip = "Minimum one MageKnight must enter the forsaken lands before the journey can begin.";
    }
    else if ((ui->checkBox_Volkare->isChecked() == true) && ((ui->spinBox_VolkareLegion->value() == 0) || (ui->spinBox_VolkareLevel->value() == 0))) {
        finalStatement = false;
        button_toolTip = "Volkare Level and Lost Legion count must be specified before the game can begin.";
    }

    ui->pushButton_startGame->setEnabled(finalStatement);


    ui->pushButton_startGame->setToolTip(button_toolTip);

    if (button_toolTip != "") {
        ui->pushButton_startGame->setToolTipDuration(10000);
    }
    else {
        ui->pushButton_startGame->setToolTipDuration(-1);
    }
}

void Dialog_StartMenu::on_checkBox_Volkare_clicked() {
    startButtonControlCheck();
}

void Dialog_StartMenu::on_spinBox_VolkareLevel_valueChanged(int arg1) {
    Volkare.mLevel = arg1;
    startButtonControlCheck();
}

void Dialog_StartMenu::on_spinBox_VolkareLegion_valueChanged(int arg1) {
    Volkare.mMonsters = arg1;
    Volkare.mMonstersRemaining = arg1;
    startButtonControlCheck();
}
