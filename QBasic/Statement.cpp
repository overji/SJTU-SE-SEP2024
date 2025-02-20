#include "Statement.h"
#include "util.h"
#include <QRegularExpression>
#include <QWidget>
#include <cassert>
#include <iostream>

// Remove #pragma once directive

LetStatement::LetStatement(const QString& str, Program * program) {
    //根据输入构造LetStatement
    this->program = program;
    this->root = nullptr;
    try {
        buildLetStatement(str);
    } catch (QString e) {
        throw e;
    }
}

void LetStatement::buildStatement(QString str) {
    //包裹函数，用于调用buildLetStatement
    try{
        buildLetStatement(str);
    } catch (QString e){
        throw e;
    }
}

void LetStatement::buildLetStatement(const QString& str) {
    //根据输入构造LetStatement
    QRegularExpression re(R"(^\s*LET\s+([a-zA-Z0-9_]+)\s*=\s*(.+)\s*$)");
    QRegularExpressionMatch match = re.match(str);
    if (!match.hasMatch()) {
        throw QString("Invalid Statement");
    }
    try{
        QString variable = match.captured(1);
        QString exp = match.captured(2);
        this->root = new StatementNode(STA_LET, STANODE_BASE);
        auto varStatementNode = new StatementNode(STA_LET, STANODE_VAR);
        program->setVariable(variable, 0);
        varStatementNode->varName = variable;
        auto expStatement = new Expression(exp, this->program);
        auto expStatementNode = new StatementNode(STA_LET, STANODE_EXP);
        expStatementNode->expression = expStatement;
        this->root->children.push_back(varStatementNode);
        this->root->children.push_back(expStatementNode);
    } catch (QString e){
        throw e;
    }
}

QString LetStatement::printStatement() {
    //打印LetStatement
    QString ans = "";
    assert(this->root != nullptr);
    assert(this->root->children.size() == 2);
    ans += "LET =\n";
    ans += "    ";
    assert(this->root->children[0]->nodeType == STANODE_VAR);
    ans += this->root->children[0]->varName;
    ans += '\n';
    assert(this->root->children[1]->nodeType == STANODE_EXP);
    ans += this->root->children[1]->expression->levelTraverseExpressTree(1);
    return ans;
}

void LetStatement::run() {
    //运行LetStatement
    std::cout << "Now running LetStatement" << std::endl;
    assert(this->root != nullptr);
    assert(this->root->children.size() == 2);
    assert(this->root->children[0]->nodeType == STANODE_VAR);
    assert(this->root->children[1]->nodeType == STANODE_EXP);
    std::cout << "Variable Name: " << this->root->children[0]->varName.toStdString() << std::endl;
    std::cout << "Expression: " << this->root->children[1]->expression->calculateExpressionTree() << std::endl;
    this->program->setVariable(this->root->children[0]->varName,this->root->children[1]->expression->calculateExpressionTree());
}

PrintStatement::PrintStatement(const QString &str, Program * program) {
    //构造PrintStatement
    this->program = program;
    this->root = nullptr;
    try{
        buildPrintStatement(str);
    } catch (QString e){
        throw e;
    }
}

void PrintStatement::buildStatement(QString str) {
    //包裹函数，用于调用buildPrintStatement
    buildPrintStatement(str);
}

void PrintStatement::buildPrintStatement(const QString &str) {
    //根据输入构造PrintStatement
    QRegularExpression re(R"(^\s*PRINT\s+(.*)\s*$)");
    QRegularExpressionMatch match = re.match(str);
    if(!match.hasMatch()){
        throw QString("Invalid Statement");
    }
    QString exp = match.captured(1);
    this->root = new StatementNode(STA_PRINT,STANODE_BASE);
    auto expStatement = new Expression(exp,this->program);
    auto expStatementNode = new StatementNode(STA_PRINT,STANODE_EXP);
    expStatementNode->expression = expStatement;
    this->root->children.push_back(expStatementNode);
}

QString PrintStatement::printStatement() {
    //打印PrintStatement
    QString ans = "";
    assert(this->root != nullptr);
    assert(this->root->children.size() == 1);
    ans += "PRINT \n";
    assert(this->root->children[0]->nodeType == STANODE_EXP);
    ans += this->root->children[0]->expression->levelTraverseExpressTree(1);
    return ans; // Ensure the function returns a QString
}

void PrintStatement::run() {
    //运行PrintStatement
    std::cout << "Now running PrintStatement" << std::endl;
    assert(this->root != nullptr);
    assert(this->root->children.size() == 1);
    assert(this->root->children[0]->nodeType == STANODE_EXP);
    assert(this->program->getMainWindow() != nullptr);
    QString currentResult = this->program->getMainWindow()->getResult();
    this->program->getMainWindow()->updateResult(currentResult + QString::number(this->root->children[0]->expression->calculateExpressionTree()) + "\n" );
}

InputStatement::InputStatement(const QString &str, Program * program) {
    //构造InputStatement
    this->program = program;
    this->root = nullptr;
    try{
        buildInputStatement(str);
    } catch (QString e){
        throw e;
    }
}

void InputStatement::buildStatement(QString str) {
    //包裹函数，用于调用buildInputStatement
    buildInputStatement(str);
}

void InputStatement::buildInputStatement(const QString &str) {
    //根据输入构造InputStatement
    QRegularExpression re(R"(^\s*INPUT\s+([a-zA-Z0-9_]+)\s*$)");
    QRegularExpressionMatch match = re.match(str);
    if(!match.hasMatch()){
        throw QString("Invalid Statement");
    }
    QString variable = match.captured(1);
    if(!checkVarNameValidity(variable)){
        throw QString("Invalid Variable Name ") + variable;
    }
    this->root = new StatementNode(STA_INPUT,STANODE_BASE);
    auto varStatementNode = new StatementNode(STA_INPUT,STANODE_VAR);
    varStatementNode->varName = variable;
    this->root->children.push_back(varStatementNode);
}

QString InputStatement::printStatement() {
    //打印InputStatement
    QString ans = "";
    assert(this->root != nullptr);
    assert(this->root->children.size() == 1);
    ans += "INPUT =\n";
    ans += "    ";
    assert(this->root->children[0]->nodeType == STANODE_VAR);
    ans += this->root->children[0]->varName;
    return ans;
}

void InputStatement::run() {
    //运行InputStatement
    std::cout << "Now running InputStatement" << std::endl;
    assert(this->root != nullptr);
    assert(this->root->children.size() == 1);
    assert(this->root->children[0]->nodeType == STANODE_VAR);
    assert(this->program->getMainWindow() != nullptr);
    if(program->getVariable(this->root->children[0]->varName) == nullptr){
        throw QString("Unfound Variable While Input ") + this->root->children[0]->varName;
    }
    this->program->getMainWindow()->setUIForInputMode(true,program->getVariable(this->root->children[0]->varName));
}

GotoStatement::GotoStatement(const QString &str, Program * program) {
    //构造GotoStatement
    this->program = program;
    this->root = nullptr;
    try{
        buildGotoStatement(str);
    } catch (QString e){
        throw e;
    }
}

void GotoStatement::buildGotoStatement(const QString &str) {
    //根据输入构造GotoStatement
    QRegularExpression re(R"(^\s*GOTO\s+(\d+)\s*$)");
    QRegularExpressionMatch match = re.match(str);
    if(!match.hasMatch()){
        throw QString("Invalid Statement");
    }
    int target = match.captured(1).toInt();
    this->root = new StatementNode(STA_GOTO,STANODE_BASE);
    auto targetStatementNode = new StatementNode(STA_GOTO,STANODE_JMPTARGET);
    targetStatementNode->jumpTarget = target;
    this->root->children.push_back(targetStatementNode);
}

void GotoStatement::buildStatement(QString str) {
    buildGotoStatement(str);
}

QString GotoStatement::printStatement() {
    //打印GotoStatement
    QString ans = "";
    assert(this->root != nullptr);
    assert(this->root->children.size() == 1);
    ans += "GOTO =\n";
    ans += "    ";
    assert(this->root->children[0]->nodeType == STANODE_JMPTARGET);
    ans += QString::number(this->root->children[0]->jumpTarget);
    return ans;
}

void GotoStatement::run() {
    //运行GotoStatement
    std::cout << "Now running GotoStatement" << std::endl;
    assert(this->root != nullptr);
    assert(this->root->children.size() == 1);
    assert(this->root->children[0]->nodeType == STANODE_JMPTARGET);
    this->program->jumpToLine(this->root->children[0]->jumpTarget);
}

IFTHENStatement::IFTHENStatement(const QString &str, Program * program) {
    //构造IFTHENStatement
    this->program = program;
    this->root = nullptr;
    try{
        buildIFTHENStatement(str);
    } catch (QString e){
        throw e;
    }
}

void IFTHENStatement::buildIFTHENStatement(const QString &str) {
    //根据输入构造IFTHENStatement
    QRegularExpression re(R"(^\s*IF\s+([a-zA-Z0-9_+-/*\s\(\)]+)\s*(=|<|>)\s*([a-zA-Z0-9_+-/*\s\(\)]+)\s+THEN\s+(\d+)\s*$)");
    QRegularExpressionMatch match = re.match(str);
    if(!match.hasMatch()){
        throw QString("Invalid Statement");
    }
    QString exp1 = match.captured(1);
    QString exp2 = match.captured(3);
    QString comp = match.captured(2);
    int target = match.captured(4).toInt();
    this->root = new StatementNode(STA_IF,STANODE_BASE);
    auto exp1Statement = new Expression(exp1,this->program);
    auto exp1StatementNode = new StatementNode(STA_IF,STANODE_EXP);
    exp1StatementNode->expression = exp1Statement;
    auto exp2Statement = new Expression(exp2,this->program);
    auto exp2StatementNode = new StatementNode(STA_IF,STANODE_EXP);
    exp2StatementNode->expression = exp2Statement;
    auto compStatementNode = new StatementNode(STA_IF,STANODE_COM);
    if(comp == "="){
        compStatementNode->compType = COM_EQUAL;
    } else if(comp == "<"){
        compStatementNode->compType = COM_LESS;
    } else if(comp == ">"){
        compStatementNode->compType = COM_GREAT;
    } else {
        throw QString("Invalid Statement");
    }
    auto targetStatementNode = new StatementNode(STA_IF,STANODE_JMPTARGET);
    targetStatementNode->jumpTarget = target;
    this->root->children.push_back(exp1StatementNode);
    this->root->children.push_back(compStatementNode);
    this->root->children.push_back(exp2StatementNode);
    this->root->children.push_back(targetStatementNode);
}

void IFTHENStatement::buildStatement(QString str) {
    //包裹函数，用于调用buildIFTHENStatement
    buildIFTHENStatement(str);
}

QString IFTHENStatement::printStatement() {
    //打印IFTHENStatement
    QString ans = "";
    assert(this->root != nullptr);
    assert(this->root->children.size() == 4);
    ans += "IF THEN\n";
    assert(this->root->children[0]->nodeType == STANODE_EXP);
    ans += this->root->children[0]->expression->levelTraverseExpressTree(1);
    if(this->root->children[1]->compType == COM_EQUAL){
        ans += "    =\n";
    } else if(this->root->children[1]->compType == COM_LESS){
        ans += "    <\n";
    } else if(this->root->children[1]->compType == COM_GREAT){
        ans += "    >\n";
    }
    assert(this->root->children[2]->nodeType == STANODE_EXP);
    ans += this->root->children[2]->expression->levelTraverseExpressTree(1);
    ans += "    ";
    assert(this->root->children[3]->nodeType == STANODE_JMPTARGET);
    ans += QString::number(this->root->children[3]->jumpTarget);
    return ans;
}

void IFTHENStatement::run() {
    //运行IFTHENStatement
    std::cout << "Now running IFTHENStatement" << std::endl;
    assert(this->root != nullptr);
    assert(this->root->children.size() == 4);
    assert(this->root->children[0]->nodeType == STANODE_EXP);
    assert(this->root->children[1]->nodeType == STANODE_COM);
    assert(this->root->children[2]->nodeType == STANODE_EXP);
    assert(this->root->children[3]->nodeType == STANODE_JMPTARGET);
    int exp1 = this->root->children[0]->expression->calculateExpressionTree();
    int exp2 = this->root->children[2]->expression->calculateExpressionTree();
    std::cout << "exp1 = " << exp1 << " exp2 = " << exp2 << std::endl;
    if(this->root->children[1]->compType == COM_EQUAL){
        if(exp1 == exp2){
            this->program->jumpToLine(this->root->children[3]->jumpTarget);
        }
    } else if(this->root->children[1]->compType == COM_LESS){
        if(exp1 < exp2){
            this->program->jumpToLine(this->root->children[3]->jumpTarget);
        }
    } else if(this->root->children[1]->compType == COM_GREAT){
        if(exp1 > exp2){
            this->program->jumpToLine(this->root->children[3]->jumpTarget);
        }
    }
}

RemStatement::RemStatement(const QString &str, Program * program) {
    //构造RemStatement
    this->program = program;
    this->root = nullptr;
    try{
        buildRemStatement(str);
    } catch (QString e){
        throw e;
    }
}

void RemStatement::buildRemStatement(const QString &str) {
    //根据输入构造RemStatement
    QRegularExpression re(R"(^\s*REM(.*))");
    QRegularExpressionMatch match = re.match(str);
    if(!match.hasMatch()){
        throw QString("Invalid Statement");
    }
    this->root = new StatementNode(STA_REM,STANODE_BASE);
    this->root->str = match.captured(1);
}

void RemStatement::buildStatement(QString str) {
    //包裹函数，用于调用buildRemStatement
    buildRemStatement(str);
}

QString RemStatement::printStatement() {
    //该函数不会被调用
    QString ans = "";
    assert(this->root != nullptr);
    ans += "REM\n";
    ans += "    ";
    ans += this->root->str;
    return ans;
}

void RemStatement::run() {
    return;
}

//int main() {
//    try{
//        auto let = new LetStatement("LET a = (((1+3)*7-(2**(4+5))))");
//        std::cout << let->printStatement().toStdString() << std::endl;
//        auto print = new PrintStatement("PRINT 15");
//        std::cout << print->printStatement().toStdString() << std::endl;
//        auto input = new InputStatement("INPUT a");
//        std::cout << input->printStatement().toStdString() << std::endl;
//        auto gotoStatement = new GotoStatement("GOTO 114514");
//        std::cout << gotoStatement->printStatement().toStdString()<< std::endl;
//        auto ifThen = new IFTHENStatement("IF a*10 = b+c**114514 THEN 1919810");
//        std::cout << ifThen->printStatement().toStdString() << std::endl;
//    } catch (QString e) {
//        std::cout << e << std::endl;
//    }
//    return 0;
//}
