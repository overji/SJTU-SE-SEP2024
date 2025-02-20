//
// Created by overji on 2024/6/24.
//

#include "SimpleTest.h"
#include "Player.h"
#include "SelectChecker.h"
#include <QtTest/QtTest>
#include <QPainter>

void SimpleTest::noTwistHorizontalCorrectTest()
{
    //测试无折线横向无阻挡，应当正确消除方块
    LinkGame * linkGame = new LinkGame(6,5);
    Player * player = new Player(0,0,0,0,0,linkGame,1);
    linkGame->getBoxMap()[0][0]->setTypeOfBox(1);;
    linkGame->getBoxMap()[0][1]->setTypeOfBox(1);;
    player->appendToCurrentSelected(QPair<int,int>(0,0));
    player->appendToCurrentSelected(QPair<int,int>(0,1));
    SelectChecker::checkSelected(linkGame,player);
    QPainter painter;
    player->drawLine(painter);
    QVERIFY(player->getCurrentSelected().size() == 0);
    QVERIFY(linkGame->getBoxMap()[0][0]->getBoxState().boxToBeRemoved == true);
    QVERIFY(linkGame->getBoxMap()[0][1]->getBoxState().boxToBeRemoved == true);
    delete linkGame;
    delete player;
}

void SimpleTest::noTwistVerticalCorrectTest()
{
    //测试无折线纵向无阻挡，应当正确消除方块
    LinkGame * linkGame = new LinkGame(4,5);
    Player * player = new Player(0,0,0,0,0,linkGame);
    linkGame->getBoxMap()[0][0]->setTypeOfBox(1);;
    linkGame->getBoxMap()[1][0]->setTypeOfBox(1);;
    player->appendToCurrentSelected(QPair<int,int>(0,0));
    player->appendToCurrentSelected(QPair<int,int>(1,0));
    QVERIFY(player->getCurrentSelected().size() == 2);
    SelectChecker::checkSelected(linkGame,player);
    QPainter painter;
    player->drawLine(painter);
    QVERIFY(player->getCurrentSelected().size() == 0);
    QVERIFY(linkGame->getBoxMap()[0][0]->getBoxState().boxToBeRemoved == true);
    QVERIFY(linkGame->getBoxMap()[1][0]->getBoxState().boxToBeRemoved == true);
    delete linkGame;
    delete player;
}

void SimpleTest::noTwistHorizontalWrongTest()
{
    //测试无折线横向有阻挡，应当不消除方块
    LinkGame * linkGame = new LinkGame(4,5);
    Player * player = new Player(0,0,0,0,0,linkGame);
    linkGame->getBoxMap()[1][1]->setTypeOfBox(1);;
    linkGame->getBoxMap()[1][3]->setTypeOfBox(1);;
    player->appendToCurrentSelected(QPair<int,int>(1,1));
    player->appendToCurrentSelected(QPair<int,int>(1,3));
    SelectChecker::checkSelected(linkGame, player);
    QPainter painter;
    player->drawLine(painter);
    QVERIFY(player->getCurrentSelected().size() == 0);
    QVERIFY(linkGame->getBoxMap()[1][1]->getBoxState().boxToBeRemoved == false);
    QVERIFY(linkGame->getBoxMap()[1][3]->getBoxState().boxToBeRemoved == false);
    delete linkGame;
    delete player;
}

void SimpleTest::noTwistVerticalWrongTest()
{
    //测试无折线纵向有阻挡，应当不消除方块
    LinkGame * linkGame = new LinkGame(4,5);
    Player * player = new Player(0,0,0,0,0,linkGame);
    linkGame->getBoxMap()[1][1]->setTypeOfBox(1);;
    linkGame->getBoxMap()[3][1]->setTypeOfBox(1);;
    player->appendToCurrentSelected(QPair<int,int>(1,1));
    player->appendToCurrentSelected(QPair<int,int>(3,1));
    SelectChecker::checkSelected(linkGame, player);
    QPainter painter;
    player->drawLine(painter);
    QVERIFY(player->getCurrentSelected().size() == 0);
    QVERIFY(linkGame->getBoxMap()[1][1]->getBoxState().boxToBeRemoved == false);
    QVERIFY(linkGame->getBoxMap()[3][1]->getBoxState().boxToBeRemoved == false);
    delete linkGame;
    delete player;
}

void SimpleTest::oneTwistCorrectTest()
{
    //测试一折线正确消除方块(左下)
    LinkGame * linkGame = new LinkGame(4,5);
    Player * player = new Player(0,0,0,0,0,linkGame);
    linkGame->getBoxMap()[1][1]->setTypeOfBox(1);;
    linkGame->getBoxMap()[2][2]->setTypeOfBox(1);;
    linkGame->getBoxMap()[2][1]->setBoxRemoved(true);
    player->appendToCurrentSelected(QPair<int,int>(1,1));
    player->appendToCurrentSelected(QPair<int,int>( 2,2));
    SelectChecker::checkSelected(linkGame, player);
    QPainter painter;
    player->drawLine(painter);
    QVERIFY(player->getCurrentSelected().size() == 0);
    QVERIFY(linkGame->getBoxMap()[1][1]->getBoxState().boxToBeRemoved == true);
    QVERIFY(linkGame->getBoxMap()[2][2]->getBoxState().boxToBeRemoved == true);
    delete linkGame;
    delete player;
}

void SimpleTest::oneTwistCorrectTest2()
{
    //测试一折线正确消除方块(右上)
    LinkGame * linkGame = new LinkGame(4,5);
    Player * player = new Player(0,0,0,0,0,linkGame);
    linkGame->getBoxMap()[1][1]->setTypeOfBox(1);;
    linkGame->getBoxMap()[2][2]->setTypeOfBox(1);;
    linkGame->getBoxMap()[1][2]->setBoxRemoved(true);
    player->appendToCurrentSelected(QPair<int,int>(1,1));
    player->appendToCurrentSelected(QPair<int,int>( 2,2));
    SelectChecker::checkSelected(linkGame, player);
    QPainter painter;
    player->drawLine(painter);
    QVERIFY(player->getCurrentSelected().size() == 0);
    QVERIFY(linkGame->getBoxMap()[1][1]->getBoxState().boxToBeRemoved == true);
    QVERIFY(linkGame->getBoxMap()[2][2]->getBoxState().boxToBeRemoved == true);
    delete linkGame;
    delete player;
}

void SimpleTest::oneTwistWrongTest()
{
    //测试一折线应当不消除方块
    LinkGame * linkGame = new LinkGame(4,5);
    Player * player = new Player(0,0,0,0,0,linkGame);
    linkGame->getBoxMap()[1][1]->setTypeOfBox(1);;
    linkGame->getBoxMap()[2][2]->setTypeOfBox(1);;
    player->appendToCurrentSelected(QPair<int,int>(1,1));
    player->appendToCurrentSelected(QPair<int,int>( 2,2));
    SelectChecker::checkSelected(linkGame, player);
    QPainter painter;
    player->drawLine(painter);
    QVERIFY(player->getCurrentSelected().size() == 0);
    QVERIFY(linkGame->getBoxMap()[1][1]->getBoxState().boxToBeRemoved == false);
    QVERIFY(linkGame->getBoxMap()[2][2]->getBoxState().boxToBeRemoved == false);
    delete linkGame;
    delete player;
}

void SimpleTest::oneTwistWithOneLineTest()
{
    //测试一折线，一条线较长，应当正确消除方块
    LinkGame * linkGame = new LinkGame(4,5);
    Player * player = new Player(0,0,0,0,0,linkGame);
    linkGame->getBoxMap()[1][1]->setTypeOfBox(1);;
    linkGame->getBoxMap()[2][3]->setTypeOfBox(1);;
    linkGame->getBoxMap()[2][1]->setBoxRemoved(true);
    linkGame->getBoxMap()[2][2]->setBoxRemoved(true);
    player->appendToCurrentSelected(QPair<int,int>(1,1));
    player->appendToCurrentSelected(QPair<int,int>( 2,3));
    SelectChecker::checkSelected(linkGame, player);
    QPainter painter;
    player->drawLine(painter);
    QVERIFY(player->getCurrentSelected().size() == 0);
    QVERIFY(linkGame->getBoxMap()[1][1]->getBoxState().boxToBeRemoved == true);
    QVERIFY(linkGame->getBoxMap()[2][3]->getBoxState().boxToBeRemoved == true);
    delete linkGame;
    delete player;
}

void SimpleTest::oneTwistWithOneLineBlockedTest()
{
    //测试一折线，一条线较长且存在阻挡，应当不消除方块
    LinkGame * linkGame = new LinkGame(4,5);
    Player * player = new Player(0,0,0,0,0,linkGame);
    linkGame->getBoxMap()[1][1]->setTypeOfBox(1);;
    linkGame->getBoxMap()[2][3]->setTypeOfBox(1);;
    linkGame->getBoxMap()[2][1]->setBoxRemoved(true);
    player->appendToCurrentSelected(QPair<int,int>(1,1));
    player->appendToCurrentSelected(QPair<int,int>( 2,3));
    SelectChecker::checkSelected(linkGame, player);
    QPainter painter;
    player->drawLine(painter);
    QVERIFY(player->getCurrentSelected().size() == 0);
    QVERIFY(linkGame->getBoxMap()[1][1]->getBoxState().boxToBeRemoved == false);
    QVERIFY(linkGame->getBoxMap()[2][3]->getBoxState().boxToBeRemoved == false);
    delete linkGame;
    delete player;
}

void SimpleTest::twoTwistCorrectTest()
{
    //测试两折线正确消除方块(外侧)
    LinkGame * linkGame = new LinkGame(4,5);
    Player * player = new Player(0,0,0,0,0,linkGame);
    linkGame->getBoxMap()[0][0]->setTypeOfBox(1);;
    linkGame->getBoxMap()[0][2]->setTypeOfBox(1);;
    player->appendToCurrentSelected(QPair<int,int>(0,0));
    player->appendToCurrentSelected(QPair<int,int>(0,2));
    SelectChecker::checkSelected(linkGame, player);
    QPainter painter;
    player->drawLine(painter);
    QVERIFY(player->getCurrentSelected().size() == 0);
    QVERIFY(linkGame->getBoxMap()[0][0]->getBoxState().boxToBeRemoved == true);
    QVERIFY(linkGame->getBoxMap()[0][2]->getBoxState().boxToBeRemoved == true);
    delete linkGame;
    delete player;
}

void SimpleTest::twoTwistCorrectTest2()
{
    //测试两折线正确消除方块(内侧)
    LinkGame * linkGame = new LinkGame(6,5);
    Player * player = new Player(0,0,0,0,0,linkGame);
    linkGame->getBoxMap()[1][1]->setTypeOfBox(1);;
    linkGame->getBoxMap()[4][3]->setTypeOfBox(1);;
    linkGame->getBoxMap()[2][1]->setBoxRemoved(true);
    linkGame->getBoxMap()[2][2]->setBoxRemoved(true);
    linkGame->getBoxMap()[2][3]->setBoxRemoved(true);
    linkGame->getBoxMap()[3][3]->setBoxRemoved(true);
    player->appendToCurrentSelected(QPair<int,int>(1,1));
    player->appendToCurrentSelected(QPair<int,int>(4,3));
    SelectChecker::checkSelected(linkGame, player);
    QPainter painter;
    player->drawLine(painter);
    QVERIFY(player->getCurrentSelected().size() == 0);
    QVERIFY(linkGame->getBoxMap()[1][1]->getBoxState().boxToBeRemoved == true);
    QVERIFY(linkGame->getBoxMap()[4][3]->getBoxState().boxToBeRemoved == true);
    delete linkGame;
    delete player;
}

void SimpleTest::threeTwistWrongTest1()
{
    //测试三折线应当不消除方块
    LinkGame * linkGame = new LinkGame(4,5);
    Player * player = new Player(0,0,0,0,0,linkGame);
    linkGame->getBoxMap()[1][0]->setTypeOfBox(1);;
    linkGame->getBoxMap()[0][4]->setTypeOfBox(1);;
    player->appendToCurrentSelected(QPair<int,int>(1,0));
    player->appendToCurrentSelected(QPair<int,int>(0,4));
    SelectChecker::checkSelected(linkGame, player);
    QPainter painter;
    player->drawLine(painter);
    QVERIFY(player->getCurrentSelected().size() == 0);
    QVERIFY(linkGame->getBoxMap()[1][0]->getBoxState().boxToBeRemoved == false);
    QVERIFY(linkGame->getBoxMap()[0][4]->getBoxState().boxToBeRemoved == false);
    delete linkGame;
    delete player;
}

void SimpleTest::threeTwistWrongTest2()
{
    //测试多折线应当不消除方块
    LinkGame * linkGame = new LinkGame(4,5);
    Player * player = new Player(0,0,0,0,0,linkGame);
    linkGame->getBoxMap()[0][1]->setTypeOfBox(1);
    linkGame->getBoxMap()[3][2]->setTypeOfBox(1);
    player->appendToCurrentSelected(QPair<int,int>(0,1));
    player->appendToCurrentSelected(QPair<int,int>(3,2));
    SelectChecker::checkSelected(linkGame, player);
    QVERIFY(player->getCurrentSelected().size() == 0);
    QVERIFY(linkGame->getBoxMap()[0][1]->getBoxState().boxToBeRemoved == false);
    QVERIFY(linkGame->getBoxMap()[3][2]->getBoxState().boxToBeRemoved == false);
    delete linkGame;
    delete player;
}

void SimpleTest::differentTypeWrongTest()
{
    //测试不同类型的方块应当不消除
    LinkGame * linkGame = new LinkGame(4,5);
    Player * player = new Player(0,0,0,0,0,linkGame,1);
    linkGame->getBoxMap()[0][0]->setTypeOfBox(1);;
    linkGame->getBoxMap()[0][1]->setTypeOfBox(2);;
    player->appendToCurrentSelected(QPair<int,int>(0,0));
    player->appendToCurrentSelected(QPair<int,int>(0,1));
    SelectChecker::checkSelected(linkGame, player);
    QPainter painter;
    player->drawLine(painter);
    QVERIFY(player->getCurrentSelected().size() == 0);
    QVERIFY(linkGame->getBoxMap()[0][0]->getBoxState().boxToBeRemoved == false);
    QVERIFY(linkGame->getBoxMap()[0][1]->getBoxState().boxToBeRemoved == false);
    delete linkGame;
    delete player;
}

void SimpleTest::popSameBoxTest()
{
    //测试相同方块只选择一次
    LinkGame * linkGame = new LinkGame(4,5);
    Player * player = new Player(0,0,0,0,0,linkGame,1);
    linkGame->getBoxMap()[0][0]->setTypeOfBox(1);;
    linkGame->getBoxMap()[0][0]->setTypeOfBox(1);;
    player->appendToCurrentSelected(QPair<int,int>(0,0));
    player->appendToCurrentSelected(QPair<int,int>(0,0));
    SelectChecker::checkSelected(linkGame, player);
    QPainter painter;
    player->drawLine(painter);
    QVERIFY(player->getCurrentSelected().size() == 1);
    QVERIFY(linkGame->getBoxMap()[0][0]->getBoxState().boxToBeRemoved == false);
    delete linkGame;
    delete player;
}

void SimpleTest::removeSelectedTest()
{
    //测试取消选中无法消除的方块
    LinkGame * linkGame = new LinkGame(6,5);
    Player * player = new Player(0,0,0,0,0,linkGame,1);
    linkGame->getBoxMap()[0][0]->setTypeOfBox(1);;
    linkGame->getBoxMap()[1][1]->setTypeOfBox(1);;
    player->appendToCurrentSelected(QPair<int,int>(0,0));
    player->appendToCurrentSelected(QPair<int,int>(1,1));
    SelectChecker::checkSelected(linkGame, player);
    QPainter painter;
    player->drawLine(painter);
    QVERIFY(player->getCurrentSelected().size() == 0);
    QVERIFY(linkGame->getBoxMap()[0][0]->getBoxState().boxToBeRemoved == false);
    QVERIFY(linkGame->getBoxMap()[1][1]->getBoxState().boxToBeRemoved == false);
    QVERIFY(linkGame->getBoxMap()[0][0]->getBoxState().boxSelected == false);
    QVERIFY(linkGame->getBoxMap()[1][1]->getBoxState().boxSelected == false);
    delete linkGame;
    delete player;
}

void SimpleTest::reversedBoxSequenceTest()
{
    //测试从另一个先后顺序消除方块
    LinkGame * linkGame = new LinkGame(6,5);
    Player * player = new Player(0,0,0,0,0,linkGame,1);
    linkGame->getBoxMap()[1][1]->setTypeOfBox(1);;
    linkGame->getBoxMap()[4][3]->setTypeOfBox(1);;
    linkGame->getBoxMap()[2][1]->setBoxRemoved(true);
    linkGame->getBoxMap()[2][2]->setBoxRemoved(true);
    linkGame->getBoxMap()[2][3]->setBoxRemoved(true);
    linkGame->getBoxMap()[3][3]->setBoxRemoved(true);
    player->appendToCurrentSelected(QPair<int,int>(4,3));
    player->appendToCurrentSelected(QPair<int,int>(1,1));
    SelectChecker::checkSelected(linkGame, player);
    QPainter painter;
    player->drawLine(painter);
    QVERIFY(player->getCurrentSelected().size() == 0);
    QVERIFY(linkGame->getBoxMap()[1][1]->getBoxState().boxToBeRemoved == true);
    QVERIFY(linkGame->getBoxMap()[4][3]->getBoxState().boxToBeRemoved == true);
    delete linkGame;
    delete player;
}

QTEST_MAIN(SimpleTest);
