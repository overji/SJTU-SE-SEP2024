//
// Created by overji on 2024/12/3.
//

#pragma once
#include <QWidget>

class Variable {
public:
    explicit Variable(const QString &name = "");
    void setValue(int val = 0);
    int getValue() const;
    QString getName();

private:
    QString name;
    int value;
    bool checkNameValidity(const QString &name);
};

