#ifndef RESETDIALOG_H
#define RESETDIALOG_H

#include <QDialog>
#include "global.h"

/******************************************************************************
 *
 * @file       resetdialog.h
 * @brief      重置密码窗口
 *
 * @author     陈佳波
 * @date       2024/12/28
 * @history
 *****************************************************************************/

namespace Ui {
class ResetDialog;
}

class ResetDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ResetDialog(QWidget *parent = nullptr);
    ~ResetDialog();

private:
    Ui::ResetDialog *ui;

    bool checkUserValid();
    bool checkPassValid();
    bool checkEmailValid();
    bool checkVerifyValid();

    void showTip(QString str, bool b_ok);
    void AddTipErr(TipErr te, QString tips);
    void DelTipErr(TipErr te);

    void initHandlers();

    QMap<TipErr, QString> _tip_errs;
    QMap<ReqId, std::function<void(const QJsonObject&)>> _handlers;

private slots:

    void on_verify_btn_clicked();
    void on_sure_btn_clicked();
    void on_return_btn_clicked();
    void slot_reset_mod_finish(ReqId id, QString res, ErrorCodes err);

signals:
    void switchLogin();
};

#endif // RESETDIALOG_H
