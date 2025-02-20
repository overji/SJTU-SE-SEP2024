//
// Created by overji on 2024/7/3.
//

#include <QDir>
#include <iostream>
#include "SaveSystem.h"

class LinkGame;
class Player;
class Gadget;
class BoxOfGame;

void SaveSystem::saveGame(LinkGame * game,const QString &name)
{
    // 如果save文件夹不存在，创建save文件夹
    QDir dir;
    if(!dir.exists("save")){
        dir.mkdir("save");
    }
    // 保存游戏数据，如果gameData.sav文件不存在，创建gameData.sav文件
    QFile saveFile(QString("save/") + name + ".sav");
    saveFile.open(QIODevice::WriteOnly);
    QDataStream out(&saveFile);
    // 保存游戏数据
    out << game->getBoxRow() << game->getBoxCol();
    out << game->getGameFps() << game->getRemainBoxNumber() << game->getBoxWidth() << game->getBoxHeight() << game->getPassageWidth() << game->getPassageHeight();
    out << game->getXScaleRatio() << game->getYScaleRatio();
    out << game->isGameEnd() << game->isGamePause() << game->isNoSolution();

    out << game->getRemainTime() << game->getGameType();
//保存玩家数据
    savePlayerData(out,game->getPlayer1());
    if(game->getGameType() != 0){
        savePlayerData(out,game->getPlayer2());
    }
//保存游戏文本
    out << game->getRemoveText() << game->getLeftTimeText() << game->getSummaryText();
//保存道具
    out << game->getGadgets().size();
    for(auto i : game->getGadgets()){
        out << i->getGadgetWidth() <<  i->getGadgetHeight() << i->getLeftTopX() << i->getLeftTopY() << i->gargetType;
        out << i->getGadgetMap();
    }
    out << game->getMaxGadgetNumber() << game->getGadgetSummonPossibility();
//保存道具相关计时情况
    out << game->getHintTime() << game->isHintTimerOn();
    out << game->getHintedBoxes().size();
    for(auto i : game->getHintedBoxes()){
        out << i.first << i.second;
    }
    out << game->getFlashTime() << game->isFlashTimerOn();
    out << game->isDizzyTimerOn() << game->isFreezeTimerOn();

//保存所有箱子
    for(int row = 0;row < game->getBoxRow();row ++){
        for(int col = 0;col < game->getBoxCol();col ++){
            out << game->getBoxMap()[row][col]->getBoxState().boxRemoved
                << game->getBoxMap()[row][col]->getBoxState().boxSelected
                << game->getBoxMap()[row][col]->getBoxState().closeBox
                << game->getBoxMap()[row][col]->getBoxState().boxToBeRemoved
                << game->getBoxMap()[row][col]->getBoxState().boxHinted;
            out << game->getBoxMap()[row][col]->getLeftTopX()
                << game->getBoxMap()[row][col]->getLeftTopY()
                << game->getBoxMap()[row][col]->getTypeOfBox()
                << game->getBoxMap()[row][col]->getBoxColor()
                << game->getBoxMap()[row][col]->getBoarderColor();
            out << game->getBoxMap()[row][col]->getBoxPixmap();
        }
    }
    std::cout << "Save Game Success" << std::endl;
}

void SaveSystem::savePlayerData(QDataStream &out, Player *player)
{
    //保存玩家数据
    out << player->getPlayerLeftTopX() << player->getPlayerLeftTopY() << player->getPlayerWidth() << player->getPlayerHeight()
        << player->getPlayerSpeed() << player->getScore() << player->getScoreString();
    out << player->getFreezeTime() << player->getDizzyTime();
    out << player->getPlayerImage();
    out << player->getCurrentSelected().size();
    for(auto i : player->getCurrentSelected()){
        out << i.first << i.second;
    }

    out << player->getToBeRemovedBox().size();
    for(auto i : player->getToBeRemovedBox()){
        out << i.first << i.second;
    }
    out << player->getLinePath().size();
    for(auto i : player->getLinePath()){
        out << i.x() << i.y();
    }
    out << player->isRemoveTimerOn();
}



LinkGame * SaveSystem::loadGame(const QString &name)
{
    //加载游戏数据
    LinkGame * game = new LinkGame();
    QFile saveFile(QString("save/")+name+QString(".sav"));
    saveFile.open(QIODevice::ReadOnly);
    QDataStream in(&saveFile);
    //加载游戏数据
    int tempInt;
    double tempDouble;
    bool tempBool;
    QColor tempColor;
    QPixmap tempPixmap;
    QString tempString;

    in >> tempInt; game->setBoxRow(tempInt);
    in >> tempInt; game->setBoxCol(tempInt);
    in >> tempInt; game->setGameFps(tempInt);
    in >> tempInt; game->setRemainBoxNumber(tempInt);
    in >> tempInt; game->setBoxWidth(tempInt);
    in >> tempInt; game->setBoxHeight(tempInt);
    in >> tempInt; game->setPassageWidth(tempInt);
    in >> tempInt; game->setPassageHeight(tempInt);
    in >> tempDouble; game->setXScaleRatio(tempDouble);
    in >> tempDouble; game->setYScaleRatio(tempDouble);
    in >> tempBool; game->setGameEnd(tempBool);
    in >> tempBool; game->setGamePause(tempBool);
    in >> tempBool; game->setNoSolution(tempBool);

    in >> tempInt; game->setRemainTime(tempInt);
    in >> tempInt; game->setGameType(tempInt);

//加载玩家数据
    loadPlayerData(in,game->getPlayer1(),game);
    if(game->getGameType() != 0){
        Player *player2 = new Player(0,0,0,0,0,0);
        game->setPlayer2(player2);
        loadPlayerData(in,game->getPlayer2(),game);
    }

//加载游戏文本
    in >> tempString; game->setRemoveText(tempString);
    in >> tempString; game->setLeftTimeText(tempString);
    in >> tempString; game->setSummaryText(tempString);

    qsizetype gadgetSize;
    in >> gadgetSize;
    QVector<Gadget *> gadgets;
    for(int i = 0;i < gadgetSize;i ++){
        int gadgetWidth,gadgetHeight,leftTopX,leftTopY,tmpGargetType;
        in >> gadgetWidth >> gadgetHeight >> leftTopX >> leftTopY >> tmpGargetType;
        Gadget * gadget = new Gadget(true,game);
        gadget->setLeftTopX(leftTopX);
        gadget->setLeftTopY(leftTopY);
        gadget->setGadgetWidth(gadgetWidth);
        gadget->setGadgetHeight(gadgetHeight);
        gadget->gargetType = tmpGargetType;
        in >> tempPixmap;
        gadget->setGadgetMap(tempPixmap);
        gadgets.push_back(gadget);
    }
    game->setGadgets(gadgets);
    //加载道具相关数据
    in >> tempInt; game->setMaxGadgetNumber(tempInt);
    in >> tempDouble; game->setGadgetSummonPossibility(tempDouble);
    in >> tempInt; game->setHintTime(tempInt);
    in >> tempBool; game->setHintTimerOn(tempBool);
    //加载提示箱子
    qsizetype hintedBoxesSize;
    in >> hintedBoxesSize;
    QVector<QPair<int,int>> hintedBoxes;
    for(int i = 0;i < hintedBoxesSize;i ++){
        int x,y;
        in >> x >> y;
        hintedBoxes.push_back(QPair<int,int>(x,y));
    }
    game->setHintedBoxes(hintedBoxes);
    //加载道具计时器
    in >> tempInt; game->setFlashTime(tempInt);
    in >> tempBool; game->setFlashTimerOn(tempBool);
    in >> tempBool; game->setDizzyTimerOn(tempBool);
    in >> tempBool; game->setFreezeTimerOn(tempBool);

//所有箱子初始化
    game->initGlobalBox(game->getBoxRow(),game->getBoxCol());
    game->initMap();
    for(int row = 0;row < game->getBoxRow();row ++){
        for(int col = 0;col < game->getBoxCol();col ++){
            BoxState tempBoxState;
            in >> tempBoxState.boxRemoved >> tempBoxState.boxSelected >> tempBoxState.closeBox
               >> tempBoxState.boxToBeRemoved >> tempBoxState.boxHinted;
           std::cout << game->getBoxMap()[row].size() << std::endl;
            std::cout << (game->getBoxMap()[row])[col]->getBoxState().closeBox<< std::endl;
            game->getBoxMap()[row][col]->setBoxRemoved(tempBoxState.boxRemoved);
            game->getBoxMap()[row][col]->setBoxSelected(tempBoxState.boxSelected);
            game->getBoxMap()[row][col]->setBoxClose(tempBoxState.closeBox);
            game->getBoxMap()[row][col]->setBoxToBeRemoved(tempBoxState.boxToBeRemoved);
            game->getBoxMap()[row][col]->setBoxHinted(tempBoxState.boxHinted);

            in >> tempInt; game->getBoxMap()[row][col]->setLeftTopX(tempInt);
            in >> tempInt; game->getBoxMap()[row][col]->setLeftTopY(tempInt);
            in >> tempInt; game->getBoxMap()[row][col]->setTypeOfBox(tempInt);
            in >> tempColor; game->getBoxMap()[row][col]->setBoxColor(tempColor);
            in >> tempColor; game->getBoxMap()[row][col]->setBoarderColor(tempColor);

            in >> tempPixmap; game->getBoxMap()[row][col]->setBoxPixmap(tempPixmap);
        }
    }

    in >> tempBool; game->setGamePause(tempBool);
    return game;
}

void SaveSystem::loadPlayerData(QDataStream &in, Player *player,LinkGame * game) {
    //加载玩家数据
    int tempInt;
    QString tempString;
    QPixmap tempPixmap;

    in >> tempInt; player->setPlayerLeftTopX(tempInt);
    in >> tempInt; player->setPlayerLeftTopY(tempInt);
    in >> tempInt; player->setPlayerWidth(tempInt);
    in >> tempInt; player->setPlayerHeight(tempInt);
    in >> tempInt; player->setPlayerSpeed(tempInt);
    in >> tempInt; player->setScore(tempInt);
    in >> tempString; player->setScoreString(tempString);
    in >> tempInt; player->setFreezeTime(tempInt);
    in >> tempInt; player->setDizzyTime(tempInt);
    in >> tempPixmap; player->setPlayerImage(tempPixmap);

    qsizetype currentSelectedSize;
    in >> currentSelectedSize;
    QVector<QPair<int,int>> currentSelected;
    for (int i = 0; i < currentSelectedSize; i++) {
        int x, y;
        in >> x >> y;
        currentSelected.push_back(QPair<int, int>(x, y));
    }
    player->setCurrentSelected(currentSelected);

//加载待消除箱子
    qsizetype toBeRemovedBoxSize;
    in >> toBeRemovedBoxSize;
    QVector<QPair<int,int>> toBeRemovedBox;
    for(int i = 0;i < toBeRemovedBoxSize;i ++){
        int x,y;
        in >> x >> y;
        toBeRemovedBox.push_back(QPair<int,int>(x,y));
    }
    player->setToBeRemovedBox(toBeRemovedBox);

    qsizetype linePathSize;
    in >> linePathSize;
    QVector<QPoint> linePath;
    for(int i = 0;i < linePathSize;i ++){
        int x,y;
        in >> x >> y;
        linePath.push_back(QPoint(x,y));
    }
    player->setLinePath(linePath);

//加载道具
    bool tempBool;
    in >> tempBool; player->setRemoveTimerOn(tempBool);
    player->setGame(game);
}

void SaveSystem::saveGame(LinkGame * game)
{
    //保存游戏数据
    saveGame(game,QString("quickSave"));
}

LinkGame * SaveSystem::loadGame()
{
    //加载游戏数据
    return loadGame(QString("quickSave"));
}