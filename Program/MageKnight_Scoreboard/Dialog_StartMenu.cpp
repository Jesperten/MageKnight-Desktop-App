#include "Dialog_StartMenu.h"
#include "ui_Dialog_StartMenu.h"
#include "CharacterDescriptionTexts.h"

#define STYLE_CHARACTER_CLEAN     " QFrame {background-color: rgb(0, 0, 0, 20%); border: 1px solid white;}"
#define STYLE_CHARACTER_ARYTHEA   " QFrame {border-image: url(:/images/images/CharacterArythea.jpg); border: 1px solid white;}"
#define STYLE_CHARACTER_BREAVELAR " QFrame {border-image: url(:/images/images/CharacterBraevelar.jpg); border: 1px solid white;}"
#define STYLE_CHARACTER_KRANG     " QFrame {border-image: url(:/images/images/CharacterKrang.jpg); border: 1px solid white;}"
#define STYLE_CHARACTER_GOLDYX    " QFrame {border-image: url(:/images/images/CharacterGoldyx.jpg); border: 1px solid white;}"
#define STYLE_CHARACTER_NOROWAS   " QFrame {border-image: url(:/images/images/CharacterNorowas.jpg); border: 1px solid white;}"
#define STYLE_CHARACTER_TOVAK     " QFrame {border-image: url(:/images/images/CharacterTovak.jpg); border: 1px solid white;}"
#define STYLE_CHARACTER_WOLFHAWK  " QFrame {border-image: url(:/images/images/CharacterWolfhawk.jpg); border: 1px solid white;}"

Dialog_StartMenu::Dialog_StartMenu(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_StartMenu)
{
    ui->setupUi(this);

    // Set background image and initial index on the stacked widget
    ui->stackedWidget->setStyleSheet("background-image:url(./images/Start_background.jpg)");
    ui->stackedWidget->setCurrentIndex(0);
}

Dialog_StartMenu::~Dialog_StartMenu()
{
    delete ui;
}

// Public Slots
void Dialog_StartMenu::on_playerListUpdated(const std::vector<Player>& mageKnight_Players)
{
    // When the player list on the GameEngine changes, the updated list is sent to
    // the StartMenu to display all players on the dedicated list
    unsigned int nPlayers = unsigned(mageKnight_Players.size());

    ui->tableWidget_listOfPlayers->setRowCount(int(nPlayers));

    for (unsigned int i = 0; i<nPlayers; i++)
    {
        //TODO: add a button on each row to delete a character?
        ui->tableWidget_listOfPlayers->setItem(int(i), 0, new QTableWidgetItem(QString::number(mageKnight_Players.at(i).mId)));
        ui->tableWidget_listOfPlayers->setItem(int(i), 1, new QTableWidgetItem(mageKnight_Players.at(i).mName));
        ui->tableWidget_listOfPlayers->setItem(int(i), 2, new QTableWidgetItem(mageKnight_Players.at(i).mCharacter));
    }

    ui->tableWidget_listOfPlayers->resizeColumnsToContents();

    startButtonControlCheck();
}

// Private Slots
void Dialog_StartMenu::on_pushButton_newGame_clicked()
{
    ui_newGameSetupClean(); // Create a clean view for the "New Game" menu
    ui->stackedWidget->setCurrentIndex(1); // Go to the "New Game" menu view
}

void Dialog_StartMenu::on_pushButton_loadGame_clicked()
{

}

void Dialog_StartMenu::on_pushButton_Exit_clicked()
{
    this->close();
}

void Dialog_StartMenu::on_pushButton_returnToStart_clicked()
{
    // Clear all created players and clear the table
    ui->tableWidget_listOfPlayers->clear();
    ui->stackedWidget->setCurrentIndex(0); // Go back to the "Start" menu view
}

void Dialog_StartMenu::on_pushButton_startGame_clicked()
{
    emit startNewGame();
    this->accept();
}

void Dialog_StartMenu::on_pushButton_addPlayer_clicked()
{
    QString playerName = ui->lineEdit_playerName->text();
    QString playerCharacter = ui->comboBox_playerCharacter->currentText();

    ui->comboBox_playerCharacter->setCurrentIndex(0);
    ui->lineEdit_playerName->clear();

    // Send the new player to the GameEngine.
    emit mageKnightPlayerAdded(playerName, playerCharacter);
}

void Dialog_StartMenu::on_comboBox_playerCharacter_currentIndexChanged(const QString &arg1)
{
    if((arg1 == "Select Character") || (ui->lineEdit_playerName->text() == ""))
    {
        ui->pushButton_addPlayer->setEnabled(false);
    }
    else
    {
        ui->pushButton_addPlayer->setEnabled(true);
    }

    bool nameCheck = false;

    for (int i = 0; i< ui->comboBox_playerCharacter->count(); ++i)
    {
        QString playerName = ui->lineEdit_playerName->text();
        QString Character = ui->comboBox_playerCharacter->itemText(i);

        if(playerName == Character)
            nameCheck = true;
    }


    if((ui->lineEdit_playerName->text() == "" || nameCheck) && (arg1 != "Select Character")) {
        ui->lineEdit_playerName->setText(arg1);
    }

    // Change the appearing character image and description
    if (arg1 == "Arythea")
    {
        ui->frameCharacter->setStyleSheet(STYLE_CHARACTER_ARYTHEA);
        ui->textBrowser_characterDescription->setText(CHARACTER_DESCRIPTION_ARYTHEA);
    }
    else if (arg1 == "Braevalar")
    {
        ui->frameCharacter->setStyleSheet(STYLE_CHARACTER_BREAVELAR);
        ui->textBrowser_characterDescription->setText(CHARACTER_DESCRIPTION_BRAEVALAR);
    }
    else if(arg1 == "Goldyx")
    {
        ui->frameCharacter->setStyleSheet(STYLE_CHARACTER_GOLDYX);
        ui->textBrowser_characterDescription->setText(CHARACTER_DESCRIPTION_GOLDYX);
    }
    else if(arg1 == "Krang")
    {
        ui->frameCharacter->setStyleSheet(STYLE_CHARACTER_KRANG);
        ui->textBrowser_characterDescription->setText(CHARACTER_DESCRIPTION_KRANG);
    }
    else if(arg1 == "Norowas")
    {
        ui->frameCharacter->setStyleSheet(STYLE_CHARACTER_NOROWAS);
        ui->textBrowser_characterDescription->setText(CHARACTER_DESCRIPTION_NOROWAS);
    }
    else if(arg1 == "Tovak")
    {
        ui->frameCharacter->setStyleSheet(STYLE_CHARACTER_TOVAK);
        ui->textBrowser_characterDescription->setText(CHARACTER_DESCRIPTION_TOVAK);
    }
    else if (arg1 == "Wolfhawk")
    {
        ui->frameCharacter->setStyleSheet(STYLE_CHARACTER_WOLFHAWK);
        ui->textBrowser_characterDescription->setText(CHARACTER_DESCRIPTION_WOLFHAWK);
    }
}

void Dialog_StartMenu::on_lineEdit_playerName_textChanged(const QString &arg1)
{
    if((arg1 == "") || (ui->comboBox_playerCharacter->currentText() == "Select Character")){
        ui->pushButton_addPlayer->setEnabled(false);
    }
    else {
        ui->pushButton_addPlayer->setEnabled(true);
    }
}

// Private methods
void Dialog_StartMenu::ui_newGameSetupClean()
{
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

    startButtonControlCheck();
}

void Dialog_StartMenu::startButtonControlCheck()
{
    // Check the setup status before enabling the "Start Game" Button
    if(ui->tableWidget_listOfPlayers->rowCount() > 0)
    {
        ui->pushButton_startGame->setEnabled(true);
    }
    else
    {
        ui->pushButton_startGame->setEnabled(false);
    }
}
