#pragma once

enum commands{
    CMD_RUN,
    CMD_LOAD,
    CMD_CLEAR,
    CMD_HELP,
    CMD_QUIT,
    CMD_LIST,
    CMD_ADDBREAK,
    CMD_DELBREAK
};

#include <QMainWindow>
#include "cmdInput.h"
#include "CodesInCodeDisplay.h"
#include "Program.h"

class CodesInCodeDisplay;
class cmdInput;


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    friend class cmdInput;
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;
    Ui::MainWindow * getUi();
    void quitInterpreter();
    void printHelp();
    Program * getProgram();
    void updateResult(QString text);
    QString getResult();
    void setUIForInputMode(bool mode = true, Variable * var = nullptr);
    void setMonitor(const QString &str);
    void setBreakMonitor(const QString &str);
    void setCmdLineEditInputMode();
    bool getDebugMode();

public slots:
    void loadCodesFromFile();
    void clearProgram();

private slots:
    void on_cmdLineEdit_editingFinished();
    void on_cmdLineEdit_textChanged();
    void programRun();
    void resumeProgramRunFromBreak();

private:
    Ui::MainWindow *ui;
    CodesInCodeDisplay * codesInCodeDisplay;
    Program * program;

    bool isInteractiveInput;
    bool isInputMode;
    bool isDebugMode;
    Variable * inputVar;

    void setUIForDebugMode();
    void setUIExitDebugMode();

    void loadFromFile(QString path);

    void judgeOrder(commands cmd);

    void resumeProgramRunFromInput();

    void interactiveInput();

    void parseCmdInput(QString cmd);

    void showSyntaxTree();

    void judgeBreakOrder(const QString &cmd);
};