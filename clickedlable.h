#ifndef CLICKEDLABLE_H
#define CLICKEDLABLE_H

#include <QLabel>
#include "global.h"

/******************************************************************************
 *
 * @file       clickedlable.h
 * @brief      自定义label，可以在鼠标悬浮和点击时切换不同的渲染
 *
 * @author     陈佳波
 * @date       2024/12/28
 * @history
 *****************************************************************************/

class ClickedLable : public QLabel
{
    Q_OBJECT
public:
    ClickedLable(QWidget *parent=nullptr);
    ~ClickedLable();

    virtual void mousePressEvent(QMouseEvent *ev) override;
    virtual void enterEvent(QEnterEvent *event) override;
    virtual void leaveEvent(QEvent *event) override;

    void SetState(QString normal="", QString hover="", QString press="",
                  QString select="", QString select_hover="", QString select_press="");
    ClickLbState GetCurState();

private:
    QString _normal;
    // 悬浮状态
    QString _normal_hover;
    QString _normal_press;

    QString _selected;
    QString _selected_hover;
    QString _selected_press;

    ClickLbState _curState;

signals:
    void clicked();
};

#endif // CLICKEDLABLE_H
