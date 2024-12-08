#include "global.h"

/**
 * 定义了一个名为 repolish 的 std::function 对象，它接受一个指向 QWidget 的指针作为参数，并且没有返回值（即返回类型为 void）。
 * 这个 std::function 对象被初始化为一个 lambda 表达式，该表达式对传入的 QWidget 指针执行两个操作：首先调用 unpolish 方法，然后调用 polish 方法。
 * unpolish 方法来撤销对 w 的任何先前样式更改
 * polish 方法来重新应用样式到 w 上
 */
std::function<void(QWidget*)> repolish = [](QWidget* w){
    w->style()->unpolish(w);
    w->style()->polish(w);
};
