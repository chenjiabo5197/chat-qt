#include "logindialog.h"
#include "ui_logindialog.h"

LoginDialog::LoginDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
    ui->pass_edit->setEchoMode(QLineEdit::Password);
    connect(ui->register_btn, &QPushButton::clicked, this, &LoginDialog::switchRegister);

    ui->forget_label->SetState("normal", "hover", "", "selected", "selected_hover", "");

    connect(ui->forget_label, &ClickedLable::clicked, this, &LoginDialog::slot_forget_pwd);
}

LoginDialog::~LoginDialog()
{
    delete ui;
    qDebug() << "~LoginDialog";
}

void LoginDialog::slot_forget_pwd()
{
    qDebug() << "slot forget pwd";
    emit switchReset();
}
