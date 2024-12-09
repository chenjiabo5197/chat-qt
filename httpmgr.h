#ifndef HTTPMGR_H
#define HTTPMGR_H
#include "singleton.h"
#include <QString>
#include <QUrl>
#include <QObject>
#include <QNetworkAccessManager>
#include <QJsonObject>
#include <QJsonDocument>

// CRTP 奇异递归
class HttpMgr: public QObject, public Singleton<HttpMgr>, public std::enable_shared_from_this<HttpMgr>
{
    // 支持信号与槽机制
    Q_OBJECT
public:
    // 析构是public的是因为在singleton调用析构时，智能指针会调用httpmgr的析构
    ~HttpMgr();
private:
    // 声明singleton为友元，目的是在singleton的GetInstance函数中需要new T(HttpMgr)对象，为了让其可以访问到HttpMgr的构造函数，所以加一下友元
    friend class Singleton<HttpMgr>;
    HttpMgr();
    QNetworkAccessManager _manager;

    // 发送http请求
    void PostHttpReq(QUrl url, QJsonObject json, ReqId req_id, Modules mod);

signals:
    // http发送完后，发送该信号给其他模块
    void sig_http_finish(ReqId req_id, QString res, ErrorCodes err, Modules mod);
};

#endif // HTTPMGR_H
