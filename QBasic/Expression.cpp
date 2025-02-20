//
// Created by overji on 2024/12/3.
//

#include "Expression.h"
#include "util.h"
#include <QStack>
#include <iostream>
#include <QQueue>

Expression::Expression(const QString& expression,Program * program) {
    //根据输入和现有的程序，判断表达式是否合法，如果合法则构建表达式树，否则抛出异常
    this->program = program;
    if(!checkExpressValidity(expression)){
        throw QString("Unknown Signal in your expression ") + expression;
    }
    this->root = nullptr;
    this->buildStack = new QStack<assistNode>;
    try{
        this->midFix2preFix(expression);
        this->buildExpressTree();
    } catch (QString e){
        throw e;
    }
}

void Expression::midFix2preFix(QString str) {
    //中缀表达式转前缀表达式
    removeSpace(str); //去除空格
    QStack<expressionType> opeStack; //操作符栈
    int cur = str.size() - 1; //从后往前遍历
    while(cur >= 0) {
        if (str[cur] == '+') {
            //如果是加号，那么将栈中所有优先级高于加号的操作符弹出
            while(!opeStack.isEmpty() && (opeStack.top() == EXP_MULT || opeStack.top() == EXP_DIV || opeStack.top() == EXP_MOD || opeStack.top() == EXP_EXPON)){
                auto tmpType = opeStack.pop();
                auto tmpNode = assistNode(tmpType);
                this->buildStack->push(tmpNode);
            }
            opeStack.push(EXP_ADD);
            cur --;
        } else if (str[cur] == '-') {
            //如果是减号或者负号，那么将栈中所有优先级高于减号的操作符弹出
            while(!opeStack.isEmpty() && (opeStack.top() == EXP_MULT || opeStack.top() == EXP_DIV || opeStack.top() == EXP_MOD || opeStack.top() == EXP_EXPON)){
                auto tmpType = opeStack.pop();
                auto tmpNode = assistNode(tmpType);
                this->buildStack->push(tmpNode);
            }
            if(cur == 0 || str[cur-1] == '('){
                //如果是负号，那么将负号压入栈中
                opeStack.push(EXP_NEG);
            }else{
                //如果是减号，那么将减号压入栈中
                opeStack.push(EXP_SUB);
            }
            cur --;
        } else if (str[cur] == '*') {
            //如果是乘号或者乘方，那么将栈中所有优先级高于乘号的操作符弹出
            if(cur != 0 && str[cur - 1] == '*'){
                //判断是否是乘方，如果是乘方，那么将栈中所有优先级高于乘方的操作符弹出，并且将乘方压入栈中
                while(!opeStack.isEmpty() && opeStack.top() == EXP_EXPON){
                    auto tmpType = opeStack.pop();
                    auto tmpNode = assistNode(tmpType);
                    this->buildStack->push(tmpNode);
                }
                opeStack.push(EXP_EXPON);
                cur-=2;
            } else {
                //如果是乘号，那么将乘号压入栈中，并且将栈中所有优先级高于乘号的操作符弹出
                while(!opeStack.isEmpty() && opeStack.top() == EXP_EXPON){
                    auto tmpType = opeStack.pop();
                    auto tmpNode = assistNode(tmpType);
                    this->buildStack->push(tmpNode);
                }
                opeStack.push(EXP_MULT);
                cur --;
            }
        } else if (str[cur] == '/') {
            //如果是除号，那么将栈中所有优先级高于除号的操作符弹出，并且将除号压入栈中
            while(!opeStack.isEmpty() && opeStack.top() == EXP_EXPON){
                auto tmpType = opeStack.pop();
                auto tmpNode = assistNode(tmpType);
                this->buildStack->push(tmpNode);
            }
            opeStack.push(EXP_DIV);
            cur --;
        } else if (isMod(str,cur)) {
            //如果是取模，那么将栈中所有优先级高于取模的操作符弹出，并且将取模压入栈中，并且将cur指针向前移动3位
            while(!opeStack.isEmpty() && opeStack.top() == EXP_EXPON){
                auto tmpType = opeStack.pop();
                auto tmpNode = assistNode(tmpType);
                this->buildStack->push(tmpNode);
            }
            opeStack.push(EXP_MOD);
            cur -= 3;
        } else if (str[cur] == ')') {
            //如果是右括号，那么压入栈中，并且将cur指针向前移动一位
            opeStack.push(EXP_RBRA);
            cur --;
        } else if (str[cur] == '(') {
            //如果是左括号，那么将栈中所有操作符弹出，直到遇到右括号
            expressionType tmpType = EXP_ADD;
            if(opeStack.empty()){
                //如果栈为空，那么说明括号不匹配
                throw QString("Invalid expression!");
            }
            while((tmpType = opeStack.pop()) != EXP_RBRA){
                auto tmpNode = assistNode(tmpType);
                this->buildStack->push(tmpNode);
            }
            cur --;
        } else {
            //如果是数字或者变量，那么将数字或者变量压入栈中
            QString varOrNum = "";
            bool isVar = false;//判断是否是变量
            while (str[cur] != '+' && str[cur] != '-' && str[cur] != '*' && str[cur] != '/' && str[cur] != '(' &&
                   str[cur] != ')'&& str[cur] != ' ' && !isMod(str,cur)) {
                //获取完整的数字或者变量名
                varOrNum = str[cur] + varOrNum;
                if(!str[cur].isDigit()){
                    isVar = true;
                }
                cur--;
                if(cur < 0){
                    //如果cur小于0，那么说明已经到达了表达式的最左边
                    break;
                }
            }
            if(isVar && !checkVarNameValidity(varOrNum)){
                //如果是变量，那么判断变量名是否合法
                throw QString("Invalid Variable! ") + varOrNum;
            }
            if(isVar){
                //如果是变量，那么将变量压入栈中
                auto tmpNode = assistNode(EXP_VAR,-1,varOrNum);
                this->buildStack->push(tmpNode);
            } else {
                //如果是数字，那么将数字压入栈中
                auto tmpNode = assistNode(EXP_NUM,varOrNum.toInt());
                this->buildStack->push(tmpNode);
            }
        }
    }
    while(!opeStack.isEmpty()){
        //将栈中剩余的操作符弹出
        auto tmpType = opeStack.pop();
        if(tmpType == EXP_RBRA){
            //如果栈中有右括号，那么说明括号不匹配
            throw QString("Invalid Bracket! ") + str;
        }
        auto tmpNode = assistNode(tmpType);
        this->buildStack->push(tmpNode);
    }
}

bool Expression::isMod(QString str, int cur) {
    //判断是否是取模
    return cur >= 2 && str[cur] == 'D' && str[cur - 1] == 'O' && str[cur - 2] == 'M';
}

void Expression::removeSpace(QString &str) {
    //去除空格
    for(int i = 0; i < str.size(); i++){
        if(str[i] == ' '){
            str.remove(i,1);
            i--;
        }
    }
}

bool Expression::isOperator(QString str, int cur) {
    //判断是否是操作符
    return str[cur] == '+' || str[cur] == '-' || str[cur] == '*' || str[cur] == '/' || isMod(str,cur);
}

void Expression::logBuildStack() {
    //打印栈
    std::cout << "----STACK----" << std::endl;
    for(auto i = this->buildStack->begin(); i != this->buildStack->end(); i++){
        if(i->type == EXP_NUM){
            std::cout << i->number << std::endl;
        } else if(i->type == EXP_VAR){
            std::cout << i->varName.toStdString() << std::endl;
        } else if(i->type == EXP_ADD){
            std::cout << "+" << std::endl;
        } else if(i->type == EXP_SUB || i->type == EXP_NEG){
            std::cout << "-" << std::endl;
        } else if(i->type == EXP_MULT){
            std::cout << "*" << std::endl;
        } else if(i->type == EXP_DIV){
            std::cout << "/" << std::endl;
        } else if(i->type == EXP_MOD){
            std::cout << "MOD" << std::endl;
        } else if(i->type == EXP_EXPON){
            std::cout << "**" << std::endl;
        } else if(i->type == EXP_RBRA){
            std::cout << ")" << std::endl;
        }
    }
    std::cout << "----STACK----" << std::endl;
}

ExpressionNode *Expression::buildExpressTreeHelper() {
    //根据栈中的元素构建表达式树
    assistNode node = buildStack->pop();
    ExpressionNode * ans = new ExpressionNode();
    ans->type = node.type;
    if(ans->type == EXP_NUM){
        ans->number = node.number;
        return ans;
    } else if(ans->type == EXP_VAR) {
        ans->varName = node.varName;
        return ans;
    }
    if(this->buildStack->isEmpty()){
        return ans;
    }
    ans->left = buildExpressTreeHelper();
    if(ans->type == EXP_NEG){
        //负号只有左子树
        ans->right = nullptr;
        return ans;
    }
    if(this->buildStack->isEmpty()){
        throw QString("Invalid Expression!");
    }
    ans->right = buildExpressTreeHelper();
    return ans;
}

void Expression::logExpressTree() {
    //打印表达式树
    std::cout << "----BUILD----" << std::endl;
    logExpressTreeHelper(this->root);
}

void Expression::logExpressTreeHelper(ExpressionNode *rt) {
    //打印表达式树
    if(rt == nullptr){
        return;
    }
    if(rt->type == EXP_NUM){
        std::cout << rt->number << std::endl;
    } else if(rt->type == EXP_VAR){
        std::cout << rt->varName.toStdString() << std::endl;
    } else if(rt->type == EXP_ADD){
        std::cout << "+" << std::endl;
    } else if(rt->type == EXP_SUB || rt->type == EXP_NEG){
        std::cout << "-" << std::endl;
    } else if(rt->type == EXP_MULT){
        std::cout << "*" << std::endl;
    } else if(rt->type == EXP_DIV){
        std::cout << "/" << std::endl;
    } else if(rt->type == EXP_MOD){
        std::cout << "MOD" << std::endl;
    } else if(rt->type == EXP_EXPON){
        std::cout << "**" << std::endl;
    } else if(rt->type == EXP_RBRA){
        std::cout << ")" << std::endl;
    }
    logExpressTreeHelper(rt->left);
    logExpressTreeHelper(rt->right);
}

void Expression::buildExpressTree() {
    //构建表达式树
    this->root = buildExpressTreeHelper();
    if(!this->buildStack->empty()){
        throw QString("Invalid Expression!");
    }
}

QString Expression::levelTraverseExpressTree(int level) {
    //递归地层级遍历表达式树
    if(root == nullptr)return "";//如果根节点为空，那么返回空字符串
    QString ans = "";
    QQueue<ExpressionNode *>que;
    que.enqueue(root);
    while(!que.isEmpty()){
        //标准的层次遍历代码
        int len = que.size();
        for(int i = 0;i < len;i ++){
            auto node = que.dequeue();
            for(int s = 0;s < level;s ++){
                ans += "    ";
            }
            if(node->type == EXP_ADD){
                ans += "+\n";
            } else if(node->type == EXP_SUB || node->type == EXP_NEG){
                ans += "-\n";
            } else if(node->type == EXP_MULT){
                ans += "*\n";
            } else if(node->type == EXP_DIV){
                ans += "/\n";
            } else if(node->type == EXP_MOD){
                ans += "MOD\n";
            } else if(node->type == EXP_EXPON){
                ans += "**\n";
            } else if(node->type == EXP_NUM){
                ans += QString::number(node->number) + "\n";
            } else if(node->type == EXP_VAR){
                ans += node->varName + "\n";
            }
            if(node->left)que.enqueue(node->left);
            if(node->right)que.enqueue(node->right);
        }
        level++;
    }
    return ans;
}

bool Expression::checkExpressValidity(QString str) {
    //检查表达式是否合法
    for(QChar chr:str){
        if(!chr.isLetterOrNumber() && !chr.isSpace() && !(chr == '+' || chr == '_' || chr == '-' || chr == '*' || chr == '/' || chr == '(' || chr == ')')){
            return false;
        }
    }
    return true;
}

int Expression::calculateExpressionTreeRecursion(ExpressionNode *root) {
    //递归地计算表达式树
    if(root == nullptr){
        return 0;
    }
    int left = calculateExpressionTreeRecursion(root->left); //计算左子树
    int right = calculateExpressionTreeRecursion(root->right); //计算右子树
    int sign = 1;
    int ans = 0;
    QString name = "";
    Variable * var = nullptr;
    switch (root->type) {
        case EXP_VAR:
            //如果是变量，那么获取变量的值
           assert(root->varName != nullptr);
           name = root->varName;
           var = this->program->getVariable(name);
           if(var == nullptr){
               throw QString::fromStdString("Variable " + name.toStdString() + " Not Found!");
           }
           ans = var->getValue() * sign;
           return ans;
        case EXP_NUM:
            //如果是数字，那么直接返回数字
            return root->number;
        case EXP_ADD:
            return left + right;
        case EXP_NEG:
            return -left;
        case EXP_EXPON:
            //如果是乘方，那么计算乘方
            return powl(left,right);
        case EXP_DIV:
            return left / right;
        case EXP_MOD:
            return left % right;
        case EXP_MULT:
            return left * right;
        case EXP_SUB:
            return left - right;
        default:
            return 0;
    }
}

int Expression::calculateExpressionTree() {
    //计算表达式树
    return this->calculateExpressionTreeRecursion(root);
}

//int main(){
//    try{
//        Program * program = new Program();
//        program->setVariable("a",11);
//        Expression exp("-b*5+3**(2)",program);
//        std::cout << exp.levelTraverseExpressTree().toStdString() << std::endl;
//        std::cout << exp.calculateExpressionTree() << std::endl;
//    } catch (QString e){
//        std::cout << e.toStdString() << std::endl;
//    }
//    return 0;
//}
