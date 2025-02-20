//
// Created by overji on 2024/8/8.
//

#include "MainPage.h"
#include "SavePage.h"
#include <iostream>
#include <QWidget>
#include <QPalette>

void SavePage::initSavePage()
{
    //初始化存档界面，并且初始化各项控件
    // 创建一个QPalette对象
    QPalette palette;

    // 设置QPalette的背景颜色为白色
    palette.setColor(QPalette::Window, Qt::white);

    // 使用QPalette设置SavePage的背景颜色
    this->setAutoFillBackground(true);
    this->setPalette(palette);
    this->layout = new QGridLayout;
    this->Title = new QLabel("存档");
    this->Title->setStyleSheet("font-size:50px;");
    this->Title->setAlignment(Qt::AlignCenter);
    this->scrollArea = new QScrollArea;
    this->newSave = new QPushButton("新建存档");
    this->returnButton = new QPushButton("返回");
    this->quickSave = new QPushButton("快速存档");
    this->quickLoad = new QPushButton("快速读档");
    this->verticalLayout = new QVBoxLayout;
    this->scrollArea->setLayout(this->verticalLayout);
    this->layout->addWidget(this->Title,0,0,1,16);
    this->layout->addWidget(this->scrollArea,1,0,8,12);
    this->layout->addWidget(this->returnButton,1,12,2,4);
    this->layout->addWidget(this->newSave,3,12,2,4);
    this->layout->addWidget(this->quickSave,5,12,2,4);
    this->layout->addWidget(this->quickLoad,7,12,2,4);
    initSaveScrollArea();
    connect(this->returnButton,&QPushButton::clicked,this,&SavePage::returnOriginPage);
    connect(this->newSave,&QPushButton::clicked,this,&SavePage::saveGame);
    connect(this->quickSave,&QPushButton::clicked,[=](){
        SaveSystem::saveGame(game);
    });
    connect(this->quickLoad,&QPushButton::clicked,[=](){
        std::filesystem::path savePath = std::filesystem::current_path() / "save/quickSave.sav";
        if(!std::filesystem::exists(savePath)){
            std::cout << "quickSave does not exist!" << std::endl;
            return;
        }
        SaveSystem::loadGame()->show();
        this->game->close();
    });
    this->setCSS();
    this->setLayout(this->layout);
}

void SavePage::initSaveScrollArea() const
{
    //初始化存档滚动区域
    int saveNum = 0;
    std::filesystem::path savePath = std::filesystem::current_path() / "save";
    if(!std::filesystem::exists(savePath)){
        std::filesystem::create_directory(savePath);
    }
    for(auto &p:std::filesystem::directory_iterator(savePath)){
        if(p.path().extension() == ".sav" && p.path().stem() != "quickSave"){
            SaveDataWidget * saveDataWidget = new SaveDataWidget(QString::fromStdString(p.path().string()),QString::fromStdString(p.path().stem().string()));
            connect(saveDataWidget->deleteButton,&QPushButton::clicked,[=](){
                std::filesystem::remove(p.path());
                saveDataWidget->deleteLater();
            });
            this->verticalLayout->addWidget(saveDataWidget);
            saveNum ++;
        }
    }
}

void SavePage::saveGame()
{
    //保存游戏
    int saveNum = 0;
    std::filesystem::path savePath = std::filesystem::current_path() / "save";
    if(!std::filesystem::exists(savePath)){
        std::filesystem::create_directory(savePath);
    }
    for(auto &p:std::filesystem::directory_iterator(savePath)){
        if(p.path().extension() == ".sav"){
            saveNum++;
        }
    }
    QDateTime currentTime = QDateTime::currentDateTime();
    SaveSystem::saveGame(game,QString::number(saveNum) + QString('_') + currentTime.toString("yyyyMMddhhmmss"));
    qDebug() << QString::number(saveNum) + QString('_') + currentTime.toString("yyyyMMddhhmmss");
    auto * saveDataWidget = new SaveDataWidget(QString::fromStdString((savePath / (QString::number(saveNum) + QString('_') + currentTime.toString("yyyyMMddhhmmss") + ".sav").toStdString()).string()),QString::number(saveNum) + QString('_') + currentTime.toString("yyyyMMddhhmmss"));
    connect(saveDataWidget->deleteButton,&QPushButton::clicked,[=](){
        std::string saveString = (QString::number(saveNum) + QString('_') + currentTime.toString("yyyyMMddhhmmss")).toStdString() + ".sav";
        std::filesystem::path savePath(saveString);
        std::filesystem::remove(savePath);
        saveDataWidget->deleteLater();
    });
    this->verticalLayout->addWidget(saveDataWidget);
}

SavePage::SavePage(LinkGame *game):QWidget(game)
{
    //构造函数
    this->game = game;
    initSavePage();
}

void SavePage::returnOriginPage()
{
    //返回主界面
    game->showPausePage();
}

SaveDataWidget::SaveDataWidget(QString pathInput, QString nameInput)
{
    //构造函数，构造saveDataWidget
    this->path = pathInput;
    this->name = nameInput;
    this->nameLabel = new QLabel;
    this->deleteButton = new QPushButton("D");
    this->setStyleSheet("font-size:15px;background-color:#FFFFFF;border-radius:5px;border:1px solid black;padding:1px 2px 1px 2px;height:75%");
    this->nameLabel->setText(nameInput);
    this->nameLabel->setStyleSheet("border-radius:2px;background-color:azure");
    this->deleteButton->setStyleSheet("border-radius:2px;background-color:azure");
    this->gridLayout = new QGridLayout;
    this->gridLayout->addWidget(this->nameLabel,0,0,1,3);
    this->gridLayout->addWidget(this->deleteButton,0,3,1,1);
    this->setLayout(this->gridLayout);
}

void SavePage::setCSS()
{
    //设置CSS
    this->setStyleSheet("font-size:15px;");
    QVector<QPushButton*>buttons = {this->newSave,this->returnButton,this->quickSave,this->quickLoad};
    for(auto i:buttons){
        i->setStyleSheet("font-size:25px;background-color:#73ff95;color:black;border-radius:5px;border:1px solid black;padding:1px 2px 1px 2px;height:75%");
    }
}

// LoadPage

void LoadPage::initLoadPage()
{
    //初始化读档界面
    // 创建一个QPalette对象
    QPalette palette;

    // 设置QPalette的背景颜色为白色
    palette.setColor(QPalette::Window, Qt::white);

    // 使用QPalette设置SavePage的背景颜色
    this->setAutoFillBackground(true);
    this->setPalette(palette);
    this->layout = new QGridLayout;
    this->Title = new QLabel("存档");
    this->Title->setStyleSheet("font-size:50px;");
    this->Title->setAlignment(Qt::AlignCenter);
    this->scrollArea = new QScrollArea;
    this->returnButton = new QPushButton("返回");
    this->quickLoad = new QPushButton("快速读档");
    this->verticalLayout = new QVBoxLayout;
    this->scrollArea->setLayout(this->verticalLayout);
    this->layout->addWidget(this->Title,0,0,1,16);
    this->layout->addWidget(this->scrollArea,1,0,8,12);
    this->layout->addWidget(this->returnButton,1,12,2,4);
    this->layout->addWidget(this->quickLoad,3,12,2,4);
    initSaveScrollArea();
    connect(this->returnButton,&QPushButton::clicked,this,&LoadPage::returnOriginPage);
    connect(this->quickLoad,&QPushButton::clicked,[=](){
        std::filesystem::path savePath = std::filesystem::current_path() / "save/quickSave.sav";
        if(!std::filesystem::exists(savePath)){
            std::cout << "quickSave doex not exist!" << std::endl;
            return;
        }
        SaveSystem::loadGame()->show();
        this->mainPage->close();
    });
    this->setCSS();
    this->setLayout(this->layout);
}

void LoadPage::initSaveScrollArea() const
{
    //初始化存档滚动区域
    int saveNum = 0;
    std::filesystem::path savePath = std::filesystem::current_path() / "save";
    if(!std::filesystem::exists(savePath)){
        std::filesystem::create_directory(savePath);
    }
    for(auto &p:std::filesystem::directory_iterator(savePath)){
        if(p.path().extension() == ".sav"  && p.path().stem() != "quickSave"){
            SaveDataWidget * saveDataWidget = new SaveDataWidget(QString::fromStdString(p.path().string()),QString::fromStdString(p.path().stem().string()));
            connect(saveDataWidget,&SaveDataWidget::clicked,[=](){
                SaveSystem::loadGame(saveDataWidget->name)->show();
                mainPage->close();
            });
            connect(saveDataWidget->deleteButton,&QPushButton::clicked,[=](){
                std::filesystem::remove(p.path());
                saveDataWidget->deleteLater();
            });
            this->verticalLayout->addWidget(saveDataWidget);
            saveNum ++;
        }
    }
}

void LoadPage::setCSS()
{
    //设置CSS
    this->setStyleSheet("font-size:15px;");
    QVector<QPushButton*>buttons = {this->returnButton,this->quickLoad};
    for(auto i:buttons){
        i->setStyleSheet("font-size:25px;background-color:#73ff95;color:black;border-radius:5px;border:1px solid black;padding:1px 2px 1px 2px;height:75%");
    }
}

LoadPage::LoadPage(MainPage * mainPage):QWidget(mainPage)
{
    //构造函数
    this->mainPage = mainPage;
    initLoadPage();
}

void LoadPage::returnOriginPage()
{
    //返回主界面
    mainPage->toMainPage();
}
