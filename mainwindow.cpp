#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // 新建对象时给其添加上父对象(this)，则qt在回收父对象时，自动回收该对象，可以不需要再析构函数中手动回收，手动回收有概率出core
    _login_dlg = new LoginDialog(this);
    setCentralWidget(_login_dlg);
    // _login_dlg->show();

    // 创建和注册消息连接
    connect(_login_dlg, &LoginDialog::switchRegister, this, &MainWindow::SlotSwitchReg);
    _reg_dlg = new RegisterDialog(this);

    // 添加上父对象后，若不设置此属性，则_login_dlg/_reg_dlg弹窗是独立的，不会嵌入到main弹窗里去
    // Qt::CustomizeWindowHint：这个标志允许窗口被高度定制，包括改变窗口的边框、标题栏等。它通常用于创建具有特殊外观或行为的窗口，比如无边框窗口或自定义标题栏的窗口。
    // Qt::FramelessWindowHint：这个标志用于创建一个无边框的窗口。没有边框意味着窗口没有标准的标题栏和边框，这通常用于创建全屏应用或自定义外观的窗口。
    _login_dlg->setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);
    _reg_dlg->setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);
    _reg_dlg->hide();
}

MainWindow::~MainWindow()
{
    delete ui;
    // if(_login_dlg)
    // {
    //     delete _login_dlg;
    //     _login_dlg = nullptr;
    // }

    // if(_reg_dlg)
    // {
    //     delete _reg_dlg;
    //     _reg_dlg = nullptr;
    // }
}

void MainWindow::SlotSwitchReg()
{
    setCentralWidget(_reg_dlg);
    _login_dlg->hide();
    _reg_dlg->show();
}
