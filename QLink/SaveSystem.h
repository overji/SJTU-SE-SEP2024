//
// Created by overji on 2024/7/3.
//
#pragma once
#include "LinkGame.h"
#include "Player.h"
#include "Gadget.h"
#include "BoxOfGame.h"

class LinkGame;
class Player;
class Gadget;
class BoxOfGame;


class SaveSystem {
public:
    static void saveGame(LinkGame *game,const QString &name);//保存游戏
    static LinkGame * loadGame(const QString &name);//加载游戏
    static LinkGame * loadGame();//快速读档
    static void saveGame(LinkGame *game);//快速存档
private:
    static void savePlayerData(QDataStream &out, Player *player);//保存玩家数据
    static void loadPlayerData(QDataStream &in, Player *player,LinkGame * game); //加载玩家数据
};


