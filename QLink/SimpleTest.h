//
// Created by overji on 2024/6/24.
//

#pragma once
#include <QtTest/QtTest>

class SimpleTest :public QObject
{
    Q_OBJECT
private slots:
    void noTwistHorizontalCorrectTest(); //无折线横向无阻挡，应当正确消除方块
    void noTwistVerticalCorrectTest(); //无折线纵向无阻挡，应当正确消除方块
    void noTwistHorizontalWrongTest(); //无折线横向有阻挡，应当不消除方块
    void noTwistVerticalWrongTest();//无折线纵向有阻挡，应当不消除方块
    void oneTwistCorrectTest();//一折线正确消除方块(左下)
    void oneTwistCorrectTest2();//一折线正确消除方块(右上)
    void oneTwistWrongTest();//一折线应当不消除方块
    void oneTwistWithOneLineTest();//一折线，一条线较长，应当正确消除方块
    void oneTwistWithOneLineBlockedTest();//一折线，一条线较长且存在阻挡，应当不消除方块
    void twoTwistCorrectTest();//两折线正确消除方块(外侧)
    void twoTwistCorrectTest2(); //两折线正确消除方块(内侧)
    void threeTwistWrongTest1(); //三折线应当不消除方块
    void threeTwistWrongTest2(); //多折线应当不消除方块
    void differentTypeWrongTest();//不同类型的方块应当不消除
    void popSameBoxTest();//相同方块只选择一次
    void removeSelectedTest(); //测试取消选中无法消除的方块
    void reversedBoxSequenceTest(); //从另一个先后顺序消除方块
};

