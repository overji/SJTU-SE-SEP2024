#include "util.h"

QString plainText2htmlText(QString plainText) {
    //将纯文本转换为HTML格式的文本
    QString htmlText = "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
                       "<html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">\n"
                       "p, li { white-space: pre-wrap; }\n"
                       "hr { height: 1px; border-width: 0; }\n"
                       "li.unchecked::marker { content: \"\\2610\"; }\n"
                       "li.checked::marker { content: \"\\2612\"; }\n"
                       "</style></head><body style=\" font-family:'Microsoft YaHei UI'; font-size:9pt; font-weight:400; font-style:normal;\">\n";

    QStringList lines = plainText.split('\n');
    for (const QString& line : lines) {
        htmlText += "<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">";
        htmlText += "<span style=\" font-family:'.AppleSystemUIFont'; font-size:13pt;\">";
        htmlText += line.toHtmlEscaped();
        htmlText += "</span></p>\n";
    }
    htmlText += "</body></html>";
    return htmlText;
}

bool checkVarNameValidity(const QString &name) {
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