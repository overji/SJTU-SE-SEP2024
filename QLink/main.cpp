#include <QApplication>
#include "LinkGame.h"
#include "MainPage.h"
#include <iostream>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    MainPage mainPage; // 创建主页面
    mainPage.show(); // 显示主页面

    return QApplication::exec(); // 运行程序
}
