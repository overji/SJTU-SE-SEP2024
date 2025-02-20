#include "util.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "cmdInput.h"
#include <iostream>
#include <QFileDialog>
#include <QRegularExpression>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    //初始化主界面
    this->isInteractiveInput = false;
    this->isInputMode = false;
    this->isDebugMode = false;
    ui->setupUi(this);
    this->program = new Program(this);
    setUIExitDebugMode();
    this->codesInCodeDisplay = new CodesInCodeDisplay(this);
    this->codesInCodeDisplay->updateCodeLines();
    connect(ui->btnDebugMode, &QPushButton::clicked, this, &MainWindow::setUIForDebugMode);
    connect(ui->btnExitDebugMode, &QPushButton::clicked, this, &MainWindow::setUIExitDebugMode);
    connect(ui->cmdLineEdit,&QLineEdit::editingFinished,this,&MainWindow::on_cmdLineEdit_editingFinished);
    connect(ui->cmdLineEdit,&QLineEdit::textChanged,this,&MainWindow::on_cmdLineEdit_textChanged);
    connect(ui->btnLoadCode,&QPushButton::clicked,this,&MainWindow::loadCodesFromFile);
    connect(ui->btnRunCode,&QPushButton::clicked,this, &MainWindow::programRun);
    connect(ui->btnClearCode,&QPushButton::clicked,this,&MainWindow::clearProgram);
    connect(ui->btnDebugResume,&QPushButton::clicked,this,&MainWindow::resumeProgramRunFromBreak);
}

MainWindow::~MainWindow()
{
    //析构函数
    delete ui;
}

void MainWindow::on_cmdLineEdit_editingFinished()
{
    //处理cmdLineEdit的输入
    if(this->isInteractiveInput){
        //如果处于交互式输入状态(无行号INPUT)情况下，直接处理输入
        this->interactiveInput();
        return;
    }
    if(this->isInputMode){
        //如果处于输入状态，处理输入
        try{
            this->resumeProgramRunFromInput();
        } catch (QString e) {
            updateResult(e);
        }
        return;
    }
    //否则，认为输入是一条指令或者是一行代码，处理输入
    QString cmd = ui->cmdLineEdit->text();
    this->parseCmdInput(cmd);
}

void MainWindow::setUIForDebugMode(){
    //设置UI为调试模式
    ui->btnClearCode->setVisible(false);
    ui->btnLoadCode->setVisible(false);
    ui->btnDebugMode->setVisible(false);

    ui->btnExitDebugMode->setVisible(true);
    ui->btnDebugResume->setVisible(true);

    ui->labelSyntaxTree->setVisible(false);
    ui->treeDisplay->setVisible(false);

    ui->labelMonitor->setVisible(true);
    ui->monitorDisplay->setVisible(true);
    ui->labelBreakPoints->setVisible(true);
    ui->breakPointsDisplay->setVisible(true);

    this->isDebugMode = true;
}

void MainWindow::setUIExitDebugMode(){
    //设置UI为退出调试模式
    ui->btnClearCode->setVisible(true);
    ui->btnLoadCode->setVisible(true);
    ui->btnDebugMode->setVisible(true);

    ui->btnExitDebugMode->setVisible(false);
    ui->btnDebugResume->setVisible(false);

    ui->labelSyntaxTree->setVisible(true);
    ui->treeDisplay->setVisible(true);

    ui->labelMonitor->setVisible(false);
    ui->monitorDisplay->setVisible(false);
    ui->labelBreakPoints->setVisible(false);
    ui->breakPointsDisplay->setVisible(false);

    this->isDebugMode = false;
}

Ui::MainWindow * MainWindow::getUi() {
    //获取UI
    return this->ui;
}

void MainWindow::loadCodesFromFile() {
    //加载代码，打开一个框，选择文件
    std::cout << "Now Load Codes!" << std::endl;
    QString fileName = QFileDialog::getOpenFileName(this,tr("Open BASIC Code File"),"C:/Users/overji/Desktop/homework/QBasic-handout/",tr("BASIC Code Files: (*.basic)"));
    try{
        this->loadFromFile(fileName);
    } catch (QString e) {
        updateResult(e);
        std::cout << e.toStdString() << std::endl;
        return;
    }
}

void MainWindow::loadFromFile(QString path) {
    //根据给出的文件路径path，加载文件
    QFile codeFile(path);
    if(! codeFile.open(QIODevice::ReadOnly | QIODevice::Text)){
        throw QString("Wrong File ") + path;
    }
    //如果文件不为空，清空当前程序
    this->clearProgram();
    this->program->clearBreak();
    QTextStream textStream(&codeFile);
    while(!textStream.atEnd()){
        //逐行读取文件，并将其当作输入处理，注意这里没有考虑不带行号的INPUT,PRINT,LET等情况
        QString text = textStream.readLine();
        try{
            this->parseCmdInput(text);
        } catch (QString e) {
            updateResult(e);
            return;
        }
    }
}

void MainWindow::quitInterpreter() {
    //退出解释器
    this->close();
}

void MainWindow::clearProgram() {
    //清空程序
    this->ui->treeDisplay->setText("");
    this->ui->CodeDisplay->setText("");
    this->ui->cmdLineEdit->setText("");
    this->ui->textBrowser->setText("");
    this->ui->breakPointsDisplay->setText("");
    this->ui->monitorDisplay->setText("");
    this->codesInCodeDisplay->clearAll();
    this->program->clearBreak();
}

void MainWindow::updateResult(QString text) {
    //更新结果
    this->ui->textBrowser->setHtml(plainText2htmlText(text));
}

QString MainWindow::getResult() {
    //获取结果
    return this->ui->textBrowser->toPlainText();
}

void MainWindow::printHelp() {
    //打印帮助信息
    QString hint = "This is a BASIC interpreter written by Ji\n"
                   "Commands are as follows:\n"
                   "RUN : run the program\n"
                   "LOAD : load codes from file\n"
                   "CLEAR : clear the current program\n"
                   "HELP : print current Message\n"
                   "QUIT : exits from the BASIC interpreter";
    this->updateResult(hint);
}

void MainWindow::judgeOrder(commands cmd) {
    //根据命令cmd，执行相应的操作
    switch (cmd) {
        case CMD_RUN:
            this->programRun();
            break;
        case CMD_LOAD:
            this->loadCodesFromFile();
            break;
        case CMD_CLEAR:
            this->clearProgram();
            break;
        case CMD_HELP:
            this->printHelp();
            break;
        case CMD_QUIT:
            this->quitInterpreter();
            break;
        default:
            break;
    }
}

Program *MainWindow::getProgram() {
    //获取程序
    return this->program;
}

void MainWindow::setUIForInputMode(bool mode,Variable * var) {
    //设置UI为输入模式
    this->isInputMode = mode;
    this->inputVar = var;
    this->program->setProgramInputPaused(true);
}

void MainWindow::on_cmdLineEdit_textChanged() {
    //在输入状态下，如果输入不是以?开头，则自动加上?
    if(!this->isInputMode && !this->isInteractiveInput)return;
    QString curInput = this->ui->cmdLineEdit->text();
    if(curInput == "" || curInput[0] != '?'){
        curInput = "?" + curInput;
    }
    this->ui->cmdLineEdit->setText(curInput);
}

void MainWindow::setCmdLineEditInputMode(){
    //设置输入模式，也就是首个字符必须是?
    this->ui->cmdLineEdit->setText("?");
}

void MainWindow::programRun() {
    //运行程序
    try{
        this->codesInCodeDisplay->updateCodeLines();
        this->showSyntaxTree();
        this->program->run(0);
    } catch (QString e) {
        updateResult(e);
    }
}

void MainWindow::resumeProgramRunFromInput() {
    //从输入状态恢复程序运行
    QRegularExpression re(R"(^\s*\?\s*(\d+)\s*$)");
    QRegularExpressionMatch match = re.match(ui->cmdLineEdit->text());
    if(match.hasMatch()) {
        //如果用户正常输入，将输入的值赋给变量，然后继续运行
        this->program->setVariable(this->inputVar->getName(), match.captured(1).toInt());
        this->program->setProgramInputPaused(false);
        this->isInputMode = false;
        this->inputVar = nullptr;
        ui->cmdLineEdit->setText("");
        this->program->run(1);
        return;
    } else {
        //否则，继续保持输入状态
        this->ui->cmdLineEdit->setText("?");
        this->setUIForInputMode(true,this->inputVar);
        return;
    }
}

void MainWindow::interactiveInput(){
    //交互式输入
    QRegularExpression re(R"(^\s*\?\s*(\d+)\s*$)");
    QRegularExpressionMatch match = re.match(ui->cmdLineEdit->text());
    if(match.hasMatch()){
        //如果用户正常输入，将输入的值赋给变量
        this->program->setVariable(this->inputVar->getName(),match.captured(1).toInt());
        this->isInteractiveInput = false;
        this->inputVar = nullptr;
        ui->cmdLineEdit->setText("");
        return;
    } else {
        //否则，继续保持输入状态
        this->ui->cmdLineEdit->setText("?");
        return;
    }
}

void MainWindow::parseCmdInput(QString cmd) {
    //处理cmdLine获取到的用户输入
    if(cmd.isEmpty()){
        return;
    }
    ui->cmdLineEdit->setText("");
    try{
        //首先尝试解析用户输入的代码
        auto pr = cmdInput::parseCodeInput(cmd);
        //限制行号范围
        if(pr.first < 0 || pr.first > 1000000){
            throw QString("Invalid Index!");
        }
        //逐行解析代码，这里损失了一些性能，但是保障了程序安全
        this->codesInCodeDisplay->updateCodeLines();
        //如果行号已经存在，更新或者删除，否则插入新行
        if(this->codesInCodeDisplay->indexExists(pr.first)){
            this->codesInCodeDisplay->updateOrDeleteLine(pr);
        } else {
            if(!pr.second.isEmpty()){
                this->program->insertCodePair(pr);
            }
        }
        //更新代码显示
        this->ui->CodeDisplay->setHtml(plainText2htmlText(this->codesInCodeDisplay->getPlainCode()));
    } catch (QString e) {
        //如果捕获到异常，说明用户输入的可能是指令
        try{
            //尝试解析指令
            commands order = cmdInput::parseCommandInput(cmd);
            if(this->isDebugMode && (order == CMD_DELBREAK || order == CMD_ADDBREAK)){
                //如果处于调试模式，且用户输入的是ADD或者DEL指令，那么判断是否是断点指令
                judgeBreakOrder(cmd);
            } else if(!this->isDebugMode && (order == CMD_DELBREAK || order == CMD_ADDBREAK)) {
                //如果不处于调试模式，且用户输入的是ADD或者DEL指令，那么提示用户不在调试模式
                this->updateResult(QString("Unable to update break point while not in debug mode!"));
            } else {
                //否则，执行指令
                judgeOrder(order);
            }
        } catch (QString e) {
            //如果捕获到异常，说明用户非法输入了，显示错误信息。
            cmdInput::parseDirectCode(cmd,this);
        }
    }
    this->program->printProgram();
}

void MainWindow::showSyntaxTree(){
    //显示语法树
    this->ui->treeDisplay->clear();
    this->ui->treeDisplay->setHtml(plainText2htmlText(this->program->getSyntaxTree()));
}

void MainWindow::setMonitor(const QString & str){
    //设置变量监视器
    this->ui->monitorDisplay->setHtml(plainText2htmlText(str));
}

void MainWindow::setBreakMonitor(const QString & str){
    //设置断点监视器
    this->ui->breakPointsDisplay->setHtml(plainText2htmlText(str));
}

void MainWindow::judgeBreakOrder(const QString &cmd){
    //判断断点指令是否合法，如果合法则处理断点。
    QRegularExpression breakRe(R"(^\s*([A-Za-z]+)\s+([0-9]+)\s*$)");
    QRegularExpressionMatch breakMatch = breakRe.match(cmd);
    if(!breakMatch.hasMatch()){
        throw QString("Invalid Command!");
    }
    QString command = breakMatch.captured(1);
    int target = breakMatch.captured(2).toInt();
    if(command == "ADD"){
        //如果命令是ADD，那么插入断点
        this->program->setBreak(target);
    } else {
        //否则直接删除断点
        this->program->deleteBreak(target);
    }
    //更新断点监视器
    this->setBreakMonitor(this->program->getBreakPointsMonitor());
}

void MainWindow::resumeProgramRunFromBreak() {
    //从断点恢复程序运行
    if(!this->program->isBreakPausing() || !this->program->isRunning()){
        return;
    } else {
        this->program->resumeFromBreak();
        this->setMonitor("");
        this->program->run(2);
    }
}

bool MainWindow::getDebugMode(){
    //获取当前程序是否运行在调试模式下
    return this->isDebugMode;
}




