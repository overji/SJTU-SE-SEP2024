//
// Created by overji on 2024/7/5.
//

#include "MainPage.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSpinBox>
#include <QWidget>
#include <QPushButton>
#include <QRadioButton>
#include <QLabel>
#include <QButtonGroup>
#include <iostream>
#include "SaveSystem.h"


MainPage::MainPage()
{
    //初始化主界面和开始游戏界面
    initMainPage();
    initStartGamePage();
    this->loadWidget = new LoadPage(this);
    //初始化stackedLayout，并且把主界面和开始游戏界面加入其中
    stackMainLayout = new QStackedLayout;
    stackMainLayout->addWidget(this->mainWidget);
    stackMainLayout->addWidget(this->startWidget);
    stackMainLayout->addWidget(this->loadWidget);
    stackMainLayout->setCurrentIndex(0); //默认显示主界面
    this->setLayout(stackMainLayout); //设置布局
    //设置窗口大小和图标
    resize(800,600);
    std::filesystem::path curFilePath(__FILE__);
    std::filesystem::path curPath = curFilePath.parent_path();
    setWindowIcon(QIcon(QString::fromStdString(curPath.string() +"\\images\\icon.png")));
    this->setBackGround(QString::fromStdString(curPath.string() + "\\images\\__sys_tm_bg01a.png"));
}

void MainPage::initMainPage()
{
    //初始化主界面
    this->mainWidget = new QWidget;
    QGridLayout * mainLayout = new QGridLayout(this);

    //设置按钮，并且把按钮和槽函数连接起来
    QPushButton *startGame = new QPushButton("开始游戏");
    connect(startGame,&QPushButton::clicked,this, &MainPage::toStartPage);
    QPushButton *exitGame = new QPushButton("退出游戏");
    connect(exitGame,&QPushButton::clicked,this,&MainPage::endGame);
    QPushButton * loadGame = new QPushButton("加载游戏");
    connect(loadGame,&QPushButton::clicked,this,&MainPage::toLoadPage);
    QVector<QPushButton *> buttons = {startGame,exitGame,loadGame};
    //设置按钮样式
    for(auto button:buttons){
        button->setStyleSheet("background-color:rgba(255,255,255,0);"
                              "font-size:20px;"
                              "border:1px solid white;"
                              "border-radius: 5px;");
    }
    //设置标题Label，并且设置样式
    QLabel *gameTitle = new QLabel("扣踢连连看");
    gameTitle->setStyleSheet("font-size:50px;"
                             "color: white;"
                             "text-decoration: dotted azure 2px;");
    gameTitle->setAlignment(Qt::AlignCenter);

    //把按钮和标题加入到布局中
    mainLayout->addWidget(new QLabel,0,0,2,16);
    mainLayout->addWidget(new QLabel,2,0,4,4);
    mainLayout->addWidget(gameTitle,2,4,4,8);
    mainLayout->addWidget(new QLabel,2,12,4,4);
    mainLayout->addWidget(new QLabel,6,0,4,16);
    mainLayout->addWidget(startGame,10,1,2,4);
    mainLayout->addWidget(loadGame,10,6,2,4);
    mainLayout->addWidget(exitGame,10,11,2,4);
    //设置布局
    this->mainWidget->setLayout(mainLayout);
}

void MainPage::setBackGround(QString path)
{
    //设置背景图片
    QPalette palette;
    QPixmap pixmap(QPixmap(path).scaled(this->size(),Qt::IgnoreAspectRatio,Qt::SmoothTransformation));
    palette.setBrush(QPalette::Window,QBrush(pixmap));
    this->setPalette(palette);
    setAutoFillBackground(true);
}

void MainPage::initStartGamePage()
{
    //初始化开始游戏界面
    this->startWidget = new QWidget;
    QGridLayout * startLayout = new QGridLayout;
    //设置Label和按钮，并且把按钮连接到槽函数上
    QLabel * MHint = new QLabel("几行方块(请输入2~16之间的整数)?");
    QLabel * NHint = new QLabel("几列方块(请输入2~16之间的整数)?");
    QLabel * oddHint = new QLabel("请确保行数和列数至少有一个为偶数!");
    QLabel * gameTypeHint = new QLabel("游戏模式: ");
    QLabel * title = new QLabel("游戏启动设置");
    QLabel * gameHint = new QLabel("W向上移动,S向下移动,A向左移动,D向右移动,Esc暂停游戏");
    QPushButton * startGameButton = new QPushButton("开始游戏");
    connect(startGameButton,&QPushButton::clicked,this,&MainPage::startGame);
    QPushButton * cancelButton = new QPushButton("取消");
    connect(cancelButton,&QPushButton::clicked,this, &MainPage::toMainPage);
    QRadioButton * singleGame = new QRadioButton("单人游戏");
    singleGame->setChecked(true); //默认选择单人游戏
    QRadioButton * multiGame = new QRadioButton("多人游戏");
    connect(singleGame,&QRadioButton::clicked,this,&MainPage::gameTypeClicked);
    connect(multiGame,&QRadioButton::clicked,this,&MainPage::gameTypeClicked);

    //设置数字输入框，并且设置范围和默认值
    QSpinBox * MInput = new QSpinBox();
    MInput->setRange(2,16);
    MInput->setValue(9);
    QSpinBox * NInput = new QSpinBox();
    NInput->setRange(2,16);
    NInput->setValue(12);

    //设置样式
    QVector<QLabel *> hints = {MHint,NHint,oddHint,gameTypeHint,title,gameHint};
    QVector<QSpinBox *> inputs = {MInput,NInput};
    QVector<QRadioButton *> gameTypes = {singleGame,multiGame};
    QVector<QPushButton *> buttons = {startGameButton,cancelButton};
    for(auto hint:hints){
        hint->setStyleSheet("font-size:20px;");
        hint->setAlignment(Qt::AlignCenter);
    }
    for(auto input:inputs){
        input->setStyleSheet("font-size:20px;");
        input->setAlignment(Qt::AlignCenter);
    }
    for(auto gameType:gameTypes){
        gameType->setStyleSheet("font-size:20px;");
    }
    for(auto button:buttons){
        button->setStyleSheet("background-color:rgba(255,255,255,0);"
                              "font-size:20px;"
                              "border:1px solid white;"
                              "border-radius: 5px;");
    }
    title->setStyleSheet("font-size:20px;");

    //把控件加入到布局中，注意是16列12行
    startLayout->addWidget(title,0,0,1,8);
    startLayout->addWidget(MHint,1,0,1,4);
    startLayout->addWidget(MInput,1,4,1,4);
    startLayout->addWidget(NHint,2,0,1,4);
    startLayout->addWidget(NInput,2,4,1,4);
    startLayout->addWidget(oddHint,3,0,1,8);
    startLayout->addWidget(gameTypeHint,4,0,1,4);
    startLayout->addWidget(singleGame,4,4,1,2);
    startLayout->addWidget(multiGame,4,6,1,2);
    startLayout->addWidget(gameHint,5,0,1,8);
    startLayout->addWidget(startGameButton,6,1,1,2);
    startLayout->addWidget(cancelButton,6,5,1,2);
    //设置布局
    this->startWidget->setLayout(startLayout);
}

void MainPage::gameTypeClicked()
{
    //根据游戏模式，设置提示信息
    QRadioButton * singleGame = qobject_cast<QRadioButton *>(qobject_cast<QGridLayout*>(startWidget->layout())->itemAtPosition(4, 4)->widget());
    QLabel * gameHint = qobject_cast<QLabel *>(qobject_cast<QGridLayout*>(startWidget->layout())->itemAtPosition(5,0)->widget());
    if(singleGame->isChecked()){
        //单人游戏提示
        gameHint->setText("W向上移动,S向下移动,A向左移动,D向右移动,Esc、空格暂停");
    } else {
        //多人游戏提示
        gameHint->setText("第二个玩家,↑向上移动,↓向下移动,←向左移动,→向右移动,Esc、空格暂停");
    }
}

void MainPage::toStartPage()
{
    //切换到开始游戏界面
    this->stackMainLayout->setCurrentIndex(1);
}

void MainPage::startGame()
{
    //开始游戏
    //获取输入的行数和列数
    QSpinBox * MInput = qobject_cast<QSpinBox *>(qobject_cast<QGridLayout*>(startWidget->layout())->itemAtPosition(1,4)->widget());
    QSpinBox * NInput = qobject_cast<QSpinBox *>(qobject_cast<QGridLayout*>(startWidget->layout())->itemAtPosition(2,4)->widget());
    QRadioButton * singleGame = qobject_cast<QRadioButton *>(qobject_cast<QGridLayout*>(startWidget->layout())->itemAtPosition(4, 4)->widget());
    int M = MInput->value();
    int N = NInput->value();
    if((M * N) % 2){
        std::cout << "Between M and N there must be an odd number! " << std::endl;
        return;
    }
    //根据游戏模式，选择不同的游戏
    int gameType = singleGame->isChecked()?0:1;
    LinkGame * game = new LinkGame(M,N,gameType);
    //显示游戏界面，关闭启动游戏窗口
    game->show();
    this->close();
}

void MainPage::toMainPage()
{
    //返回主界面
    this->stackMainLayout->setCurrentIndex(0);
}

void MainPage::toLoadPage()
{
    //进入加载界面
    this->stackMainLayout->setCurrentIndex(2);
}


void MainPage::endGame()
{
    //退出游戏
    std::exit(0);
}

void MainPage::loadGame()
{
    //加载游戏
    SaveSystem::loadGame()->show();
    this->close();
}


