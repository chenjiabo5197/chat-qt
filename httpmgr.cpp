#include "httpmgr.h"

HttpMgr::~HttpMgr()
{

}

HttpMgr::HttpMgr()
{

}

void HttpMgr::PostHttpReq(QUrl url, QJsonObject json, ReqId req_id, Modules mod)
{
    QByteArray data = QJsonDocument(json).toJson();
    QNetworkRequest request(url);
    // 设置请求类型
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    // 设置数据长度，可以自动处理大、小端
    request.setHeader(QNetworkRequest::ContentLengthHeader, QByteArray::number(data.length()));
    // 从类的成员函数内部获取指向该类实例的 std::shared_ptr
    auto self = shared_from_this();
    QNetworkReply * reply = _manager.post(request, data);
    // 收到应答的finish信号，执行lambda表达式,使用self是为了防止HttpMgr被回收导致出问题
    QObject::connect(reply, &QNetworkReply::finished, [self, reply, req_id, mod](){
        // 处理错误情况
        if(reply->error() != QNetworkReply::NoError){
            qDebug() << reply->errorString();
            // 发送信号通知完成
            emit self->sig_http_finish(req_id, "", ErrorCodes::ERR_NETWORK, mod);
            // 回收reply
            reply->deleteLater();
            return;
        }
        // 无错误
        QString res = reply->readAll();
        // 发送信号通知完成
        emit self->sig_http_finish(req_id, res, ErrorCodes::SUCCESS, mod);
        reply->deleteLater();
        return;
    });
}
