//
// Created by overji on 2024/12/2.
//

#pragma once
#include <QWidget>
#include <QTextBrowser>
#include "Program.h"

class MainWindow;
class Program;

namespace Ui {
    class MainWindow;
}

class CodesInCodeDisplay {
public:
    CodesInCodeDisplay(MainWindow * mainWindowUi);
    QString getText();

    void updateCodeLines();
    void updateOrDeleteLine(const std::pair<int,QString>& pairInput);
    void insertCodePair(std::pair<int,QString>pr);

    QString getPlainCode();
    bool indexExists(int target);
    void printProgram();
    void clearAll();

private:
    Ui::MainWindow * ui; //mainWindow中的ui
    QTextBrowser * codeDisplay; //ui中的代码显示框
    QString text;
    QStringList codeLines;
    Program * program; //程序

};

