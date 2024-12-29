#ifndef TIMERBTN_H
#define TIMERBTN_H
#include <QPushButton>
#include <QTimer>

/******************************************************************************
 *
 * @file       timerbtn.h
 * @brief      自定义button，可以在button上渲染倒计时
 *
 * @author     陈佳波
 * @date       2024/12/28
 * @history
 *****************************************************************************/

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
