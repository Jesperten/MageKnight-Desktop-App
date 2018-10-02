#include "Window_StartMenu.h"
#include "ui_Window_StartMenu.h"
#include "Window_MainBoard.h"
#include "Player.h"

Window_StartMenu::Window_StartMenu(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Window_StartMenu)
{
    ui->setupUi(this);
    ui->stackedWidget->setStyleSheet("background-image:url(./images/Start_background.jpg)");
    ui->stackedWidget->setCurrentIndex(0);

    mStyle_CharacterClean     = " QFrame {background-color: rgb(0, 0, 0, 20%); border: 1px solid white;}";
    mStyle_CharacterArythea   = " QFrame {border-image: url(:/images/images/CharacterArythea.jpg); border: 1px solid white;}";
    mStyle_CharacterGoldyx    = " QFrame {border-image: url(:/images/images/CharacterGoldyx.jpg); border: 1px solid white;}";
    mStyle_CharacterNorowas   = " QFrame {border-image: url(:/images/images/CharacterNorowas.jpg); border: 1px solid white;}";
    mStyle_CharacterTovak     = " QFrame {border-image: url(:/images/images/CharacterTovak.jpg); border: 1px solid white;}";
    mStyle_CharacterKrang     = " QFrame {border-image: url(:/images/images/CharacterKrang.jpg); border: 1px solid white;}";
}

Window_StartMenu::~Window_StartMenu()
{
    delete ui;
}

void Window_StartMenu::on_pushButton_newGame_clicked()
{
    ui_newGameSetupClean(); // Create a clean view for the "New Game" menu
    ui->stackedWidget->setCurrentIndex(1); // Go to the "New Game" menu view
}

void Window_StartMenu::on_pushButton_returnToStart_clicked()
{
    // Clear all created players and clear the table
    // TODO: create a "All_Players_destruction" method and call it from here
    ui->tableWidget_listOfPlayers->clear();
    ui->stackedWidget->setCurrentIndex(0); // Go back to the "Start" menu view
}

void Window_StartMenu::on_pushButton_Exit_clicked()
{
    this->close();
}

void Window_StartMenu::on_pushButton_startGame_clicked()
{
    this->hide();

    unsigned int number_of_players = ui->tableWidget_listOfPlayers->rowCount();
    unsigned int number_of_cities = ui->spinBox_cityCount->value();
    unsigned int number_of_days = ui->spinBox_dayCount->value();

    mWindow_MainBoard = new Window_MainBoard();
    mWindow_MainBoard->allocatePlayers(number_of_players);
    mWindow_MainBoard->allocateCities(number_of_cities);
    mWindow_MainBoard->setDayCount(number_of_days);

    int playerID = 0;
    QString playerName = "";
    QString playerCharacter = "";

    for (unsigned int i = 1; i<=number_of_players; ++i)
    {
        playerID = i;
        playerName = ui->tableWidget_listOfPlayers->item(playerID-1, 1)->text();
        playerCharacter = ui->tableWidget_listOfPlayers->item(playerID-1, 2)->text();
        mWindow_MainBoard->addPlayer(playerID, playerName, playerCharacter);
    }

    mWindow_MainBoard->showFullScreen();
}

void Window_StartMenu::on_pushButton_addPlayer_clicked()
{
    unsigned int playerID = ui->tableWidget_listOfPlayers->rowCount() + 1;
    QString playerName = ui->lineEdit_playerName->text();
    QString playerCharacter = ui->comboBox_playerCharacter->currentText();

    ui->tableWidget_listOfPlayers->setRowCount(playerID);

    ui->tableWidget_listOfPlayers->setItem(playerID-1, 0, new QTableWidgetItem(QString::number(playerID)));
    ui->tableWidget_listOfPlayers->setItem(playerID-1, 1, new QTableWidgetItem(playerName));
    ui->tableWidget_listOfPlayers->setItem(playerID-1, 2, new QTableWidgetItem(playerCharacter));

    ui->tableWidget_listOfPlayers->resizeColumnsToContents();

    ui->comboBox_playerCharacter->setCurrentIndex(0);
    ui->lineEdit_playerName->clear();

    startButtonControlCheck();
}

void Window_StartMenu::on_comboBox_playerCharacter_currentIndexChanged(const QString &arg1)
{
    if((arg1 == "Select Character") || (ui->lineEdit_playerName->text() == "")){
        ui->pushButton_addPlayer->setEnabled(false);
    }
    else {
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

    // Change the appearing character image
    if (arg1 == "Arythea")
    {
        ui->frameCharacter->setStyleSheet(mStyle_CharacterArythea);
    }
    else if(arg1 == "Norowas")
    {
        ui->frameCharacter->setStyleSheet(mStyle_CharacterNorowas);
    }
    else if(arg1 == "Tovak")
    {
        ui->frameCharacter->setStyleSheet(mStyle_CharacterTovak);
    }
    else if(arg1 == "Goldyx")
    {
        ui->frameCharacter->setStyleSheet(mStyle_CharacterGoldyx);
    }
    else if(arg1 == "Krang")
    {
        ui->frameCharacter->setStyleSheet(mStyle_CharacterKrang);
    }

    else
    {
        ui->frameCharacter->setStyleSheet(mStyle_CharacterClean);
    }
}

void Window_StartMenu::on_lineEdit_playerName_textChanged(const QString &arg1)
{
    if((arg1 == "") || (ui->comboBox_playerCharacter->currentText() == "Select Character")){
        ui->pushButton_addPlayer->setEnabled(false);
    }
    else {
        ui->pushButton_addPlayer->setEnabled(true);
    }
}


void Window_StartMenu::ui_newGameSetupClean()
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

    ui->spinBox_cityCount->setValue(0);
    ui->spinBox_dayCount->setValue(0);

    startButtonControlCheck();
}

void Window_StartMenu::startButtonControlCheck()
{
    // Check the setup status before enabling the "Start Game" Button
    if((ui->tableWidget_listOfPlayers->rowCount() == 0) || (ui->spinBox_dayCount->value() == 0))
    {
        ui->pushButton_startGame->setEnabled(false);
    }
    else
    {
        ui->pushButton_startGame->setEnabled(true);
    }
}

void Window_StartMenu::on_pushButton_loadGame_clicked()
{

}

void Window_StartMenu::on_spinBox_dayCount_valueChanged(int arg1)
{
    startButtonControlCheck();
}
