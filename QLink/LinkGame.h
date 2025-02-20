//
// Created by overji on 2024/6/20.
//

#pragma once

#include <QWidget>
#include <QTime>
#include <QPair>
#include <QVector>
#include <QQueue>
#include "BoxOfGame.h"
#include "Player.h"
#include "Gadget.h"
#include "SaveSystem.h"
#include "SelectChecker.h"
#include "SpecialAlgorithm.h"
#include "MainPage.h"

class Player;
class BoxOfGame;
class Gadget;
class SaveSystem;

class LinkGame : public QWidget
{
    Q_OBJECT

public:
    // Getter and setter for gameFps
    [[nodiscard]] int getGameFps() const { return gameFps; }
    void setGameFps(int value) { gameFps = value; }

    // Getter and setter for remainBoxNumber
    [[nodiscard]] int getRemainBoxNumber() const { return remainBoxNumber; }
    void setRemainBoxNumber(int value) { remainBoxNumber = value; }

    // Getter and setter for boxCol
    [[nodiscard]] int getBoxCol() const { return boxCol; }
    void setBoxCol(int value) { boxCol = value; }

    // Getter and setter for boxRow
    [[nodiscard]] int getBoxRow() const { return boxRow; }
    void setBoxRow(int value) { boxRow = value; }

    // Getter and setter for boxWidth
    [[nodiscard]] int getBoxWidth() const { return boxWidth; }
    void setBoxWidth(int value) { boxWidth = value; }

    // Getter and setter for boxHeight
    [[nodiscard]] int getBoxHeight() const { return boxHeight; }
    void setBoxHeight(int value) { boxHeight = value; }

    // Getter and setter for passageWidth
    [[nodiscard]] int getPassageWidth() const { return passageWidth; }
    void setPassageWidth(int value) { passageWidth = value; }

    // Getter and setter for passageHeight
    [[nodiscard]] int getPassageHeight() const { return passageHeight; }
    void setPassageHeight(int value) { passageHeight = value; }

    // Getter and setter for xScaleRatio
    [[nodiscard]] double getXScaleRatio() const { return xScaleRatio; }
    void setXScaleRatio(double value) { xScaleRatio = value; }

    [[nodiscard]] QVector<Gadget *> getGadgets() const { return gadgets; }
    void setGadgets(const QVector<Gadget *> &value) { gadgets = value; }
    void removeGadget(Gadget *gadget) {gadgets.removeOne(gadget);};

    // Getter and setter for yScaleRatio
    [[nodiscard]] double getYScaleRatio() const { return yScaleRatio; }
    void setYScaleRatio(double value) { yScaleRatio = value; }

    // Getter and setter for gameEnd
    [[nodiscard]] bool isGameEnd() const { return gameEnd; }
    void setGameEnd(bool value) { gameEnd = value; }

    // Getter and setter for gamePause
    [[nodiscard]] bool isGamePause() const { return gamePause; }
    void setGamePause(bool value) { gamePause = value; }

    // Getter and setter for noSolution
    [[nodiscard]] bool isNoSolution() const { return noSolution; }
    void setNoSolution(bool value) { noSolution = value; }

    // Getter and setter for remainTime
    [[nodiscard]] int getRemainTime() const { return remainTime; }
    void setRemainTime(int value) { remainTime = value; }

    // Getter and setter for gameType
    [[nodiscard]] int getGameType() const { return gameType; }
    void setGameType(int value) { gameType = value; }

    // Getter and setter for typeToColor
    [[nodiscard]] QVector<QColor> getTypeToColor() const { return typeToColor; }
    void setTypeToColor(QVector<QColor> value) { typeToColor = value; }

    // Getter and setter for boxType
    [[nodiscard]] QVector<QVector<int>> getBoxType() const { return boxType; }
    void setBoxType(QVector<QVector<int>> value) { boxType = value; }

    // Getter and setter for boxMap
    [[nodiscard]] QVector<QVector<BoxOfGame *>> getBoxMap() const { return boxMap; }
    void setBoxMap(QVector<QVector<BoxOfGame *>> value) { boxMap = value; }

    // Getter and setter for removeText
    [[nodiscard]] QString getRemoveText() const { return removeText; }
    void setRemoveText(QString value) { removeText = value; }

    // Getter and setter for leftTimeText
    [[nodiscard]] QString getLeftTimeText() const { return leftTimeText; }
    void setLeftTimeText(QString value) { leftTimeText = value; }

    // Getter and setter for summaryText
    [[nodiscard]] QString getSummaryText() const { return summaryText; }
    void setSummaryText(QString value) { summaryText = value; }

    // Getter and setter for types
    [[nodiscard]] int getTypes() const { return types; }
    void setTypes(int value) { types = value; }

    // Getter and setter for maxGadgetNumber
    [[nodiscard]] int getMaxGadgetNumber() const { return maxGadgetNumber; }
    void setMaxGadgetNumber(int value) { maxGadgetNumber = value; }

    // Getter and setter for gadgetSummonPossibility
    [[nodiscard]] double getGadgetSummonPossibility() const { return gadgetSummonPossibility; }
    void setGadgetSummonPossibility(double value) { gadgetSummonPossibility = value; }

    // Getter and setter for hintTime
    [[nodiscard]] int getHintTime() const { return hintTime; }
    void setHintTime(int value) { hintTime = value; }

    // Getter and setter for hintTimerOn
    [[nodiscard]] bool isHintTimerOn() const { return hintTimerOn; }
    void setHintTimerOn(bool value) { hintTimerOn = value; }

    // Getter and setter for hintedBoxes
    [[nodiscard]] QVector<QPair<int,int>> getHintedBoxes() const { return hintedBoxes; }
    void setHintedBoxes(QVector<QPair<int,int>> value) { hintedBoxes = value; }

    // Getter and setter for flashTime
    [[nodiscard]] int getFlashTime() const { return flashTime; }
    void setFlashTime(int value) { flashTime = value; }

    // Getter and setter for flashTimerOn
    [[nodiscard]] bool isFlashTimerOn() const { return flashTimerOn; }
    void setFlashTimerOn(bool value) { flashTimerOn = value; }

    // Getter and setter for dizzyTimerOn
    [[nodiscard]] bool isDizzyTimerOn() const { return dizzyTimerOn; }
    void setDizzyTimerOn(bool value) { dizzyTimerOn = value; }

    // Getter and setter for freezeTimerOn
    [[nodiscard]] bool isFreezeTimerOn() const { return freezeTimerOn; }
    void setFreezeTimerOn(bool value) { freezeTimerOn = value; }

    // Getter and setter for player1
    [[nodiscard]] Player * getPlayer1() const { return player1; }
    void setPlayer1(Player * value) { player1 = value; }

    // Getter and setter for player2
    [[nodiscard]] Player * getPlayer2() const { return player2; }
    void setPlayer2(Player * value) { player2 = value; }

public:
    explicit LinkGame(const int& M = 10, const int& N = 10,int gameTypeInput = 0,const int &remainTimeInput = 0 ,QWidget * parent = nullptr); //构造函数
    ~LinkGame();    //析构函数
    void shuffleMap(); //重排地图
    void setGamePauseNoValue(); //设置游戏暂停或继续
    void checkGameEnd(); //检查游戏是否结束
    void initGlobalBox(const int& M, const int& N); //初始化箱子的种类对应颜色，初始化boxMap和boxType大小
public slots:
    void updateRemainTime(); //更新剩余时间
    void updateHintTime(); //更新hint时间
    void updateFlashTime(); //更新flash时间
    void updateFreezeTime(); //更新freeze时间
    void updateDizzyTime(); //更新dizzy时间
    void startNewGame(); //开始新游戏
    void loadGame(); //加载游戏
    void showSavePage();
    void showPausePage(); //显示暂停页面
    void initMap(); //初始化地图
private:
    void paintEvent(QPaintEvent *event) override; //绘制事件
    void resizeEvent(QResizeEvent *event) override; //重绘事件
    void keyPressEvent(QKeyEvent * event) override; //键盘事件
    void keyReleaseEvent(QKeyEvent * event) override; //键盘释放事件
    void mousePressEvent(QMouseEvent * event) override; //鼠标点击事件
    void drawMap(QPainter & painter); //绘制地图
    void drawEndPage(QPainter & painter); //绘制结束页面
    void drawPausePage(QPainter & painter); //绘制暂停页面
    //初始化箱子
    void initTimers(const int &remainTimeInput); //初始化计时器
    void initBoxType(); //初始化箱子类型
    void initPlayer(int gameTypeInput); //初始化玩家
    void gameUpdate(); //游戏更新
    void clearClose(); //消除相邻的两个方块
    void generateGadget(); //生成道具
    void hintBox(); //提示箱子
    void clearHintBox(); //清除提示箱子
    void setEndText(); //设置结束页面文本
    void addToEndPageLayout(QGridLayout *layout,QPushButton *newGameButton,QPushButton *loadButton,QPushButton *exitButton,QLabel * endLabel,QLabel * player1Score,QLabel * player2Score);//添加到结束页面布局


    int gameFps; //游戏帧率
    int remainBoxNumber; //剩余箱子数量
    int boxCol; //箱子列数
    int boxRow; //箱子行数
    int boxWidth; //箱子宽度
    int boxHeight; //箱子高度
    int passageWidth; //通道宽度
    int passageHeight; //通道高度
    double xScaleRatio; //x轴缩放比例
    double yScaleRatio; //y轴缩放比例
    bool gameEnd; //游戏是否结束
    bool gamePause; //游戏是否暂停
    bool noSolution; //无解
    int remainTime; //剩余时间
    int gameType;//0是单人游戏，1是双人游戏

    QVector<QColor> typeToColor; //各个类型对应箱子颜色
    QVector<QVector<int>> boxType; //箱子类型
    QVector<QVector<BoxOfGame *>> boxMap; //箱子地图
    QString removeText; //消除提示字符串
    QString leftTimeText; //剩余时间提示字符串
    QString summaryText; //总结字符串
    int types; //箱子类型数

    QTimer * remainTimeTimer; //剩余时间计时器
    QTimer * flashTimeTimer; //flash计时器
    QTimer * hintTimeTimer; //hint计时器
    QTimer * freezeTimeTimer; //freeze计时器
    QTimer * dizzyTimeTimer; //dizzy计时器

    QVector<Gadget *> gadgets; //道具
    int maxGadgetNumber; //最大道具数量
    double gadgetSummonPossibility; //道具生成概率

    int hintTime; //提示时间
    bool hintTimerOn; //提示计时器是否开启
    QVector<QPair<int,int>>hintedBoxes; //提示箱子

    int flashTime; //flash时间
    bool flashTimerOn; //flash计时器是否开启

    bool dizzyTimerOn; //dizzy计时器是否开启
    bool freezeTimerOn; //freeze计时器是否开启

    QStackedLayout * stackedLayout; //堆叠布局

    Player * player1; //玩家1
    Player * player2; //玩家2
};

