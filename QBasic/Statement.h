//
// Created by overji on 2024/12/4.
//

#pragma once
#include <QWidget>
#include "Variable.h"
#include "Expression.h"
#include "Program.h"

class Expression;
class Program;

enum statementType{
    STA_REM,
    STA_LET,
    STA_PRINT,
    STA_INPUT,
    STA_GOTO,
    STA_IF,
    STA_END,
};

enum statementNodeType{
    STANODE_BASE,
    STANODE_VAR,
    STANODE_EXP,
    STANODE_COM,
    STANODE_JMPTARGET
};

enum compareType{
    COM_GREAT,
    COM_LESS,
    COM_EQUAL
};

struct StatementNode{   //StatementNode是Statement的基本单元，用于构建语法树
    statementType type;        //用于区分Statement的类型
    statementNodeType nodeType;     //用于区分StatementNode的类型
    QString varName;             //nodeType = STANODE_VAR时设定，仅在STA_LET和STA_PRINT和STA_INPUT中使用
    Expression * expression;    //nodeType = STANODE_EXP时设定，仅在STA_LET和STA_PRINT和STA_INPUT和STA_IF中使用
    compareType compType;       //nodeType = STANODE_COM时设定，仅在STA_IF中使用
    QString str;                //nodeType = STANODE_BASE时设定，仅在STA_REM中使用
    int jumpTarget;             //nodeType = STANODE_JMPTARGET时设定，仅在STA_GOTO和STA_IF中使用
    QVector<StatementNode*>children;  //用于存储子节点
    explicit StatementNode(statementType typeInput,statementNodeType staNodeTypeInput, const QString &varInput = "", Expression * expInput = nullptr, compareType compTypeInput = COM_EQUAL, int jumpTargetInput = -1):type(typeInput),nodeType(staNodeTypeInput),varName(varInput),expression(expInput),compType(compTypeInput),jumpTarget(jumpTargetInput),str(""){
    }
};

class Statement {
public:
    virtual ~Statement() = default; // Define the virtual destructor here
    virtual void buildStatement(QString str) = 0; // Pure virtual function
    virtual QString printStatement() = 0;
    virtual void run() = 0;
protected:
    Program * program;

};

class LetStatement : public Statement {
public:
    LetStatement(const QString& str,Program * program);
    void buildStatement(QString str) override; // Implement the pure virtual function
    QString printStatement() override;
    void run() override;
private:
    StatementNode* root;
    void buildLetStatement(const QString& str);
};

class PrintStatement : public Statement {
public:
    PrintStatement(const QString& str,Program * program);
    void buildStatement(QString str) override; // Implement the pure virtual function
    QString printStatement() override;
    void run() override;
private:
    StatementNode* root;
    void buildPrintStatement(const QString& str);
};

class InputStatement : public Statement {
public:
    InputStatement(const QString& str,Program * program);
    void buildStatement(QString str) override; // Implement the pure virtual function
    QString printStatement() override;
    void run() override;
private:
    StatementNode* root;
    void buildInputStatement(const QString& str);
};

class GotoStatement : public Statement {
public:
    explicit GotoStatement(const QString& str,Program * program);
    void buildStatement(QString str) override; // Implement the pure virtual function
    QString printStatement() override;
    void run() override;
private:
    StatementNode* root;
    void buildGotoStatement(const QString& str);
};

class IFTHENStatement : public Statement {
public:
    explicit IFTHENStatement(const QString& str,Program * program);
    void buildStatement(QString str) override; // Implement the pure virtual function
    QString printStatement() override;
    void run() override;
private:
    StatementNode* root;
    void buildIFTHENStatement(const QString& str);
};

class RemStatement : public Statement {
public:
    explicit RemStatement(const QString& str,Program * program);
    void buildStatement(QString str) override; // Implement the pure virtual function
    QString printStatement() override;
    void run() override;
private:
    StatementNode* root;
    void buildRemStatement(const QString& str);
};