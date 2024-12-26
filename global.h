#ifndef GLOBAL_H
#define GLOBAL_H
#include <QWidget>
#include <functional>
#include <QRegularExpression>
#include "QStyle"
#include <memory>
#include <iostream>
#include <mutex>
#include <QByteArray>
#include <QNetworkReply>
#include <QJsonObject>
#include <QDir>
#include <QSettings>

/**
 * @brief repolish  用于刷新qss的函数
 */
extern std::function<void(QWidget *)> repolish;

/**
 * @brief xorString 获取字符串的异或值，用于密码加密
 */
extern std::function<QString(QString)> xorString;

enum ReqId{
    ID_GET_VARIFY_CODE = 1001,  // 获取验证码
    ID_REG_USER = 1002,         // 注册用户
    ID_RESET_PWD = 1003,        // 重置密码
    ID_LOGIN_USER = 1004,       // 用户登录
    ID_CHAT_LOGIN = 1005,       // 登录聊天服务器
    ID_CHAT_LOGIN_RSP = 1006,   // 登录聊天服务器回包
};

enum Modules{
    REGISTERMOD = 0,        // 注册模块
    RESETMOD = 1,           // 重置模块
};

enum TipErr{
    TIP_SUCCESS = 0,        // 成功
    TIP_EMAIL_ERR = 1,      // 邮箱错误
    TIP_PWD_ERR = 2,        // 密码错误
    TIP_CONFIRM_ERR = 3,    // 确认密码错误
    TIP_PWD_CONFIRM = 4,    // 确认密码与密码不一致
    TIP_VERIFY_ERR = 5,     // 验证码错误
    TIP_USER_ERR = 6        // 用户名错误
};

enum ErrorCodes{
    SUCCESS = 0,
    ERR_JSON = 1,       // json解析失败
    ERR_NETWORK = 2,    // 网络错误
};

enum ClickLbState{
    Normal = 0,         // 闭眼，即密码不显示
    Selected = 1,       // 睁眼，显示密码
};

extern QString gate_url_prefix;

#endif // GLOBAL_H
