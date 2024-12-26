#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
/******************************************************************************
 *
 * @file       logindialog.h
 * @brief      登录窗口
 *
 * @author     陈佳波
 * @date       2024/12/08
 * @history
 *****************************************************************************/
namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = nullptr);
    ~LoginDialog();

private:
    Ui::LoginDialog *ui;
// 定义信号
signals:
    void switchRegister();
    void switchReset();

private slots:
    void slot_forget_pwd();
};

#endif // LOGINDIALOG_H
