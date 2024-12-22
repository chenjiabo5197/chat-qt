#ifndef TIMERBTN_H
#define TIMERBTN_H
#include <QPushButton>
#include <QTimer>

class TimerBtn: public QPushButton
{
public:
    TimerBtn(QWidget *parent = nullptr);
    ~TimerBtn();
    void mouseReleaseEvent(QMouseEvent *e) override;

private:
    // 定时器
    QTimer* _timer;
    // 用于计数
    int _counter;
};

#endif // TIMERBTN_H
