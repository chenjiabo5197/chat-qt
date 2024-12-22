#include "clickedlable.h"
#include <QMouseEvent>

ClickedLable::ClickedLable(QWidget *parent): QLabel(parent), _curState(ClickLbState::Normal)
{

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

}

void ClickedLable::leaveEvent(QEvent *event)
{

}

void ClickedLable::SetState(QString normal, QString hover, QString press, QString select, QString select_hover, QString select_press)
{

}

ClickLbState ClickedLable::GetCurState()
{

}
