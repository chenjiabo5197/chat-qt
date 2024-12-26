#include "clickedlable.h"
#include <QMouseEvent>

ClickedLable::ClickedLable(QWidget *parent): QLabel(parent), _curState(ClickLbState::Normal)
{
    // 让鼠标在此区域变成小手样式
    this->setCursor(Qt::PointingHandCursor);
}

ClickedLable::~ClickedLable()
{

}

void ClickedLable::mousePressEvent(QMouseEvent *ev)
{
    if(ev->button() == Qt::LeftButton)
    {
        if(_curState == ClickLbState::Normal)
        {
            qDebug() << "clicked, change to selected hover: " << _selected_hover;
            _curState = ClickLbState::Selected;
            setProperty("state", _selected_hover);
            repolish(this);
            // 更新
            update();
        }
        else
        {
            qDebug() << "clicked, change to normale hover: " << _normal_hover;
            _curState = ClickLbState::Normal;
            setProperty("state", _normal_hover);
            repolish(this);
            update();
        }
        emit clicked();
    }
    // 调用基类的mousePressEvent函数，确保正常事件的处理
    QLabel::mousePressEvent(ev);
}

void ClickedLable::enterEvent(QEnterEvent *event)
{
    if(_curState == ClickLbState::Normal)
    {
        qDebug() << "enter, change to normal hover: " << _normal_hover;
        setProperty("state", _normal_hover);
        repolish(this);
        update();
    }
    else
    {
        qDebug() << "enter, change to selected hover: " << _selected_hover;
        setProperty("state", _selected_hover);
        repolish(this);
        update();
    }
    QLabel::enterEvent(event);
}

void ClickedLable::leaveEvent(QEvent *event)
{
    if(_curState == ClickLbState::Normal)
    {
        qDebug() << "leave, change to normal hover: " << _normal;
        setProperty("state", _normal);
        repolish(this);
        update();
    }
    else
    {
        qDebug() << "leave, change to selected hover: " << _selected;
        setProperty("state", _selected);
        repolish(this);
        update();
    }
    QLabel::leaveEvent(event);
}

void ClickedLable::SetState(QString normal, QString hover, QString press, QString select, QString select_hover, QString select_press)
{
    _normal = normal;
    _normal_hover = hover;
    _normal_press = press;

    _selected = select;
    _selected_hover = select_hover;
    _selected_press = select_press;

    // 设置标签当前的格式为默认状态
    setProperty("state", normal);
    // 更新当前格式
    repolish(this);
}

ClickLbState ClickedLable::GetCurState()
{
    return _curState;
}
