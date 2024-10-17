#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <math.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    digitBTNs = {{Qt::Key_0, ui->btnNum0},
        {Qt::Key_1, ui->btnNum1},
        {Qt::Key_2, ui->btnNum2},
        {Qt::Key_3, ui->btnNum3},
        {Qt::Key_4, ui->btnNum4},
        {Qt::Key_5, ui->btnNum5},
        {Qt::Key_6, ui->btnNum6},
        {Qt::Key_7, ui->btnNum7},
        {Qt::Key_8, ui->btnNum8},
        {Qt::Key_9, ui->btnNum9},
    };

    foreach (auto btn, digitBTNs) {
        connect(btn, SIGNAL(clicked()), this, SLOT(btnNumClicked()));
    }

    connect(ui->btnPlus, SIGNAL(clicked()), this, SLOT(btnBinaryOperatorClicked()));
    connect(ui->btnMinus, SIGNAL(clicked()), this, SLOT(btnBinaryOperatorClicked()));
    connect(ui->btnMultiple, SIGNAL(clicked()), this, SLOT(btnBinaryOperatorClicked()));
    connect(ui->btnDivide, SIGNAL(clicked()), this, SLOT(btnBinaryOperatorClicked()));

    connect(ui->btnPercentage, SIGNAL(clicked()), this, SLOT(btnUnaryOperatorClicked()));
    connect(ui->btnInverse, SIGNAL(clicked()), this, SLOT(btnUnaryOperatorClicked()));
    connect(ui->btnSquare, SIGNAL(clicked()), this, SLOT(btnUnaryOperatorClicked()));
    connect(ui->btnSqrt, SIGNAL(clicked()), this, SLOT(btnUnaryOperatorClicked()));
    connect(ui->btnSign, SIGNAL(clicked()), this, SLOT(btnUnaryOperatorClicked()));




}

MainWindow::~MainWindow()
{
    delete ui;
}

QString MainWindow::calculation(bool *ok)
{
    double result = 0;
    if (operands.size() == 2 && opcodes.size() > 0) {
        //取操作数
        double operand1 = operands.front().toDouble();
        operands.pop_front();
        double operand2 = operands.front().toDouble();
        operands.pop_front();

        //取操作符
        QString op = opcodes.front();
        opcodes.pop_front();

        if (op == "+") {
            result = operand1 + operand2;
        } else if (op == "-") {
            result = operand1 - operand2;
        } else if (op == "×") {
            result = operand1 * operand2;
        } else if (op == "÷") {
            result = operand1 / operand2;
        }

        lastResult = result;

        operands.push_back(QString::number(result));
        ui->statusbar->showMessage(QString("calculation in progress: operands is %1, opcode is %2").arg(operands.size()).arg(
                                       opcodes.size()));
    } else {
        ui->statusbar->showMessage(QString("operands is %1, opcode is %2").arg(operands.size()).arg(opcodes.size()));
    }
    return QString::number(result);
}

void MainWindow::btnNumClicked()
{
    QString digit = qobject_cast<QPushButton *>(sender())->text();

    // 如果之前的操作是按了等号，则清空 operand，准备输入新的操作数
    if (operand == QString::number(lastResult)) {
        operand = "";  // 清空之前的结果，准备输入新的数字
    }

    if (digit == "0" && operand == "0")
        digit = "";
    if (operand == "0" && digit != "0")
        operand = "";

    operand += digit;

    ui->display->setText(operand);

}

void MainWindow::btnBinaryOperatorClicked()
{
    ui->statusbar->showMessage("last operand" + operand);
    QString opcode = qobject_cast<QPushButton *>(sender())->text();

    if (hasCalculated) {
        operands.clear();
        hasCalculated = false;
    }
    if (operand != "") {
        operands.push_back(operand);
        operand = "";

    }

    if (!operand.isEmpty()) {
        opcodes.pop_back();
    }

    opcodes.push_back(opcode);

    QString result = calculation();
    ui->display->setText(result);


}

void MainWindow::btnUnaryOperatorClicked()
{
    if (!operand.isEmpty()) {
        double result = operand.toDouble();
        operand = "";

        QString op = qobject_cast<QPushButton *>(sender())->text();
        if (op == "%")
            result /= 100.0;
        else if (op == "1/x")
            result = 1 / result;
        else if (op == "x^2")
            result *= result;
        else if (op == "√")
            result = sqrt(result);
        else if (op == "±")
            result = -result;

        operand = QString::number(result);
        ui->display->setText(operand);
    }



}

void MainWindow::on_btnPeriod_clicked()
{
    if (!operand.contains("."))
        operand += qobject_cast<QPushButton *>(sender())->text();
    ui->display->setText(operand);
}


void MainWindow::on_btnDelete_clicked()
{
    if (!operand.isEmpty()) {
        operand = operand.left(operand.length() - 1);
        ui->display->setText(operand);
    }

}


void MainWindow::on_btnClearAl_clicked()
{
    operand.clear();
    opcode.clear();
    operands.clear();
    opcodes.clear();
    lastResult = 0;
    ui->display->setText(operand);
}


void MainWindow::on_btnEqual_clicked()
{
    if (!operand.isEmpty() && opcodes.isEmpty()) {
        ui->display->setText(operand);
        lastResult = operand.toDouble();
        return;
    }

    if (operand.isEmpty() && !operands.isEmpty()) {
        operand = operands.top();
    }

    if (!operand.isEmpty()) {
        operands.push_back(operand);
        operand = "";

    }

    if (!operands.isEmpty()) {
        QString result = calculation();
        ui->display->setText(result);
        lastResult = result.toDouble();
    }


    opcodes.clear();
    operands.clear();
    operand = QString::number(lastResult);

    hasCalculated = true;

}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    foreach (auto btnKey, digitBTNs.keys()) {
        if (event->key() == btnKey)
            digitBTNs[btnKey]->animateClick();
    }

    if (event->key() == Qt::Key_Plus)
        ui->btnPlus->animateClick();
    if (event->key() == Qt::Key_Minus)
        ui->btnMinus->animateClick();
    if (event->key() == Qt::Key_Asterisk)
        ui->btnMultiple->animateClick();
    if (event->key() == Qt::Key_Slash)
        ui->btnDivide->animateClick();
    if (event->key() == Qt::Key_Backspace)
        ui->btnDelete->animateClick();
    if (event->key() == Qt::Key_Enter)
        ui->btnEqual->animateClick();
    if (event->key() == Qt::Key_Percent)
        ui->btnPercentage->animateClick();
    if (event->key() == Qt::Key_Period)
        ui->btnPeriod->animateClick();


}



void MainWindow::on_btnClear_clicked()//计算器CE键
{
    operand.clear();
    ui->display->setText("0");

}

