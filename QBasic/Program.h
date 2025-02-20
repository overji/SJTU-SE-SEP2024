//
// Created by overji on 2024/12/4.
//

#pragma once
#include "Statement.h"
#include "mainwindow.h"
#include <QWidget>

enum programLineType{
    PRO_LET,
    PRO_PRINT,
    PRO_INPUT,
    PRO_GOTO,
    PRO_IFTHEN,
    PRO_REM,
    PRO_END
};
class Statement;
class Program;
class MainWindow;

class ProgramLine{
public:
    explicit ProgramLine(int line, QString str,Program * program);

    programLineType type;
    int lineNumber;
    Statement * statement;
};

class Program {

public:
    Program();

    Program(MainWindow *mainWindow);

    void insertCodePair(std::pair<int, QString> pair);

    void printProgram();
    QString getOriginProgram();
    bool indexExists(int target);
    void updateOrDeleteLine(const std::pair<int, QString> &pairInput);
    void clearAll();

    void setVariable(QString name, int value);
    Variable* getVariable(QString name);

    QStringList getVariableNames();
    MainWindow * getMainWindow();

    void jumpToLine(int target);
    int run(int isResume = 0);
    ProgramLine * getLine(int target);
    ProgramLine * getLineFromIndex(int index);
    int getIndexFromLineNum(int lineNum);

    void setProgramInputPaused(bool paused);
    bool getProgramInputPaused();

    void setBreak(int target);
    void deleteBreak(int target);

    QString getSyntaxTree();
    QString getVariableMonitor();
    QString getBreakPointsMonitor();

    bool isBreakPausing();
    void resumeFromBreak();

    bool isRunning();

    void clearBreak();

private:
    QVector<ProgramLine> codeLines;
    QVector<std::pair<int, QString>> originProgram;
    QVector<Variable*> variables;
    QVector<int>breakPoints;
    MainWindow * mainWindow;
    int currentLine; //行号编码
    int currentIndex; //行号索引
    bool jmpExecuted;
    bool programInputPaused;
    bool programBreakPaused;
    bool running;
    ProgramLine * currentProgramLine;

    bool isBreak(int lineNum);
};