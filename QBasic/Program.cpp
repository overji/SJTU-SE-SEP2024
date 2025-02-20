//
// Created by overji on 2024/12/4.
//

#include "Program.h"
#include "mainwindow.h"
#include <QWidget>
#include <QRegularExpression>
#include <algorithm>
#include <iostream>

ProgramLine::ProgramLine(int line, QString str,Program * program) {
    //初始化程序行
    this->lineNumber = line;
    QRegularExpression letRe(R"(^\s*LET\s+([a-zA-Z0-9_]+)\s*=\s*(.+)\s*$)");
    QRegularExpressionMatch letMatch = letRe.match(str);
    try{
        if (letMatch.hasMatch()) {
            //如果是LET语句，就新建一个LET语句
            program->setVariable(letMatch.captured(1), 0);
            this->statement = new LetStatement(str,program);
            this->type = PRO_LET;
            return;
        }
        QRegularExpression printRe(R"(^\s*PRINT\s+(.*)\s*$)");
        QRegularExpressionMatch printMatch = printRe.match(str);
        if (printMatch.hasMatch()) {
            //如果是PRINT语句，就新建一个PRINT语句
            this->statement = new PrintStatement(str,program);
            this->type = PRO_PRINT;
            return;
        }
        QRegularExpression inputRe(R"(^\s*INPUT\s+([a-zA-Z0-9_]+)\s*$)");
        QRegularExpressionMatch inputMatch = inputRe.match(str);
        if (inputMatch.hasMatch()) {
            //如果是INPUT语句，就新建一个INPUT语句
            this->statement = new InputStatement(str,program);
            this->type = PRO_INPUT;
            return;
        }
        QRegularExpression gotoRe(R"(^\s*GOTO\s+(\d+)\s*$)");
        QRegularExpressionMatch gotoMatch = gotoRe.match(str);
        if (gotoMatch.hasMatch()) {
            //如果是GOTO语句，就新建一个GOTO语句
            this->statement = new GotoStatement(str,program);
            this->type = PRO_GOTO;
            return;
        }
        QRegularExpression ifthenRe(R"(^\s*IF\s+([a-zA-Z0-9_+-/*\s\(\)]+)\s*(=|<|>)\s*([a-zA-Z0-9_+-/*\s\(\)]+)\s+THEN\s+(\d+)\s*$)");
        QRegularExpressionMatch ifthenMatch = ifthenRe.match(str);
        if (ifthenMatch.hasMatch()) {
            //如果是IF THEN语句，就新建一个IF THEN语句
            this->statement = new IFTHENStatement(str,program);
            this->type = PRO_IFTHEN;
            return;
        }
        QRegularExpression remRe(R"(^\s*REM(.*))");
        QRegularExpressionMatch remMatch = remRe.match(str);
        if (remMatch.hasMatch()) {
            //如果是REM语句，就新建一个REM语句
            this->statement = new RemStatement(str,program);
            this->type = PRO_REM;
            return;
        }
        QRegularExpression endRe(R"(^\s*END\s*$)");
        QRegularExpressionMatch endMatch = endRe.match(str);
        if (endMatch.hasMatch()) {
            //如果是END语句，就新建一个END语句
            this->statement = nullptr;
            this->type = PRO_END;
            return;
        }
        throw QString("Invalid Statement");
    } catch (QString e) {
        throw e;
    }
}

Program::Program() {
    //初始化程序
    this->running = false;
    this->codeLines = QVector<ProgramLine>();
    this->originProgram = QVector<std::pair<int, QString>>();
    this->mainWindow = nullptr;
    this->variables = QVector<Variable*>();
    this->currentLine = 0;
    this->currentProgramLine = nullptr;

    this->currentIndex = 0;
    this->programInputPaused = false;
    this->jmpExecuted = false;
}

Program::Program(MainWindow * mainWindow) {
    //初始化程序，这个构造函数会把主窗口指针传入
    this->mainWindow = mainWindow;
    this->codeLines = QVector<ProgramLine>();
    this->originProgram = QVector<std::pair<int, QString>>();
    this->variables = QVector<Variable*>();
    this->currentLine = 0;
    this->currentProgramLine = nullptr;

    this->currentIndex = 0;
}

void Program::insertCodePair(std::pair<int, QString> pair) {
    //插入代码行
    try {
        ProgramLine programLine(pair.first, pair.second,this);
        this->codeLines.push_back(programLine);
        std::sort(this->codeLines.begin(), this->codeLines.end(), [](const ProgramLine &a, const ProgramLine &b) {
            return a.lineNumber < b.lineNumber;
        });
        this->originProgram.push_back(pair);
        std::sort(this->originProgram.begin(), this->originProgram.end(), [](const std::pair<int, QString> &a, const std::pair<int, QString> &b) {
            return a.first < b.first;
        });
    } catch (QString e) {
        QString errorInfo = e;
        errorInfo = QString::fromStdString("[ERROR] in Line ") + QString::number(pair.first) +" " + errorInfo;
        throw errorInfo;
    }
}

void Program::printProgram() {
    //打印程序，仅用于调试
    for (auto &codeLine : this->codeLines) {
        if (codeLine.type == PRO_REM) {
            continue;
        }
        if (codeLine.type == PRO_END) {
            std::cout << std::endl;
            continue;
        }
        std::cout << codeLine.lineNumber << " ";
        if (codeLine.statement != nullptr) {
            std::cout << codeLine.statement->printStatement().toStdString();
        } else {
            std::cout << std::endl;
        }
    }
}

QString Program::getOriginProgram() {
    //获取原始程序，便于展示程序
    QString result;
    for (const auto &pr : this->originProgram) {
        result += QString::number(pr.first) + " " + pr.second + "\n";
    }
    return result;
}

bool Program::indexExists(int target) {
    //判断某一行是否存在，注意这个是根据lineNumber进行的判断
    for (auto pr = this->codeLines.begin(); pr != this->codeLines.end(); pr++) {
        if (pr->lineNumber == target) {
            return true;
        }
    }
    return false;
}

void Program::updateOrDeleteLine(const std::pair<int, QString> &pairInput) {
    //更新或删除某一行
    try{
        if (pairInput.second.isEmpty()) {
            this->codeLines.erase(std::remove_if(this->codeLines.begin(), this->codeLines.end(),
                                                 [pairInput](auto &item) {
                                                     return item.lineNumber == pairInput.first;
                                                 }), this->codeLines.end());
            this->originProgram.erase(std::remove_if(this->originProgram.begin(), this->originProgram.end(),
                                                     [pairInput](auto &item) {
                                                         return item.first == pairInput.first;
                                                     }), this->originProgram.end());
        } else {
            for (auto &pr : codeLines) {
                if (pr.lineNumber == pairInput.first) {
                    pr = ProgramLine(pairInput.first, pairInput.second,this);
                }
            }
            for (int i = 0; i < originProgram.size(); ++i) {
                if (originProgram[i].first == pairInput.first) {
                    originProgram[i].second = pairInput.second;
                }
            }
        }
    } catch (QString e) {
        throw e;
    }
}

void Program::clearAll() {
    //清空程序
    this->variables.clear();
    this->codeLines.clear();
    this->originProgram.clear();
}

Variable *Program::getVariable(QString name) {
    //获取变量
    for (auto &variable : this->variables) {
        if (variable->getName() == name) {
            return variable;
        }
    }
    return nullptr;
}

void Program::setVariable(QString name, int value) {
    //如果变量已经存在，就更新变量的值,否则新建一个变量
    for (auto &variable : this->variables) {
        if (variable->getName() == name) {
            variable->setValue(value);
            return;
        }
    }
    auto var = new Variable(name);
    var->setValue(value);
    this->variables.push_back(var);
}

QStringList Program::getVariableNames() {
    //获取所有变量的名字
    QStringList result;
    for (auto &variable : this->variables) {
        result.push_back(variable->getName());
    }
    return result;
}

MainWindow *Program::getMainWindow() {
    //获取主窗口指针
    return this->mainWindow;
}

void Program::jumpToLine(int target) {
    //跳转到某一行
    this->jmpExecuted = true;
    this->currentLine = target;
}

ProgramLine *Program::getLine(int target) {
    //获取某一行，根据lineNumber
    for(auto &codeLine:this->codeLines){
        if(codeLine.lineNumber == target){
            return &codeLine;
        }
    }
    return nullptr;
}

ProgramLine *Program::getLineFromIndex(int index) {
    //获取某一行，根据index
    if(index < 0 || index >= this->codeLines.size()){
        return nullptr;
    }
    return &this->codeLines[index];
}

int Program::getIndexFromLineNum(int lineNum) {
    //获取某一行的index
    for(int i = 0; i < this->codeLines.size(); i++){
        if(this->codeLines[i].lineNumber == lineNum){
            return i;
        }
    }
    return -1;
}

int Program::run(int isResume) {
    //isResume:0正常启动，1输入恢复，2断点恢复
    if(!isResume){
        this->mainWindow->updateResult("");
        this->jmpExecuted = false;
        this->programInputPaused = false;
        this->currentProgramLine = getLineFromIndex(0);
        this->currentIndex = 0;
    }
    while(this->currentProgramLine && this->currentProgramLine->type != PRO_END){
        try{
            if(isResume == 0 && this->mainWindow->getDebugMode() && this->isBreak(this->currentProgramLine->lineNumber)){
                //不是正常启动，表明已经断点过了
                this->programBreakPaused = true;
                this->mainWindow->setMonitor(this->getVariableMonitor());
                this->running = true;
                return 1;
            }
            if(isResume != 1){
                //不是输入回复，表明这一行已经运行过了
                this->currentProgramLine->statement->run();
            }
            isResume = 0;
            if(this->programInputPaused){
                //如果输入暂停，就暂停程序
                this->mainWindow->setCmdLineEditInputMode();
                this->running = true;
                return 1;
            }
            if(this->jmpExecuted){
                //如果跳转了，就跳转到指定行
                this->jmpExecuted = false;
                this->currentProgramLine = getLine(this->currentLine);
                this->currentIndex = getIndexFromLineNum(this->currentLine);
                if(this->currentProgramLine == nullptr){
                    throw QString("The program tries to jump to an invalid line number in ") + QString::number(this->currentLine);
                }
            } else {
                //否则就继续执行下一行
                this->currentIndex++;
                this->currentProgramLine = getLineFromIndex(this->currentIndex);
                if(this->currentProgramLine == nullptr){
                    this->running = false;
                    return -2;
                }
                this->currentLine = this->currentProgramLine->lineNumber;
            }
            if(this->currentProgramLine->type == PRO_END || this->currentIndex >= this->codeLines.size()){
                //如果到了程序末尾，就结束程序
                std::cout << (this->currentProgramLine->type == PRO_END )<< std::endl;
                std::cout << "Program End" << std::endl;
                this->running = false;
                return 0;
            }
        } catch (QString e){
            //如果出现错误，就更新结果
            this->mainWindow->updateResult(e);
            this->running = false;
            return -1;
        }
    }
}

void Program::setProgramInputPaused(bool paused) {
    //设置输入暂停
    this->programInputPaused = paused;
}

bool Program::getProgramInputPaused() {
    //获取输入暂停状态
    return this->programInputPaused;
}

QString Program::getSyntaxTree() {
    //获取语法树
    QString syntaxTree = "";
    for(auto &codeLine:this->codeLines){
        if(codeLine.statement != nullptr && codeLine.type != PRO_END){
            syntaxTree += QString::number(codeLine.lineNumber);
            syntaxTree += " ";
            syntaxTree += codeLine.statement->printStatement();
            syntaxTree += "\n";
        }
    }
    return syntaxTree;
}

QString Program::getVariableMonitor() {
    //获取变量监视器
    QString ans = "Now in Line ";
    ans += QString::number(this->currentProgramLine->lineNumber) + " :\n";
    for(auto var:variables){
        ans += var->getName();
        ans += "=";
        ans += QString::number(var->getValue());
        ans += "\n";
    }
    return ans;
}

QString Program::getBreakPointsMonitor() {
    //获取断点监视器
    QString ans = "";
    std::sort(breakPoints.begin(),breakPoints.end());
    for(auto i:breakPoints){
        ans += QString::number(i) + "\n";
    }
    return ans;
}

void Program::setBreak(int target) {
    //设置断点
    auto line = this->getLine(target);
    if((!this->indexExists(target)) || line == nullptr){
        throw QString("Cannot set break on Line ") + QString::number(target) + QString(" !");
    }
    if(line->type == PRO_REM){
        return;
    }
    this->breakPoints.push_back(target);
}

void Program::deleteBreak(int target) {
    //删除断点
    auto line = this->getLine(target);
    if((!this->indexExists(target)) || (line == nullptr)){
        throw QString("Cannot remove break on Line ") + QString::number(target) + QString(" !");
    }
    this->breakPoints.removeOne(target);
}

bool Program::isBreak(int lineNum){
    //判断某一行是否是断点
    for(auto i : breakPoints){
        if(i == lineNum){
            return true;
        }
    }
    return false;
}

bool Program::isBreakPausing(){
    //判断是否是断点暂停
    return this->programBreakPaused;
}

void Program::resumeFromBreak(){
    //从断点恢复
    this->programBreakPaused = false;
}

bool Program::isRunning() {
    //判断程序是否在运行
    return this->running;
}

void Program::clearBreak() {
    //清空断点
    this->breakPoints.clear();
}
