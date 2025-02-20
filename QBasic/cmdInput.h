//
// Created by overji on 2024/12/2.
//

#pragma once
#include "CodesInCodeDisplay.h"
#include "mainwindow.h"

class MainWindow;

class cmdInput {
public:
    static std::pair<int,QString> parseCodeInput(const QString& cmd);
    static commands parseCommandInput(const QString& cmd);
    static void parseDirectCode(const QString& cmd,MainWindow * mainWindow);
};
