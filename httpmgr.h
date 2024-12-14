#ifndef HTTPMGR_H
#define HTTPMGR_H
#include "singleton.h"
#include <QString>
#include <QUrl>
#include <QObject>
#include <QNetworkAccessManager>
#include <QJsonObject>
#include <QJsonDocument>

// CRTP 奇异递归模板，一个类继承自一个以其自身为模板参数的模板类，使得基类能够访问派生类的成员，包括私有和保护成员
class HttpMgr: public QObject, public Singleton<HttpMgr>, public std::enable_shared_from_this<HttpMgr>
{
    // 支持信号与槽机制
    Q_OBJECT
public:
    // 析构是public的是因为在singleton调用析构时，智能指针会调用httpmgr的析构
    ~HttpMgr();
    // 发送http请求
    void PostHttpReq(QUrl url, QJsonObject json, ReqId req_id, Modules mod);
private:
    // 声明singleton为友元，目的是在singleton的GetInstance函数中需要new T(HttpMgr)对象，为了让其可以访问到HttpMgr的构造函数，所以加一下友元
    friend class Singleton<HttpMgr>;
    HttpMgr();
    // 用于发送网络请求并接收网络回复
    QNetworkAccessManager _manager;
private slots:
    // 槽函数的参数可以比信号传参少，但是不能多
    void slot_http_finish(ReqId req_id, QString res, ErrorCodes err, Modules mod);
signals:
    // http发送完后，发送该信号给其他模块
    void sig_http_finish(ReqId req_id, QString res, ErrorCodes err, Modules mod);
    // 发送信号给注册模块，通知其http已收到回复
    void sig_reg_mod_finish(ReqId req_id, QString res, ErrorCodes err);
};

#endif // HTTPMGR_H
