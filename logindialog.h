#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include "global.h"
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
    void initHttoHandlers();
    // http处理函数的map，处理函数接受的参数是一个json对象
    QMap<ReqId, std::function<void(const QJsonObject&)>> _handlers;
    Ui::LoginDialog *ui;

    void initHead();
    bool checkEmailVaild();
    bool checkPwdValid();

    void showTip(QString str, bool b_ok);
    void AddTipErr(TipErr te, QString tips);
    void DelTipErr(TipErr te);
    QMap<TipErr, QString> _tip_errs;

    bool enableBtn(bool enabled);

    int _uid;
    QString _token;

// 定义信号
signals:
    void switchRegister();
    void switchReset();

    void sig_connect_tcp(ServerInfo);

private slots:
    void slot_forget_pwd();
    void on_login_btn_clicked();
    void slot_login_mod_finish(ReqId id, QString res, ErrorCodes err);
};

#endif // LOGINDIALOG_H
