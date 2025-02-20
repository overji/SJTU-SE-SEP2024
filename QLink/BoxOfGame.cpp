//
// Created by overji on 2024/6/20.
//

#include "BoxOfGame.h"
#include <iostream>
#include <QPainter>

BoxOfGame::BoxOfGame(const int & boxWid , const int & boxHeit,const int & LeftTopX ,const int & LeftTopY  ,const QColor boxColorInput
        , const QColor boarderColorInput,const int TypeOfBoxInput, QWidget * parent): QWidget(parent)
{
    //BoxOfGame的构造函数,boxWid为箱子宽度，boxHeit为箱子高度，LeftTopX为箱子左上角X坐标，LeftTopY为箱子左上角Y坐标,boxColorInput为箱子颜色，boarderColorInput为箱子边框颜色，TypeOfBoxInput为箱子类型
    //确定箱子边框大小、箱子大小、箱子含坐标时候左上角位置
    this->boxWidth = boxWid;
    this->boxHeight = boxHeit;
    this->boarderWidth = boxWidth/20;
    this->boarderHeight = boxHeight/20;
    this->LeftTopX = LeftTopX;
    this->LeftTopY = LeftTopY;
    //箱子颜色、边框颜色、箱子类型
    this->boxColor = boxColorInput;
    this->boarderColor = boarderColorInput;
    this->typeOfBox = TypeOfBoxInput;
    //箱子状态
    this->boxState.boxSelected = false;
    this->boxState.boxToBeRemoved = false;
    this->boxState.closeBox = false;
    this->boxState.boxRemoved = false;
    this->boxState.boxHinted = false;
    this->initBoxPixMap();
}

void BoxOfGame::drawBox(QPainter &painter)
{
    //绘制箱子
    if(boxState.boxRemoved){
        return;
    }
    this->checkBoxBoarderColor();
    //确定箱子含边框的四个角的位置，以及箱子不含边框的四个角的位置
    const QPoint Boarders[4] = {
            QPoint(LeftTopX,LeftTopY),
            QPoint(LeftTopX+boxWidth,LeftTopY),
            QPoint(LeftTopX+boxWidth,LeftTopY+boxHeight),
            QPoint(LeftTopX,LeftTopY+boxHeight)
    };
    const QPoint InnerBox[4] = {
            QPoint(LeftTopX+boarderWidth,LeftTopY+boarderHeight),
            QPoint(LeftTopX+boxWidth-boarderWidth,LeftTopY+boarderHeight),
            QPoint(LeftTopX+boxWidth-boarderWidth,LeftTopY+boxHeight-boarderHeight),
            QPoint(LeftTopX+boarderWidth,LeftTopY+boxHeight-boarderHeight)
    };
    //绘制箱子
    painter.save();
    painter.setPen(Qt::NoPen);
    //绘制箱子含有边框的矩形
    painter.setBrush(boarderColor);
    painter.drawConvexPolygon(Boarders,4);
    //绘制箱子内部不含边框的矩形，以此达到绘制边框的效果
    painter.setBrush(boxColor);
    painter.drawConvexPolygon(InnerBox,4);
    painter.drawPixmap(InnerBox[0].x(),InnerBox[0].y(),boxWidth-2*boarderWidth,boxHeight-2*boarderHeight,boxPixmap);
    painter.restore();
}

void BoxOfGame::resizeBox(const int &boxWid, const int &boxHeit, const int &leftTopX, const int &leftTopY)
{
    //页面大小变换时调整箱子大小,boxWid为箱子宽度，boxHeit为箱子高度，leftTopX为箱子左上角X坐标，leftTopY为箱子左上角Y坐标
    this->boxWidth = boxWid;
    this->boxHeight = boxHeit;
    this->LeftTopX = leftTopX;
    this->LeftTopY = leftTopY;
    this->boarderWidth = boxWid/20;
    this->boarderHeight = boxHeit/20;
}

void BoxOfGame::checkBoxBoarderColor() {
    //检查箱子边框颜色，优先级逐渐递减，为
    // 1. 被选中的箱子
    // 2. 即将被移除的箱子
    // 3. 靠近的箱子
    // 4. 被提示的箱子
    // 5. 其他箱子
    if(boxState.boxSelected){
        boarderColor = QColor(255,0,0);
    }  else if(boxState.boxToBeRemoved) {
        boarderColor = QColor(255,0,0);
    } else if(boxState.closeBox){
        boarderColor = QColor(255,255,0);
    } else if(boxState.boxHinted) {
        boarderColor = QColor(0, 207, 197);
    }else {
        boarderColor = QColor(255, 255, 255);
    }
}

int BoxOfGame::addBoxScore()
{
    //返回箱子分数
    switch(typeOfBox){
        case 0:
            return 105;
        case 1:
            return 105;
        case 2:
            return 120;
        case 3:
            return 105;
        case 4:
            return 115;
        case 5:
            return 100;
        default:
            return 100;
    }
}

void BoxOfGame::swapBox(BoxOfGame * box1, BoxOfGame * box2)
{
    //交换两个箱子的颜色、边框颜色、类型、状态
    std::swap(box1->boxColor,box2->boxColor);
    std::swap(box1->boarderColor,box2->boarderColor);
    std::swap(box1->typeOfBox,box2->typeOfBox);
    std::swap(box1->boxState,box2->boxState);
    std::swap(box1->boxPixmap,box2->boxPixmap);
    //取消箱子的选中状态和靠近状态
    box1->boxState.closeBox = false;
    box2->boxState.closeBox = false;
    box1->boxState.boxSelected = false;
    box2->boxState.boxSelected = false;
}

void BoxOfGame::initBoxPixMap()
{
    //初始化箱子的贴图
    //下面这两行代码可以确定游戏的运行路径，从而确定贴图的位置
    std::filesystem::path curFilePath(__FILE__);
    std::filesystem::path curPath = curFilePath.parent_path();
    //根据不同的箱子类型，赋予不同的贴图
    switch (typeOfBox) {
        case 0:
            boxPixmap = QPixmap(QString::fromStdString(curPath.string() +"\\images\\BoxImages\\morfonica.png"));
            break;
        case 1:
            boxPixmap = QPixmap(QString::fromStdString(curPath.string() +"\\images\\BoxImages\\pp.png"));
            break;
        case 2:
            boxPixmap = QPixmap(QString::fromStdString(curPath.string() +"\\images\\BoxImages\\ppp.png"));
            break;
        case 3:
            boxPixmap = QPixmap(QString::fromStdString(curPath.string() +"\\images\\BoxImages\\r.png"));
            break;
        case 4:
            boxPixmap = QPixmap(QString::fromStdString(curPath.string() +"\\images\\BoxImages\\ras.png"));
            break;
        case 5:
            boxPixmap = QPixmap(QString::fromStdString(curPath.string() +"\\images\\BoxImages\\ag.png"));
            break;
        default:
            boxPixmap = QPixmap(QString::fromStdString(curPath.string() +"\\images\\BoxImages\\hhw.png"));
            break;
    }
}