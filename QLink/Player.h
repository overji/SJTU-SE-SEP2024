//
// Created by overji on 2024/6/20.
//

#pragma once
#include "LinkGame.h"
#include "SimpleTest.h"
#include "SaveSystem.h"
#include "Gadget.h"
#include <QWidget>

class LinkGame;
class SelectChecker;
class SaveSystem;
class Gadget;
class BoxOfGame;

class Player:public QWidget
{
    Q_OBJECT
public:
    // Getter and setter for playerLeftTopX
    [[nodiscard]] int getPlayerLeftTopX() const { return playerLeftTopX; }
    void setPlayerLeftTopX(int value) { playerLeftTopX = value; }

    // Getter and setter for playerLeftTopY
    [[nodiscard]] int getPlayerLeftTopY() const { return playerLeftTopY; }
    void setPlayerLeftTopY(int value) { playerLeftTopY = value; }

    // Getter and setter for playerWidth
    [[nodiscard]] int getPlayerWidth() const { return playerWidth; }
    void setPlayerWidth(int value) { playerWidth = value; }

    // Getter and setter for playerHeight
    [[nodiscard]] int getPlayerHeight() const { return playerHeight; }
    void setPlayerHeight(int value) { playerHeight = value; }

    // Getter and setter for playerSpeed
    [[nodiscard]] int getPlayerSpeed() const { return playerSpeed; }
    void setPlayerSpeed(int value) { playerSpeed = value; }

    // Getter and setter for score
    [[nodiscard]] int getScore() const { return score; }
    void setScore(int value) { score = value; }

    // Getter and setter for freezeTime
    [[nodiscard]] int getFreezeTime() const { return freezeTime; }
    void setFreezeTime(int value) { freezeTime = value; }

    // Getter and setter for dizzyTime
    [[nodiscard]] int getDizzyTime() const { return dizzyTime; }
    void setDizzyTime(int value) { dizzyTime = value; }

    // Getter and setter for scoreString
    [[nodiscard]] QString getScoreString() const { return scoreString; }
    void setScoreString(QString value) { scoreString = value; }

    // Getter and setter for playerImage
    [[nodiscard]] QPixmap getPlayerImage() const { return playerImage; }
    void setPlayerImage(QPixmap value) { playerImage = value; }

    // Getter and setter for currentSelected
    [[nodiscard]] QVector<QPair<int,int>> getCurrentSelected() const { return currentSelected; }
    void setCurrentSelected(QVector<QPair<int,int>> value) { currentSelected = value; }
    void appendToCurrentSelected(QPair<int,int> value) { currentSelected.append(value); }
    void popCurrentSelected() { currentSelected.pop_back(); }
    void clearCurrentSelected() { currentSelected.clear(); }

    // Getter and setter for linePath
    [[nodiscard]] QVector<QPoint> getLinePath() const { return linePath; }
    void setLinePath(QVector<QPoint> value) { linePath = value; }
    void appendToLinePath(QPoint value) { linePath.append(value); }

    // Getter and setter for toBeRemovedBox
    [[nodiscard]] QVector<QPair<int,int>> getToBeRemovedBox() const { return toBeRemovedBox; }
    void setToBeRemovedBox(QVector<QPair<int,int>> value) { toBeRemovedBox = value; }
    void appendToBeRemovedBox(QPair<int,int> value) { toBeRemovedBox.append(value); }

    // Getter and setter for removeBoxTimer
    [[nodiscard]] QTimer* getRemoveBoxTimer() const { return removeBoxTimer; }
    void setRemoveBoxTimer(QTimer* value) { removeBoxTimer = value; }

    // Getter and setter for removeTimerOn
    [[nodiscard]] bool isRemoveTimerOn() const { return removeTimerOn; }
    void setRemoveTimerOn(bool value) { removeTimerOn = value; }

    // Getter and setter for game
    [[nodiscard]] LinkGame* getGame() const { return game; }
    void setGame(LinkGame* value) { game = value; }

    [[nodiscard]] int getDirection() const { return direction; }
    void setDirection(int value) { direction = value; }

public slots:
    void removeBox(); //消除方块
public:
    explicit Player(const int &playerLeftTopXInput, const int &playerLeftTopYInput, const int &playerWidthInput,
                    const int &playerHeightInput, const int &playerSpeedInput,LinkGame * game,const int& playerSkin = 1); //构造函数
    ~Player(); //析构函数
    void resizePlayer(double xScaleRatio, double yScaleRatio); //调整玩家大小
    void drawPlayer(QPainter & painter); //绘制玩家
    void playerMove(const int &playerDirection, const int &xLoc = 0, const int &yLoc = 0); //玩家移动
    void leftMove(); //玩家向左移动
    void rightMove(); //玩家向右移动
    void upMove(); //玩家向上移动
    void downMove(); //玩家向下移动
    void flashMove(int xLoc,int yLoc); //玩家闪移
    void checkGadgetHit(); //检查玩家是否碰到道具
    void drawLine(QPainter & painter); //绘制线
    void checkClose(); //检查是否有相邻的方块
    bool checkColIndexValid(const int & xIndex);     //检查列索引是否合法
    bool checkRowIndexValid(const int & yIndex);    //检查行索引是否合法
    bool flashMoveCheckEdge(int rowLoc,int colLoc); //检查flash移动是否处于箱子边缘处
    bool flashMoveCheckCenter(int rowLoc,int colLoc); //检查flash移动是否处于箱子中间
private:
    void initPlayerLocation();
    void clearSelected(); //消除选中的方块
    int playerLeftTopX; //玩家左上角x坐标
    int playerLeftTopY; //玩家左上角y坐标
    int playerWidth; //玩家宽度
    int playerHeight; //玩家高度
    int playerSpeed; //玩家速度
    int score; //玩家分数
    int freezeTime; //玩家冻结时间
    int dizzyTime; //玩家眩晕时间
    QString scoreString; //玩家分数字符串
    QPixmap playerImage; //玩家图片
    QVector<QPair<int,int>> currentSelected;    //当前选中的方块

    QVector<QPoint> linePath; //路径
    QVector<QPair<int,int>> toBeRemovedBox; //待消除箱子
    QTimer * removeBoxTimer; //消除计时器
    bool removeTimerOn; //消除计时器是否开启
    LinkGame * game;

    int direction;//玩家移动方向 ，0上 1下 2左 3右 4不动

    void leftMoveCheckEdge(int &currentTopY, BoxOfGame *&topBox, int &topIndex, int &currentBottomY, BoxOfGame *&bottomBox,
                      int &bottomIndex);

    void rightMoveCheckEdge(int &currentTopY, BoxOfGame *&topBox, int &topIndex, int &currentBottomY, BoxOfGame *&bottomBox,
                       int &bottomIndex);

    void
    upMoveCheckEdge(int &currentLeftCol, BoxOfGame *&leftBox, int &leftIndex, int &currentRightCol,
                    BoxOfGame *&rightBox,
                    int &rightIndex);

    void
    downMoveCheckEdge(int &currentLeftX, BoxOfGame *&leftBox, int &leftIndex, int &currentRightX, BoxOfGame *&rightBox,
                      int &rightIndex);
};

