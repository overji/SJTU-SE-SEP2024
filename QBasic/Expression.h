//
// Created by overji on 2024/12/3.
//
#pragma once
#include <QWidget>
#include <QStack>
#include "Variable.h"
#include "Program.h"

class Program;

enum expressionType{
    EXP_NUM,
    EXP_VAR,
    EXP_NEG,
    EXP_ADD,
    EXP_SUB,
    EXP_MULT,
    EXP_DIV,
    EXP_MOD,
    EXP_EXPON,
    EXP_RBRA
};

struct ExpressionNode{
    expressionType type;
    int number;
    QString varName;
    ExpressionNode * left;
    ExpressionNode * right;
    ExpressionNode(){
        type = EXP_ADD;
        number = -1;
        varName = "";
        left = nullptr;
        right = nullptr;
    }
};

struct assistNode{
    expressionType type;
    int number;
    QString varName;
    explicit assistNode(expressionType typeInput, int numberInput = -1, const QString &varInput = nullptr):type(typeInput),number(numberInput),varName(varInput){
    }
};

class Expression {
public:
    explicit Expression(const QString& expression,Program * program);

    void midFix2preFix(QString str);
    bool checkExpressValidity(QString str);

    void logBuildStack();
    void logExpressTreeHelper(ExpressionNode * rt);
    void logExpressTree();
    QString levelTraverseExpressTree(int level = 0);
    ExpressionNode * buildExpressTreeHelper();

    void buildExpressTree();
    int calculateExpressionTree();

private:
    ExpressionNode * root;
    QStack<assistNode> * buildStack;
    Program * program;

    bool isMod(QString str, int cur);
    void removeSpace(QString &str);
    bool isOperator(QString str, int cur);
    int calculateExpressionTreeRecursion(ExpressionNode * root);
};

