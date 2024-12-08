#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "logindialog.h"
#include "registerdialog.h"
/******************************************************************************
 *
 * @file       mainwindow.h
 * @brief      主窗口
 *
 * @author     陈佳波
 * @date       2024/12/08
 * @history
 *****************************************************************************/
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
// 定义槽函数
public slots:
    void SlotSwitchReg();
private:
    Ui::MainWindow *ui;
    LoginDialog * _login_dlg;
    RegisterDialog * _reg_dlg;
};
#endif // MAINWINDOW_H
