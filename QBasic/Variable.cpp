//
// Created by overji on 2024/12/3.
//

#include "Variable.h"

Variable::Variable(const QString &name) {
    //检查变量名是否合法
    this->name = name;
    if(!checkNameValidity(name)){
        throw QString("Invalid Variable Name ") + name;
    }
    this->value = 0;
}

void Variable::setValue(int val) {
    //设置变量值
    this->value = val;
}

int Variable::getValue() const {
    //获取变量值
    return this->value;
}

QString Variable::getName() {
    //获取变量名
    return this->name;
}

bool Variable::checkNameValidity(const QString &name) {
    //检查变量名是否合法
    if (name.isEmpty()) {
        return false;
    }
    if (name[0].isDigit()) {
        return false;
    }
    for (auto c : name) {
        if (!c.isLetterOrNumber() && c != '_' && c != '-') {
            return false;
        }
    }
    //判断关键词INPUT, PRINT, LET, IF, THEN, GOTO, END是否
    if (name == "INPUT" || name == "PRINT" || name == "LET" || name == "IF" || name == "THEN" || name == "GOTO" || name == "END") {
        return false;
    }
    //判断名字中是否含有MOD
    if (name.contains("MOD")) {
        return false;
    }
    return true;
}
