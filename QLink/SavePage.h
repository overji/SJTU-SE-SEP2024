//
// Created by overji on 2024/8/8.
//

#pragma once
#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QScrollArea>
#include <QTextEdit>
#include "MainPage.h"

class MainPage;

class SaveDataWidget:public QPushButton
{
    Q_OBJECT
public:
    explicit SaveDataWidget(QString pathInput, QString nameInput);//构造函数
    QString path;//存档路径
    QString name;//存档名字
    QGridLayout * gridLayout;//布局
    QLabel * nameLabel;//存档名字
    QPushButton * deleteButton;//删除按钮
};

class SavePage :public QWidget{
    Q_OBJECT
    QGridLayout * layout;//布局
    QLabel * Title;//标题
    QScrollArea * scrollArea;//滚动区域
    QPushButton * newSave;//新存档按钮
    QPushButton * returnButton;//返回按钮
    QPushButton * quickSave;//快速存档按钮
    QPushButton * quickLoad;//快速读档按钮
    LinkGame * game;//游戏
    QVBoxLayout * verticalLayout;//垂直布局
public:
    SavePage(LinkGame * game);//构造函数
    void initSavePage();//初始化存档界面
    void initSaveScrollArea() const;//初始化存档滚动区域
private:
    void setCSS();//设置CSS
public slots:
    void saveGame(); //存档
    void returnOriginPage();//返回原界面
};

class LoadPage :public QWidget{
Q_OBJECT
    QGridLayout * layout;//布局
    QLabel * Title;//标题
    QScrollArea * scrollArea;//滚动区域
    QPushButton * returnButton;//返回按钮
    QPushButton * quickLoad;//快速读档按钮
    QVBoxLayout * verticalLayout;//垂直布局
    MainPage * mainPage;//主界面
public:
    LoadPage(MainPage * mainPage);//构造函数
    void initLoadPage();//初始化读档界面
    void initSaveScrollArea() const;//初始化存档滚动区域
private:
    void setCSS();//设置CSS
public slots:
    void returnOriginPage();//返回原界面
};

