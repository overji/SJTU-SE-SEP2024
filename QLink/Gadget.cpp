//
// Created by overji on 2024/6/28.
//

#include "Gadget.h"
#include "LinkGame.h"
#include "SelectChecker.h"
#include "SpecialAlgorithm.h"
#include <QWidget>
#include <QPainter>
#include <iostream>

Gadget::Gadget(bool isLoad,LinkGame *game)
{
    //加载时候，不对道具各项参数进行赋值，从而防止了由于箱子尚未完全确定，随机确定箱子y坐标时访问到尚未初始化的区域。
    this->gadgetHeight = this->gargetType = this->gadgetWidth = this->leftTopX = this->leftTopY = 0;
    this->gadgetMap = QPixmap();
}

Gadget::Gadget(LinkGame * game, int gameType)
{
    //随机生成道具的位置,并且随机生成道具的类型,game:游戏指针，gameType:游戏类型
    this->gadgetWidth = game->getBoxWidth() / 3; //道具的宽度
    this->gadgetHeight = game->getBoxHeight() / 3; //道具的高度
    this->leftTopX = QRandomGenerator::global()->bounded(1, 800 - gadgetWidth - 1); //道具的左上角x坐标
    int colLoc = specialDiv(leftTopX-game->getPassageWidth(),game->getBoxWidth()); //道具所在的列
    randomRowLoc(game,colLoc); //随机生成道具的行数
    if(gameType == 0){
        this->gargetType = QRandomGenerator::global()->bounded(1,5); //单人游戏下的道具类型，从1到4
    } else {
        this->gargetType = QRandomGenerator::global()->bounded(1,6); //双人游戏下的道具类型，从1到5，flash不可出现，所以遇到4变为6
        if(this->gargetType == 4){
            this->gargetType = 6;
        }
    }
    std::filesystem::path curFilePath(__FILE__);//确定路径
    std::filesystem::path curPath = curFilePath.parent_path();
    switch(gargetType){
        //确定道具的贴图
        case 1:
            this->gadgetMap = QPixmap(QString::fromStdString(curPath.string() + "\\images\\GargetImages\\plus1s.png"));
            break;
        case 2:
            this->gadgetMap = QPixmap(QString::fromStdString(curPath.string() + "\\images\\GargetImages\\shuffle.png"));
            break;
        case 3:
            this->gadgetMap = QPixmap(QString::fromStdString(curPath.string() + "\\images\\GargetImages\\hint.png"));
            break;
        case 4:
            this->gadgetMap = QPixmap(QString::fromStdString(curPath.string() + "\\images\\GargetImages\\Flash.png"));
            break;
        case 5:
            this->gadgetMap = QPixmap(QString::fromStdString(curPath.string() + "\\images\\GargetImages\\freeze.png"));
            break;
        case 6:
            this->gadgetMap = QPixmap(QString::fromStdString(curPath.string() + "\\images\\GargetImages\\dizzy.png"));
            break;
    }
}

void Gadget::randomRowLoc(LinkGame *game, int colLoc)
{
    //随机生成道具的行数，加载游戏时候不允许调用这个函数，防止内存越界,game:游戏指针，colLoc:道具所在的列
    QVector<QPair<int,int>>spaceRemain; //记录每一行的空白区域
    int start = 0, end = 0,totalSpace = 0; //记录空白区域的起始和终止位置以及一列中空白区域的大小
    if(colLoc < game->getBoxCol() && colLoc >= 0){
        //如果道具在可能生成箱子的列中
        spaceRemain.push_back(QPair<int,int>(0, game->getPassageHeight() - gadgetHeight)); //上部过道中的空白区域
        totalSpace += (game->getPassageHeight() - gadgetHeight); //上部过道中的空白区域大小
        start = game->getPassageHeight(); //初始化start
        for(int row = 0;row < game->getBoxRow();row ++){
            //遍历箱子的行
            if(!game->getBoxMap()[row][colLoc]->getBoxState().boxRemoved){
                //如果箱子没有被消除，更新start
                start = game->getBoxMap()[row][colLoc]->getLeftTopY() + game->getBoxHeight();
                continue;
            } else {
                //如果有箱子被消除，则将空白区域的起始点和终止点加入spaceRemain，并且更新start、end和空白区域大小
                end = game->getBoxMap()[row][colLoc]->getLeftTopY() + game->getBoxHeight();
                spaceRemain.push_back(QPair<int,int>(start, end - gadgetHeight));
                totalSpace += (end - start - gadgetHeight);
                start = end;
            }
        }
        spaceRemain.push_back(QPair<int,int>(start, 600 - gadgetHeight));//下部过道中的空白区域
        totalSpace += (600 - gadgetHeight - start - gadgetHeight);
    } else {
        //如果道具在左侧过道列或者右侧过道列，那么空白区域为整个列
        spaceRemain.push_back(QPair<int,int>(0, 600 - gadgetHeight));
        totalSpace = 600 - gadgetHeight;
    }
    this->leftTopY = QRandomGenerator::global()->bounded(1,totalSpace-1); //随机生成道具的行数
    for(auto i:spaceRemain){
        //根据随机生成的行数，确定道具的行数
        this->leftTopY -= (i.second-i.first); //减去空白区域的大小
        if(this->leftTopY <= 0){
            //当leftTopY小于等于0时，说明道具在这个空白区域中，位置为终止位置+现在的leftTopY
            this->leftTopY = this->leftTopY + i.second;
            break;
        }
    }
}

void Gadget::drawGarget(QPainter &painter)
{
    //绘制道具
    painter.drawPixmap(leftTopX, leftTopY, gadgetWidth, gadgetHeight, gadgetMap);
}

void Gadget::singleGameGadgetEffect(LinkGame *game, Player *player)
{
    switch(gargetType){
        //根据道具类型，确定道具效果
        case 1:
            plus1sEffect(game);
            break;
        case 2:
            shuffleEffect(game);
            break;
        case 3:
            hintEffect(game);
            break;
        case 4:
            flashEffect(game,player);
            break;
    }
}

void Gadget::doubleGameGadgetEffect(LinkGame *game, Player *anotherPlayer)
{
    switch(gargetType){
        //根据道具类型，确定道具效果
        case 1:
            plus1sEffect(game);
            break;
        case 2:
            shuffleEffect(game);
            break;
        case 3:
            hintEffect(game);
            break;
        case 5:
            freezeEffect(game,anotherPlayer);
            break;
        case 6:
            dizzyEffect(game,anotherPlayer);
            break;
    }
}

void Gadget::plus1sEffect(LinkGame *game)
{
    //加1秒效果
    game->setRemainTime(game->getRemainTime() + 1);
}

void Gadget::shuffleEffect(LinkGame *game)
{
    //shuffle效果
    game->shuffleMap();
}

void Gadget::hintEffect(LinkGame *game)
{
    //hint效果，并且hintTimer倒计时开始，注意多次触发时间不会叠加
    game->setHintTime(10);
    game->setHintTimerOn(true);
}

void Gadget::flashEffect(LinkGame *game,Player *player)
{
    //flash效果,并且flashTimer倒计时开始，注意多次触发时间不会叠加
    game->setFlashTime(5);
    game->setFlashTimerOn(true);
    return;
}

void Gadget::freezeEffect(LinkGame *game,Player *player)
{
    //freeze效果,并且freezeTimer倒计时开始，注意多次触发时间不会叠加
    player->setFreezeTime(3);
    game->setFreezeTimerOn(true);
}

void Gadget::dizzyEffect(LinkGame *game,Player *player)
{
    //dizzy效果,并且dizzyTimer倒计时开始，注意多次触发时间不会叠加
    player->setDizzyTime(10);
    game->setDizzyTimerOn(true);
}

