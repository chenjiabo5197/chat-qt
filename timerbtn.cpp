#include "timerbtn.h"
#include <QMouseEvent>

TimerBtn::TimerBtn(QWidget *parent): QPushButton(parent), _counter(10)
{
    _timer = new QTimer(this);

    // 定时器启动后，达到超时时间后，触发回调函数
    // 将一个信号连接到一个 lambda 表达式 时，不需要显式声明接收者（receiver）
    connect(_timer, &QTimer::timeout, [this](){
        _counter--;
        if(_counter <= 0)
        {
            _timer->stop();
            _counter = 10;
            this->setText("获取");
            // 让按钮可用
            this->setEnabled(true);
            return;
        }
        this->setText(QString::number(_counter));
    });
}

TimerBtn::~TimerBtn()
{
    _timer->stop();
}

void TimerBtn::mouseReleaseEvent(QMouseEvent *e)
{
    if(e->button() == Qt::LeftButton)
    {
        // 鼠标左键释放
        qDebug() << "leftbutton release";
        this->setEnabled(false);
        this->setText(QString::number(_counter));
        // 开启计时器，每隔1s触发一次超时事件
        _timer->start(1000);
        // 让外部了解到改按键有clicked事件触发
        emit clicked();
    }
    // 调用基类的函数，确保正常的事件处理
    QPushButton::mouseReleaseEvent(e);
}
