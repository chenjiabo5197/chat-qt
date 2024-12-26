#ifndef REGISTERDIALOG_H
#define REGISTERDIALOG_H

#include <QDialog>
#include "global.h"
/******************************************************************************
 *
 * @file       registerdialog.h
 * @brief      注册窗口
 *
 * @author     陈佳波
 * @date       2024/12/08
 * @history
 *****************************************************************************/
namespace Ui {
class RegisterDialog;
}

class RegisterDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RegisterDialog(QWidget *parent = nullptr);
    ~RegisterDialog();

private slots:
    void on_get_code_btn_clicked();
    void slot_reg_mod_finish(ReqId req_id, QString res, ErrorCodes err);

    void on_sure_btn_clicked();

    void on_return_btn_clicked();

    void on_cancel_btn_clicked();

private:
    // 初始化http处理函数
    void initHttpHandlers();
    void showTip(QString str, bool b_ok);

    void AddTipErr(TipErr te, QString tips);
    void DelTipErr(TipErr te);
    QMap<TipErr, QString> _tip_errs;

    bool checkUserValid();
    bool checkEmailValid();
    bool checkPassValid();
    bool checkConfirmValid();
    bool checkVerifyValid();

    Ui::RegisterDialog *ui;
    // http处理函数的map，处理函数接受的参数是一个json对象
    QMap<ReqId, std::function<void(const QJsonObject&)>> _handlers;
    // 计时器
    QTimer* _countdown_timer;
    // 倒计时数字
    int _countdown;
    // 切换登录界面的页码
    void ChangeTipPage();

signals:
    void sigSwitchLogin();
};

#endif // REGISTERDIALOG_H
