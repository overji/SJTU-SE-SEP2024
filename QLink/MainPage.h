//
// Created by overji on 2024/7/5.
//

#pragma once
#include <QWidget>
#include <QStackedLayout>
#include <QLabel>
#include <QPushButton>
#include <QScrollArea>
#include <QTextEdit>
#include "LinkGame.h"
#include "SavePage.h"

class MainPage :public QWidget
{
    Q_OBJECT
public:
    explicit MainPage(); //构造函数

public slots:
    void gameTypeClicked(); //按下开始游戏按钮时候，调出游戏选择界面
    void startGame(); //开始游戏
    void toMainPage(); //返回主界面
    void toLoadPage(); //进入加载界面
    void toStartPage(); //开始游戏
    void endGame(); //退出游戏
    void loadGame(); //加载游戏

private:
    QStackedLayout *stackMainLayout; //有一个stackedLayout，便于主界面和开始游戏界面的切换
    QWidget *mainWidget; //主界面
    QWidget *startWidget; //开始游戏界面
    LoadPage * loadWidget;
    void initMainPage(); //初始化主界面
    void initStartGamePage(); //初始化开始游戏界面
//    void initSettingPage(); //(TODO)
    void setBackGround(QString path); //设置背景图片
};

