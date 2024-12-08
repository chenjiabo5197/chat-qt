#ifndef GLOBAL_H
#define GLOBAL_H
#include <QWidget>
#include <functional>
#include <QRegularExpression>
#include "QStyle"

/**
 * @brief repolish  用于刷新qss的函数
 */
extern std::function<void(QWidget *)> repolish;


#endif // GLOBAL_H
