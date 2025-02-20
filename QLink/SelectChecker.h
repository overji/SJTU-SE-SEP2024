//
// Created by overji on 2024/6/24.
//
#pragma once
#include "LinkGame.h"
#include "Player.h"
class LinkGame;
class Player;

class SelectChecker
{
public:
    static void checkSelected(LinkGame * game, Player * player); // 主函数，检查是否可以链接两个箱子或选中的箱子是否合理对应时调用此函数
    static bool solutionExist(LinkGame * game,Player * player); // 检查是否有解
    static bool checkExist(LinkGame * game,Player * player,int x,int y);
    static bool checkValidReturnBool(LinkGame * game,Player * player);
    static QPair<QPair<int,int>,QPair<int,int>> searchPair(LinkGame * game); // 搜索一对可以消除的箱子
private:
    static void checkRemoveAvailable(LinkGame * game, Player * player);
    static QVector<QPair<int,int>> checkValid(LinkGame * game,Player * player);
    static bool checkLine(LinkGame * game,Player * player,int direction,int start,int end,int anotherLocation);
    static QVector<QPair<int,int>> twoTwistCheck(LinkGame * game,Player * player,int x1,int y1,int x2,int y2);
    static QVector<QPair<int,int>> getEdgeBox(LinkGame * game,Player * player);
    static bool isEdge(LinkGame * game, Player * player, int colLoc, int rowLoc);
    static void removeDoubleSelection(LinkGame * game, Player * player);
};

