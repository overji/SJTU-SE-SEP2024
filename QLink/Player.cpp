//
// Created by overji on 2024/6/20.
//

#include "Player.h"
#include "LinkGame.h"
#include "SelectChecker.h"
#include <QPainter>
#include <QRandomGenerator>
#include <iostream>
#include "SpecialAlgorithm.h"

Player::Player(const int &playerLeftTopXInput, const int &playerLeftTopYInput, const int &playerWidthInput,
               const int &playerHeightInput, const int &playerSpeedInput, LinkGame *game, const int &playerSkin)
{
    //初始化玩家的各项参数,playerLeftTopXInput:玩家左上角x坐标,playerLeftTopYInput:玩家左上角y坐标,playerWidthInput:玩家宽度,playerHeightInput:玩家高度,playerSpeedInput:玩家速度,game:游戏对象,playerSkin:玩家皮肤
    this->playerHeight = playerHeightInput;
    this->playerLeftTopX = playerLeftTopXInput;
    this->playerLeftTopY = playerLeftTopYInput;
    this->playerSpeed = playerSpeedInput;
    this->playerWidth = playerWidthInput;
    this->freezeTime = 0;
    this->dizzyTime = 0;
    this->score = 0;
    this->direction = 4;
    this->scoreString = "分数: 0";
    //设置玩家贴图
    std::filesystem::path curFilePath(__FILE__);
    std::filesystem::path curPath = curFilePath.parent_path();
    if (playerSkin == 1) {
        this->playerImage = QPixmap(QString::fromStdString(
                curPath.string() + "\\images\\PlayerImages\\20f432ebdfe483eee238d36d99e42a13.png"));
    } else {
        this->playerImage = QPixmap(QString::fromStdString(
                curPath.string() + "\\images\\PlayerImages\\1b49f5bd6f8f4d03052778304564cc38.png"));
    }

    removeBoxTimer = new QTimer(this);  // 初始化QTimer对象
    this->game = game;
    connect(removeBoxTimer, SIGNAL(timeout()), this, SLOT(removeBox()));  // 连接QTimer的timeout信号和槽函数
    removeTimerOn = false;  // 初始化定时器关闭状态
    removeBoxTimer->setSingleShot(true);  // 设置定时器为单次触发模式
    if (this->playerLeftTopX == -1) {
        //初始化玩家位置，如果不是-1就直接读取位置
        this->initPlayerLocation();
        if (game->getGameType() == 1) {
            //双人游戏时，玩家1和玩家2的初始位置不同
            if (this == game->getPlayer2()) {
                while (abs(this->playerLeftTopX - game->getPlayer1()->playerLeftTopX) <= this->playerWidth ||
                       abs(this->playerLeftTopY - game->getPlayer1()->playerLeftTopY) <= this->playerHeight) {
                    this->initPlayerLocation();
                }
            }
        }
    }
}

void Player::initPlayerLocation()
{
    //初始化玩家位置
    int gamePassageWidth = game->getPassageWidth();
    int gamePassageHeight = game->getPassageHeight();
    int randomX = QRandomGenerator::global()->bounded(0, 800 - this->playerWidth);
    if (randomX <= 800 - gamePassageWidth && randomX >= gamePassageWidth) {
        //玩家不在左右两侧空地，在上下两侧随机生成
        int totalSpace = 2 * (gamePassageHeight - playerHeight);
        int randomY = QRandomGenerator::global()->bounded(0, totalSpace);
        if (randomY <= gamePassageHeight - playerHeight) {
            this->playerLeftTopX = randomX;
            this->playerLeftTopY = randomY;
        } else {
            this->playerLeftTopX = randomX;
            this->playerLeftTopY = (randomY + playerHeight - gamePassageHeight) + (600 - gamePassageHeight);
        }
    } else {
        //玩家在左右两侧空地，随机生成位置
        this->playerLeftTopX = randomX;
        this->playerLeftTopY = QRandomGenerator::global()->bounded(0, 600 - playerHeight);
    }
}

Player::~Player()
{
    //析构函数
    this->currentSelected.clear();
    delete removeBoxTimer;
}

void Player::leftMoveCheckEdge(int & currentTopY,BoxOfGame *&topBox,int & topIndex,int & currentBottomY,BoxOfGame * &bottomBox,int & bottomIndex)
{
    for (int i = 0; i < game->getBoxCol(); i++) {
        //找到最右边没有被删除的箱子
        if (currentTopY < game->getBoxRow() && currentTopY >= 0 &&
            !(game->getBoxMap())[currentTopY][i]->getBoxState().boxRemoved) {
            if ((game->getBoxMap())[currentTopY][i]->getLeftTopX() > playerLeftTopX + game->getBoxWidth()) {
                break;
            }
            topBox = (game->getBoxMap())[currentTopY][i];
            topIndex = i;
        }
        if (currentBottomY < game->getBoxRow() && currentBottomY >= 0 &&
            !(game->getBoxMap())[currentBottomY][i]->getBoxState().boxRemoved) {
            if ((game->getBoxMap())[currentBottomY][i]->getLeftTopX() > playerLeftTopX + game->getBoxWidth()) {
                break;
            }
            bottomBox = (game->getBoxMap())[currentBottomY][i];
            bottomIndex = i;
        }
    }
}

void Player::leftMove()
{
    //玩家向左移动
    if (playerLeftTopX >= 0) {
        playerLeftTopX -= playerSpeed;
    }
    //检测向左移动的终点是否可到达
    if (playerLeftTopY + playerHeight > game->getPassageHeight() && playerLeftTopY < 600 - game->getPassageHeight()) {
        //开始检测碰撞
        int currentTopY = (playerLeftTopY - game->getPassageHeight()) / game->getBoxHeight();
        int currentBottomY = (playerLeftTopY + playerHeight - game->getPassageHeight()) / game->getBoxHeight();
        BoxOfGame *topBox = nullptr;
        int topIndex = 0;
        BoxOfGame *bottomBox = nullptr;
        int bottomIndex = 0;
        leftMoveCheckEdge(currentBottomY, bottomBox, bottomIndex, currentTopY, topBox, topIndex);
        //获取箱子的右边界
        int topBoxRight = (topBox == nullptr) ? -1000 : topBox->getLeftTopX() + game->getBoxWidth();
        int bottomBoxRight = (bottomBox == nullptr) ? -1000 : bottomBox->getLeftTopX() + game->getBoxWidth();
        if (playerLeftTopX <= topBoxRight && playerLeftTopX >= (topBoxRight - this->playerSpeed)) {
            //左边遇到箱子，限制移动，并且选中箱子
            topBox->setBoxSelected(true);
            playerLeftTopX = topBoxRight + 1;
            currentSelected.push_back(QPair<int, int>(currentTopY, topIndex));
            SelectChecker::checkSelected(game, this);
            return;
        }
        if (playerLeftTopX <= bottomBoxRight && playerLeftTopX >= (bottomBoxRight - this->playerSpeed)) {
            //左边遇到箱子，限制移动，并且选中箱子
            bottomBox->setBoxSelected(true);
            playerLeftTopX = bottomBoxRight + 1;
            currentSelected.push_back(QPair<int, int>(currentBottomY, bottomIndex));
            SelectChecker::checkSelected(game, this);
            return;
        }
    }
    game->setRemoveText("");
    //限制玩家移动范围不得超过左边界
    if (playerLeftTopX < 0) {
        playerLeftTopX = 0;
    }
}

void Player::rightMoveCheckEdge(int & currentTopY,BoxOfGame *& topBox,int & topIndex,int & currentBottomY,BoxOfGame *& bottomBox,int & bottomIndex)
{
    for (int i = game->getBoxCol() - 1; i >= 0; i--) {
        //找到最左边没有被删除的箱子
        if (currentTopY < game->getBoxRow() && currentTopY >= 0 &&
            !(game->getBoxMap())[currentTopY][i]->getBoxState().boxRemoved) {
            if ((game->getBoxMap())[currentTopY][i]->getLeftTopX() + game->getBoxWidth() < playerLeftTopX) {
                break;
            }
            topBox = (game->getBoxMap())[currentTopY][i];
            topIndex = i;
        }
        if (currentBottomY < game->getBoxRow() && currentBottomY >= 0 &&
            !(game->getBoxMap())[currentBottomY][i]->getBoxState().boxRemoved) {
            if ((game->getBoxMap())[currentBottomY][i]->getLeftTopX() + game->getBoxWidth() < playerLeftTopX) {
                break;
            }
            bottomBox = (game->getBoxMap())[currentBottomY][i];
            bottomIndex = i;
        }
    }
}

void Player::rightMove()
{
    //玩家向右移动
    if (playerLeftTopX + playerWidth <= 800) {
        playerLeftTopX += playerSpeed;
    }
    //检测向右移动的终点是否可到达
    if (playerLeftTopY + playerHeight >= game->getPassageHeight() &&
        playerLeftTopY <= 600 - game->getPassageHeight()) {
        int currentTopY = (playerLeftTopY - game->getPassageHeight()) / game->getBoxHeight();
        int currentBottomY = (playerLeftTopY + playerHeight - game->getPassageHeight()) / game->getBoxHeight();
        BoxOfGame *topBox = nullptr;
        int topIndex = 0;
        BoxOfGame *bottomBox = nullptr;
        int bottomIndex = 0;
        rightMoveCheckEdge(currentTopY, topBox, topIndex, currentBottomY, bottomBox, bottomIndex);
        int topBoxLeft = (topBox == nullptr) ? 1000 : topBox->getLeftTopX();
        int bottomBoxLeft = (bottomBox == nullptr) ? 1000 : bottomBox->getLeftTopX();
        if (playerLeftTopX + playerWidth > topBoxLeft && playerLeftTopX + playerWidth <= topBoxLeft + playerSpeed &&
            topBox) {
            //右边遇到箱子，限制移动，并且选中箱子
            topBox->setBoxSelected(true);
            playerLeftTopX = topBoxLeft - playerWidth - 1;
            currentSelected.push_back(QPair<int, int>(currentTopY, topIndex));
            SelectChecker::checkSelected(game, this);
            return;
        }
        if (playerLeftTopX + playerWidth > bottomBoxLeft &&
            playerLeftTopX + playerWidth <= bottomBoxLeft + playerSpeed && bottomBox) {
            //右边遇到箱子，限制移动，并且选中箱子
            bottomBox->setBoxSelected(true);
            playerLeftTopX = bottomBoxLeft - playerWidth - 1;
            currentSelected.push_back(QPair<int, int>(currentBottomY, bottomIndex));
            SelectChecker::checkSelected(game, this);
            return;
        }
    }
    game->setRemoveText("");
    //限制玩家移动范围不得超过右边界
    if (playerLeftTopX + playerWidth > 800) {
        playerLeftTopX = 800 - playerWidth;
    }
}

void Player::upMoveCheckEdge(int & currentLeftCol,BoxOfGame *& leftBox,int & leftIndex,int & currentRightCol,BoxOfGame *& rightBox,int & rightIndex)
{
    for (int i = 0; i < game->getBoxRow(); i++) {
        //找到最下边没有被删除的箱子
        if (currentLeftCol < game->getBoxCol() && currentLeftCol >= 0 &&
            !(game->getBoxMap())[i][currentLeftCol]->getBoxState().boxRemoved) {
            if ((game->getBoxMap())[i][currentLeftCol]->getLeftTopY() > playerLeftTopY) {
                break;
            }
            leftBox = (game->getBoxMap())[i][currentLeftCol];
            leftIndex = i;
        }
        if (currentRightCol < game->getBoxCol() && currentRightCol >= 0 &&
            !(game->getBoxMap())[i][currentRightCol]->getBoxState().boxRemoved) {
            if ((game->getBoxMap())[i][currentRightCol]->getLeftTopY() > playerLeftTopY) {
                break;
            }
            rightBox = (game->getBoxMap())[i][currentRightCol];
            rightIndex = i;
        }
    }
}

void Player::upMove() {
    //玩家向上移动
    if (playerLeftTopY >= 0) {
        playerLeftTopY -= playerSpeed;
    }
    //检测向上移动的终点是否可到达
    if (playerLeftTopX + playerWidth > game->getPassageWidth() && playerLeftTopX < 800 - game->getPassageWidth()) {
        //开始检测碰撞
        int currentLeftCol = (playerLeftTopX - game->getPassageWidth()) / game->getBoxWidth();
        int currentRightCol = (playerLeftTopX + playerWidth - game->getPassageWidth()) / game->getBoxWidth();
        BoxOfGame *leftBox = nullptr;
        int leftIndex = 0;
        BoxOfGame *rightBox = nullptr;
        int rightIndex = 0;
        upMoveCheckEdge(currentLeftCol, leftBox, leftIndex, currentRightCol, rightBox, rightIndex);
        //获取箱子的下边界
        int leftBoxBottom = (leftBox == nullptr) ? -1000 : leftBox->getLeftTopY() + game->getBoxHeight();
        int rightBoxBottom = (rightBox == nullptr) ? -1000 : rightBox->getLeftTopY() + game->getBoxHeight();
        //检测是否碰到箱子，如果碰到箱子还需限制移动
        if (playerLeftTopY <= leftBoxBottom && playerLeftTopY >= leftBoxBottom - playerSpeed - 1) {
            leftBox->setBoxSelected(true);
            playerLeftTopY = leftBoxBottom + 1;
            currentSelected.push_back(QPair<int, int>(leftIndex, currentLeftCol));
            SelectChecker::checkSelected(game, this);
            return;
        }
        if (playerLeftTopY <= rightBoxBottom && playerLeftTopY >= leftBoxBottom - playerSpeed - 1) {
            rightBox->setBoxSelected(true);
            playerLeftTopY = rightBoxBottom + 1;
            currentSelected.push_back(QPair<int, int>(rightIndex, currentRightCol));
            SelectChecker::checkSelected(game, this);
            return;
        }
    }
    game->setRemoveText("");
    if (playerLeftTopY < 0) {
        //限制玩家移动范围不得超过上边界
        playerLeftTopY = 0;
    }
}

void Player::downMoveCheckEdge(int & currentLeftX,BoxOfGame *& leftBox,int & leftIndex,int & currentRightX,BoxOfGame *& rightBox,int & rightIndex)
{
    for (int i = game->getBoxRow() - 1; i >= 0; i--) {
        //找到最上边没有被删除的箱子
        if (currentLeftX < game->getBoxCol() && currentLeftX >= 0 &&
            !(game->getBoxMap())[i][currentLeftX]->getBoxState().boxRemoved) {
            if ((game->getBoxMap())[i][currentLeftX]->getLeftTopY() + game->getBoxHeight() < playerLeftTopY) {
                break;
            }
            leftBox = (game->getBoxMap())[i][currentLeftX];
            leftIndex = i;
        }
        if (currentRightX < game->getBoxCol() && currentRightX >= 0 &&
            !(game->getBoxMap())[i][currentRightX]->getBoxState().boxRemoved) {
            if ((game->getBoxMap())[i][currentRightX]->getLeftTopY() + game->getBoxHeight() < playerLeftTopY) {
                break;
            }
            rightBox = (game->getBoxMap())[i][currentRightX];
            rightIndex = i;
        }
    }
}

void Player::downMove()
{
    //玩家向下移动
    if ((playerLeftTopY + playerHeight) <= 600) {
        playerLeftTopY += playerSpeed;
    }
    //检测向下移动的终点是否可到达
    if (playerLeftTopX + playerWidth > game->getPassageWidth() && playerLeftTopX < 800 - game->getPassageWidth()) {
        //开始检测碰撞
        int currentLeftX = (playerLeftTopX - game->getPassageWidth()) / game->getBoxWidth();
        int currentRightX = (playerLeftTopX + playerWidth - game->getPassageWidth()) / game->getBoxWidth();
        BoxOfGame *leftBox = nullptr;
        int leftIndex = 0;
        BoxOfGame *rightBox = nullptr;
        int rightIndex = 0;
        downMoveCheckEdge(currentLeftX, leftBox, leftIndex, currentRightX, rightBox, rightIndex);
        //获取箱子的上边界
        int leftBoxTop = (leftBox == nullptr) ? 1000 : leftBox->getLeftTopY();
        int rightBoxTop = (rightBox == nullptr) ? 1000 : rightBox->getLeftTopY();
        //检测是否碰到箱子，如果碰到箱子还需限制移动
        if (playerLeftTopY + playerHeight > leftBoxTop && playerLeftTopY + playerHeight <= leftBoxTop + playerSpeed) {
            leftBox->setBoxSelected(true);
            playerLeftTopY = leftBoxTop - playerHeight - 1;
            currentSelected.push_back(QPair<int, int>(leftIndex, currentLeftX));
            SelectChecker::checkSelected(game, this);
            return;
        }
        if (playerLeftTopY + playerHeight > rightBoxTop && playerLeftTopY + playerHeight <= rightBoxTop + playerSpeed) {
            rightBox->setBoxSelected(true);
            playerLeftTopY = rightBoxTop - playerHeight - 1;
            currentSelected.push_back(QPair<int, int>(rightIndex, currentRightX));
            SelectChecker::checkSelected(game, this);
            return;
        }
    }
    game->setRemoveText("");
    if ((playerLeftTopY + playerHeight) > 600) {
        //限制玩家移动范围不得超过下边界
        playerLeftTopY = 600 - playerHeight;
    }
}

void Player::resizePlayer(double xScaleRatio, double yScaleRatio)
{
    //窗口大小变化时，调整玩家大小，xScaleRatio:宽度缩放比例,yScaleRatio:高度缩放比例
    this->playerHeight = (1 * playerWidth * xScaleRatio) / yScaleRatio;
}

void Player::drawPlayer(QPainter &painter)
{
    //绘制玩家
    painter.save();
    painter.setBrush(QColor(142, 68, 173));
    //绘制玩家的四个顶点
    const QPoint playerPoints[4] = {
            QPoint(playerLeftTopX, playerLeftTopY),
            QPoint(playerLeftTopX + playerWidth, playerLeftTopY),
            QPoint(playerLeftTopX + playerWidth, playerLeftTopY + playerHeight),
            QPoint(playerLeftTopX, playerLeftTopY + playerHeight)
    };
    painter.drawConvexPolygon(playerPoints, 4);
    painter.drawPixmap(playerLeftTopX, playerLeftTopY, playerWidth, playerHeight, this->playerImage);
    painter.restore();
}

void Player::playerMove(const int &playerDirection, const int &xLoc, const int &yLoc)
{
    //玩家移动，注意这里的playerDirection应当由外界传入，以方便判断是否是flashMove
    if (game->isGamePause()) {
        //游戏暂停时，玩家无法移动
        return;
    }
    if (game->isGameEnd()) {
        //游戏结束时，玩家无法移动
        return;
    }
    if (this->freezeTime != 0) {
        //玩家被冻结时，无法移动
        return;
    }
    bool startDizzy = false;
    if (this->dizzyTime != 0) {
        //玩家被眩晕时，方向相反
        startDizzy = true;
    }
    if (!SelectChecker::solutionExist(game, this)) {
        //无解时，游戏结束
        game->setRemoveText("无解!");
        game->setGameEnd(true);
        game->setGamePause(false);
        game->setNoSolution(true);
        game->setSummaryText("游戏结束!");
        return;
    }
    if (!startDizzy) {
        //正常移动
        switch (playerDirection) {
            case 0:
                upMove();
                break;
            case 1:
                downMove();
                break;
            case 2:
                leftMove();
                break;
            case 3:
                rightMove();
                break;
            default:
                break;
        }
    } else {
        //眩晕时，方向相反
        switch (playerDirection) {
            case 0:
                downMove();
                break;
            case 1:
                upMove();
                break;
            case 2:
                rightMove();
                break;
            case 3:
                leftMove();
                break;
            default:
                break;
        }
    }

    if (playerDirection == -5) {
        //flash移动
        flashMove(xLoc, yLoc);
    }
    //检查是否有相邻的两个方块可以消除
    this->scoreString = "分数: " + QString::number(score);
    checkGadgetHit();
}

void Player::checkClose()
{
    //检查是否有相邻的方块
    int currentLeftX = specialDiv((playerLeftTopX - playerSpeed - game->getPassageWidth()), game->getBoxWidth());
    int currentTopY = specialDiv((playerLeftTopY - playerSpeed - game->getPassageHeight()), game->getBoxHeight());
    int currentRightX = specialDiv((playerLeftTopX + playerWidth - game->getPassageWidth() + playerSpeed),
                                   game->getBoxWidth());
    int currentBottomY = specialDiv((playerLeftTopY + playerHeight - game->getPassageHeight() + playerSpeed),
                                    game->getBoxHeight());
    if (checkColIndexValid(currentLeftX) && checkRowIndexValid(currentTopY)) {
        //左上方块
        (game->getBoxMap())[currentTopY][currentLeftX]->setBoxClose(true);
    }
    if (checkColIndexValid(currentLeftX) && checkRowIndexValid(currentBottomY)) {
        //左下方块
        (game->getBoxMap())[currentBottomY][currentLeftX]->setBoxClose(true);
    }
    if (checkColIndexValid(currentRightX) && checkRowIndexValid(currentTopY)) {
        //右上方块
        (game->getBoxMap())[currentTopY][currentRightX]->setBoxClose(true);
    }
    if (checkColIndexValid(currentRightX) && checkRowIndexValid(currentBottomY)) {
        //右下方块
        (game->getBoxMap())[currentBottomY][currentRightX]->setBoxClose(true);
    }
}

void Player::clearSelected()
{
    //清除玩家选中的方块
    for (int i = 0; i < game->getBoxRow(); i++) {
        for (int j = 0; j < game->getBoxCol(); j++) {
            (game->getBoxMap())[i][j]->setBoxSelected(false);;
        }
    }
}

bool Player::checkColIndexValid(const int &xIndex)
{
    //检查列索引是否合法
    return xIndex >= 0 && xIndex < game->getBoxCol();
}

bool Player::checkRowIndexValid(const int &yIndex)
{
    //检查行索引是否合法
    return yIndex >= 0 && yIndex < game->getBoxRow();
}

void Player::checkGadgetHit()
{
    //检查玩家是否碰到道具
    for (auto gadget: game->getGadgets()) {
        if (gadget->getLeftTopX() <= playerLeftTopX + playerWidth &&
            gadget->getLeftTopX() + gadget->getGadgetWidth() >= playerLeftTopX &&
            gadget->getLeftTopY() <= playerLeftTopY + playerHeight &&
            gadget->getLeftTopY() + gadget->getGadgetHeight() >= playerLeftTopY) {
            if (game->getGameType() == 0) {
                gadget->singleGameGadgetEffect(game, this);
            } else {
                //双人游戏时，判断玩家1和玩家2，并且给对方加上效果
                if (this == game->getPlayer1()) {
                    gadget->doubleGameGadgetEffect(game, game->getPlayer2());
                } else {
                    gadget->doubleGameGadgetEffect(game, game->getPlayer1());
                }
            }
            game->removeGadget(gadget);
            delete gadget;
        }
    }
}

void Player::flashMove(int xLoc, int yLoc)
{
    //判断是否点击到了方块上
    int topRowLoc = specialDiv(yLoc - game->getPassageHeight() - playerHeight / 2, game->getBoxWidth());
    int bottomRowLoc = specialDiv(yLoc - game->getPassageHeight() + playerHeight / 2, game->getBoxWidth());
    int leftColLoc = specialDiv(xLoc - game->getPassageWidth() - playerWidth / 2, game->getBoxWidth());
    int rightColLoc = specialDiv(xLoc - game->getPassageWidth() + playerWidth / 2, game->getBoxWidth());
    if (checkColIndexValid(leftColLoc) && checkRowIndexValid(topRowLoc) && !(game->getBoxMap())[topRowLoc][leftColLoc]->getBoxState().boxRemoved) {
        //左上角有箱子
        xLoc += playerWidth / 2;
        yLoc += playerWidth / 2;
    }
    if (checkColIndexValid(rightColLoc) && checkRowIndexValid(topRowLoc) && !(game->getBoxMap())[topRowLoc][rightColLoc]->getBoxState().boxRemoved) {
        //右上角有箱子
        xLoc -= playerWidth / 2;
        yLoc += playerWidth / 2;
    }
    if (checkColIndexValid(leftColLoc) && checkRowIndexValid(bottomRowLoc) && !(game->getBoxMap())[bottomRowLoc][leftColLoc]->getBoxState().boxRemoved) {
        //左下角有箱子
        xLoc += playerWidth / 2;
        yLoc -= playerWidth / 2;
    }
    if (checkColIndexValid(rightColLoc) && checkRowIndexValid(bottomRowLoc) && !(game->getBoxMap())[bottomRowLoc][rightColLoc]->getBoxState().boxRemoved) {
        //右下角有箱子
        xLoc -= playerWidth / 2;
        yLoc -= playerWidth / 2;
    }
    int rowLoc = specialDiv(yLoc - game->getPassageHeight(), game->getBoxHeight());
    int colLoc = specialDiv(xLoc - game->getPassageWidth(), game->getBoxWidth());
    //判断是否点击到了箱子上
    if (checkColIndexValid(colLoc) && checkRowIndexValid(rowLoc)) {
        if ((game->getBoxMap())[rowLoc][colLoc]->getBoxState().boxRemoved) {
            //箱子已被清除
            this->playerLeftTopY = yLoc - (this->playerWidth / 2);
            this->playerLeftTopX = xLoc - (this->playerHeight / 2);
        } else {
            //点击到了箱子上，检测是否在箱子中或箱子边缘
            if (flashMoveCheckEdge(rowLoc, colLoc)) {
                return;
            }
            if (flashMoveCheckCenter(rowLoc, colLoc)) {
                return;
            }
        }
    } else {
        //点击到了箱子之外
        this->playerLeftTopY = yLoc - (this->playerWidth / 2);
        this->playerLeftTopX = xLoc - (this->playerHeight / 2);
        return;
    }
}

bool Player::flashMoveCheckEdge(int rowLoc, int colLoc)
{
    //检查flash移动是否处于箱子边缘处，如果是就把玩家放到箱子边缘，并且选中箱子,rowLoc:行索引，colLoc:列索引
    if (rowLoc == 0) {
        //放到上面
        this->playerLeftTopY = (game->getBoxMap())[rowLoc][colLoc]->getLeftTopY() - this->playerHeight - 2;
        this->playerLeftTopX = (game->getBoxMap())[rowLoc][colLoc]->getLeftTopX() + (game->getBoxWidth() / 2) -
                               (this->playerWidth / 2);
        (game->getBoxMap())[rowLoc][colLoc]->setBoxSelected(true);
        currentSelected.push_back(QPair<int, int>(rowLoc, colLoc));
        SelectChecker::checkSelected(game, this);
        return true;
    }
    if (rowLoc == (game->getBoxRow() - 1)) {
        //放到下面
        this->playerLeftTopY = (game->getBoxMap())[rowLoc][colLoc]->getLeftTopY() + game->getBoxHeight() + 2;
        this->playerLeftTopX = (game->getBoxMap())[rowLoc][colLoc]->getLeftTopX() + (game->getBoxWidth() / 2) -
                               (this->playerWidth / 2);
        (game->getBoxMap())[rowLoc][colLoc]->setBoxSelected(true);
        currentSelected.push_back(QPair<int, int>(rowLoc, colLoc));
        SelectChecker::checkSelected(game, this);
        return true;
    }
    if (colLoc == 0) {
        //放到左边
        this->playerLeftTopY = (game->getBoxMap())[rowLoc][colLoc]->getLeftTopY() + (game->getBoxHeight() / 2) -
                               (this->playerHeight / 2);
        this->playerLeftTopX = (game->getBoxMap())[rowLoc][colLoc]->getLeftTopX() - this->playerWidth - 2;
        (game->getBoxMap())[rowLoc][colLoc]->setBoxSelected(true);
        currentSelected.push_back(QPair<int, int>(rowLoc, colLoc));
        SelectChecker::checkSelected(game, this);
        return true;
    }
    if (colLoc == (game->getBoxCol() - 1)) {
        //放到右边
        this->playerLeftTopY = (game->getBoxMap())[rowLoc][colLoc]->getLeftTopY() + (game->getBoxHeight() / 2) -
                               (this->playerHeight / 2);
        this->playerLeftTopX = (game->getBoxMap())[rowLoc][colLoc]->getLeftTopX() + game->getBoxWidth() + 2;
        (game->getBoxMap())[rowLoc][colLoc]->setBoxSelected(true);
        currentSelected.push_back(QPair<int, int>(rowLoc, colLoc));
        SelectChecker::checkSelected(game, this);
        return true;
    }
    return false;
}

bool Player::flashMoveCheckCenter(int rowLoc, int colLoc)
{
    //检查flash移动是否处于箱子中间，并且点击的箱子周边是否还有空位，如果是就把玩家放到箱子边缘，并且选中箱子，rowLoc:行索引，colLoc:列索引
    if (checkColIndexValid(colLoc - 1) && checkRowIndexValid(rowLoc)) {
        //左边有空位
        if ((game->getBoxMap())[rowLoc][colLoc - 1]->getBoxState().boxRemoved) {
            this->playerLeftTopY = (game->getBoxMap())[rowLoc][colLoc - 1]->getLeftTopY() + (game->getBoxHeight() / 2) -
                                   (this->playerWidth / 2);
            this->playerLeftTopX =
                    (game->getBoxMap())[rowLoc][colLoc - 1]->getLeftTopX() + game->getBoxWidth() - this->playerWidth -
                    2;
            (game->getBoxMap())[rowLoc][colLoc]->setBoxSelected(true);
            currentSelected.push_back(QPair<int, int>(rowLoc, colLoc));
            SelectChecker::checkSelected(game, this);
            return true;
        }
    }
    if (checkColIndexValid(colLoc + 1) && checkRowIndexValid(rowLoc)) {
        //右边有空位
        if ((game->getBoxMap())[rowLoc][colLoc + 1]->getBoxState().boxRemoved) {
            this->playerLeftTopY = (game->getBoxMap())[rowLoc][colLoc + 1]->getLeftTopY() + (game->getBoxHeight() / 2) -
                                   (this->playerWidth / 2);
            this->playerLeftTopX = (game->getBoxMap())[rowLoc][colLoc + 1]->getLeftTopX() + 2;
            (game->getBoxMap())[rowLoc][colLoc]->setBoxSelected(true);
            currentSelected.push_back(QPair<int, int>(rowLoc, colLoc));
            SelectChecker::checkSelected(game, this);
            return true;
        }
    }
    if (checkColIndexValid(colLoc) && checkRowIndexValid(rowLoc - 1)) {
        //上面有空位
        if ((game->getBoxMap())[rowLoc - 1][colLoc]->getBoxState().boxRemoved) {
            this->playerLeftTopY =
                    (game->getBoxMap())[rowLoc - 1][colLoc]->getLeftTopY() + game->getBoxHeight() - this->playerHeight -
                    2;
            this->playerLeftTopX = (game->getBoxMap())[rowLoc - 1][colLoc]->getLeftTopX() + (game->getBoxWidth() / 2) -
                                   (this->playerWidth / 2);
            (game->getBoxMap())[rowLoc][colLoc]->setBoxSelected(true);
            currentSelected.push_back(QPair<int, int>(rowLoc, colLoc));
            SelectChecker::checkSelected(game, this);
            return true;
        }
    }
    if (checkColIndexValid(colLoc) && checkRowIndexValid(rowLoc + 1)) {
        //下面有空位
        if ((game->getBoxMap())[rowLoc + 1][colLoc]->getBoxState().boxRemoved) {
            this->playerLeftTopY = (game->getBoxMap())[rowLoc + 1][colLoc]->getLeftTopY() + 2;
            this->playerLeftTopX = (game->getBoxMap())[rowLoc + 1][colLoc]->getLeftTopX() + (game->getBoxWidth() / 2) -
                                   (this->playerWidth / 2);
            (game->getBoxMap())[rowLoc][colLoc]->setBoxSelected(true);
            currentSelected.push_back(QPair<int, int>(rowLoc, colLoc));
            SelectChecker::checkSelected(game, this);
            return true;
        }
    }
    return false;
}

void Player::drawLine(QPainter &painter)
{
    //绘制线路，并且在箱子上标记需要删除的箱子
    if (linePath.empty())return;
    painter.setPen(QPen(QColor(255, 0, 0), 5));
    for (int i = 0; i < linePath.size() - 1; i++) {
        painter.drawLine(linePath[i], linePath[i + 1]);
    }
    for (auto i: toBeRemovedBox) {
        (game->getBoxMap())[i.first][i.second]->setBoxToBeRemoved(true);
    }
    if (!removeTimerOn) {
        //如果没有开启定时器，就开启定时器
        removeTimerOn = true;
        removeBoxTimer->start(200);
    }
}

void Player::removeBox()
{
    //0.2s过后消除箱子
    if (game->isGamePause()) {
        return;
    }
    //消除箱子
    for (auto i: toBeRemovedBox) {
        (game->getBoxMap())[i.first][i.second]->setBoxRemoved(true);
        game->setRemainBoxNumber(game->getRemainBoxNumber() - 1);
    }
    toBeRemovedBox.clear();
    //清除玩家选中的箱子
    currentSelected.clear();
    //清除线路
    linePath.clear();
    //关闭计时器，检查游戏是否结束
    removeTimerOn = false;
    game->checkGameEnd();
}