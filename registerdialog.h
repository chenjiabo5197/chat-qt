#ifndef REGISTERDIALOG_H
#define REGISTERDIALOG_H

#include <QDialog>
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

private:
    void showTip(QString str, bool b_ok);
    Ui::RegisterDialog *ui;
};

#endif // REGISTERDIALOG_H
