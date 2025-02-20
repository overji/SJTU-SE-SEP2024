//
// Created by overji on 2024/12/2.
//

#include "CodesInCodeDisplay.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <QRegularExpression>
#include <QFile>
#include "cmdInput.h"

CodesInCodeDisplay::CodesInCodeDisplay(MainWindow * mainWindowUi){
    //初始化代码显示
    this->ui = mainWindowUi->getUi();
    this->codeDisplay = ui->CodeDisplay;
    this->program = mainWindowUi->getProgram();
    this->text = "";
}

QString CodesInCodeDisplay::getText() {
    //获取代码显示框中的文本
    return this->codeDisplay->toPlainText();
}

void CodesInCodeDisplay::updateCodeLines() {
    //更新代码行
    this->clearAll();
    this->text = this->getText();
    this->codeLines = this->text.split('\n');
    try{
        for(const QString& line:this->codeLines)
        {
            if(line.isEmpty()){
                continue;
            }
            auto pr = cmdInput::parseCodeInput(line);
            insertCodePair(pr);
        }
    } catch (QString e) {
        throw e;
    }
}

void CodesInCodeDisplay::insertCodePair(std::pair<int, QString> pr) {
    //插入代码行
    if(this->program == nullptr){
        return;
    }
    this->program->insertCodePair(pr);
}

QString CodesInCodeDisplay::getPlainCode() {
    //获取代码显示框中的文本
    if(this->program == nullptr){
        return "";
    }
    return this->program->getOriginProgram();
}

bool CodesInCodeDisplay::indexExists(int target) {
    //判断索引是否存在
    if(this->program == nullptr){
        return false;
    }
    return this->program->indexExists(target);
}

void CodesInCodeDisplay::clearAll() {
    //清空代码显示框
    this->program->clearAll();
    this->text = "";
    this->codeLines.clear();
}

void CodesInCodeDisplay::updateOrDeleteLine(const std::pair<int, QString>& pairInput) {
    //更新或删除代码行
    try{
        this->program->updateOrDeleteLine(pairInput);
    } catch (QString e) {
        throw e;
    }
}

void CodesInCodeDisplay::printProgram() {
    //打印程序
    this->program->printProgram();
}
