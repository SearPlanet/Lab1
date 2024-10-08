#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QLabel>
#include<QStack>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QString operand;
    QStack<QString> operands;

private slots:
    void btnNumClicked();
    void btnBinaryOperatorClicked();

    void on_btnPeriod_clicked();

    void on_btnDelete_clicked();

    void on_btnClear_clicked();

    void on_btnEqual_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
