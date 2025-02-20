//
// Created by overji on 2024/12/2.
//

#include "cmdInput.h"
#include <QRegularExpression>
#include <iostream>

std::pair<int,QString> cmdInput::parseCodeInput(const QString& cmd){
    //判断输入是否是代码，如果是的话就返回行号+代码内容，否则抛出错误
    QRegularExpression re(R"(^(\s*)([0-9-]+)(\s+)(.*)$)");
    QRegularExpression reEmpty(R"(^(\s*)([0-9-]+)(\s*)$)");
    QRegularExpressionMatch reMatch = re.match(cmd);
    QRegularExpressionMatch reMatchEmpty = reEmpty.match(cmd);
    int ans;
    QString str = "";
    if(reMatch.hasMatch()){
        ans = reMatch.captured(2).toInt();
        str = reMatch.captured(4);
    } else if(reMatchEmpty.hasMatch()) {
        ans = reMatch.captured(2).toInt();
        str = "";
    } else {
        throw QString("Wrong Insert Input! ") + cmd;
    }
    return {ans,str};
}

commands cmdInput::parseCommandInput(const QString &cmd) {
    //判断输入是否是命令，如果是的话就返回对应的命令，否则抛出错误
    QRegularExpression re(R"(^\s*([A-Za-z]+)\s*$)");
    QRegularExpression breakRe(R"(^\s*([A-Za-z]+)\s+([0-9]+)\s*$)");
    QRegularExpressionMatch match = re.match(cmd);
    QRegularExpressionMatch breakMatch = breakRe.match(cmd);
    QString command = "";
    int breakPoint = 0;
    if(match.hasMatch()){
        command = match.captured(1);
        command = command.toUpper();
        std::cout << command.toStdString() << std::endl;
    } else if(breakMatch.hasMatch()) {
        command = breakMatch.captured(1);
    } else {
        throw QString::fromStdString("Wrong Command Input ") + cmd;
    }
    if(command == "RUN"){
        return CMD_RUN;
    } else if(command == "LOAD"){
        return CMD_LOAD;
    } else if(command == "CLEAR"){
        return CMD_CLEAR;
    } else if(command == "HELP"){
        return CMD_HELP;
    } else if(command == "QUIT"){
        return CMD_QUIT;
    } else if(command == "LIST"){
        return CMD_LIST;
    } else if(command == "ADD"){
        return CMD_ADDBREAK;
    } else if(command == "DELETE"){
        return CMD_DELBREAK;
    } else {
        throw QString("Invalid Command!");
    }
}

void cmdInput::parseDirectCode(const QString &cmd, MainWindow *mainWindow) {
    //解析直接输入的代码，如果是LET PRINT INPUT则执行，否则抛出错误
    try{
        QRegularExpression letRe(R"(^\s*LET\s+([a-zA-Z0-9_]+)\s*=\s*(.+)\s*$)");
        QRegularExpressionMatch letMatch = letRe.match(cmd);
        QRegularExpression printRe(R"(^\s*PRINT\s+(.*)\s*$)");
        QRegularExpressionMatch printMatch = printRe.match(cmd);
        QRegularExpression inputRe(R"(^\s*INPUT\s+([a-zA-Z0-9_]+)\s*$)");
        QRegularExpressionMatch inputMatch = inputRe.match(cmd);
        if(letMatch.hasMatch()){
            Expression * exp = new Expression(letMatch.captured(2),mainWindow->program);
            int value = exp->calculateExpressionTree();
            mainWindow->program->setVariable(letMatch.captured(1),value);
        } else if (printMatch.hasMatch()){
            Expression * exp = new Expression(printMatch.captured(1),mainWindow->program);
            int value = exp->calculateExpressionTree();
            mainWindow->updateResult(QString::number(value));
        } else if (inputMatch.hasMatch()){
            mainWindow->isInteractiveInput = true;
            mainWindow->inputVar = mainWindow->program->getVariable(inputMatch.captured(1));
            if(mainWindow->inputVar == nullptr){
                mainWindow->isInteractiveInput = false;
                throw QString("Variable ") + inputMatch.captured(1) + QString(" Not Found!");
            }
        } else {
            throw QString("Invalid Command! ") + cmd;
        }
    } catch (QString e) {
        mainWindow->updateResult(e);
    }
}
