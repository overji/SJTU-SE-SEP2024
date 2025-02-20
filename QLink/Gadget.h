//
// Created by overji on 2024/6/28.
//

#pragma once
#include <QWidget>
#include "LinkGame.h"
#include "SaveSystem.h"

class LinkGame;
class Player;
class SaveSystem;

class Gadget: public QWidget
{
Q_OBJECT
public:
    // Getter and setter for leftTopX
    [[nodiscard]] int getLeftTopX() const { return leftTopX; }
    void setLeftTopX(int value) { leftTopX = value; }

    // Getter and setter for leftTopY
    [[nodiscard]] int getLeftTopY() const { return leftTopY; }
    void setLeftTopY(int value) { leftTopY = value; }

    // Getter and setter for gadgetWidth
    [[nodiscard]] int getGadgetWidth() const { return gadgetWidth; }
    void setGadgetWidth(int value) { gadgetWidth = value; }

    // Getter and setter for gadgetHeight
    [[nodiscard]] int getGadgetHeight() const { return gadgetHeight; }
    void setGadgetHeight(int value) { gadgetHeight = value; }

    // Getter and setter for gadgetMap
    [[nodiscard]] QPixmap getGadgetMap() const { return gadgetMap; }
    void setGadgetMap(QPixmap value) { gadgetMap = value; }
public:
    void drawGarget(QPainter & painter); //绘制道具
    Gadget(LinkGame * game, int gameType = 1); //构造函数
    Gadget(bool isLoad,LinkGame * game);
    int gargetType;// 1:plus1s 2:shuffle 3:hint 4:flash 5:freeze 6:dizzy
    void singleGameGadgetEffect(LinkGame * game, Player * player); //单人游戏的道具，效果从Player方面传入
    void doubleGameGadgetEffect(LinkGame * game, Player * anotherPlayer); //双人游戏的道具，效果从Player方面传入
private:
    void randomRowLoc(LinkGame * game,int colLoc); //随机生成道具的位置
    void plus1sEffect(LinkGame * game); //加30秒效果
    void shuffleEffect(LinkGame * game); //shuffle效果
    void hintEffect(LinkGame * game); //hint效果
    void flashEffect(LinkGame *game,Player *player);    //flash效果
    void freezeEffect(LinkGame *game,Player * player); //freeze效果(双人游戏)
    void dizzyEffect(LinkGame *game,Player * player); //dizzy效果(双人游戏)
    int leftTopX;
    int leftTopY;
    int gadgetWidth;
    int gadgetHeight;
    QPixmap gadgetMap;
};

