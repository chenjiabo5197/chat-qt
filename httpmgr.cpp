#include "httpmgr.h"

HttpMgr::~HttpMgr()
{

}

HttpMgr::HttpMgr()
{
    connect(this, &HttpMgr::sig_http_finish, this, &HttpMgr::slot_http_finish);
}

void HttpMgr::PostHttpReq(QUrl url, QJsonObject json, ReqId req_id, Modules mod)
{
    // 使用QJsonDocument封装JSON对象,将JSON文档转换为字符串表示形式
    QByteArray data = QJsonDocument(json).toJson();
    QNetworkRequest request(url);
    // 设置请求类型
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    // 设置数据长度，可以自动处理大、小端
    request.setHeader(QNetworkRequest::ContentLengthHeader, QByteArray::number(data.length()));
    // 从类的成员函数内部获取指向该类实例的 std::shared_ptr
    auto self = shared_from_this();
    // 异步通信的方式
    QNetworkReply * reply = _manager.post(request, data);
    // 收到应答的finish信号，执行lambda表达式,使用self是为了防止HttpMgr被回收导致出问题
    QObject::connect(reply, &QNetworkReply::finished, [self, reply, req_id, mod](){
        // 处理错误情况
        if(reply->error() != QNetworkReply::NoError){
            qDebug() << reply->errorString();
            // 发送信号通知完成
            // 调用emit后跟一个信号名和可选的参数时，Qt会查找所有连接到该信号的槽，并依次调用它们。
            // 这个过程是自动的，由Qt的元对象系统（Meta-Object System）管理
            emit self->sig_http_finish(req_id, "", ErrorCodes::ERR_NETWORK, mod);
            // 回收reply
            // 安排对象在其事件循环的下一个迭代中被删除。当调用一个对象的 deleteLater() 方法时，该对象并不会立即被删除；
            // 相反，Qt 会将这个删除操作排队，等到当前事件处理完成后，在适当的时候（通常是在当前事件循环迭代结束时）才实际执行删除操作。
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

void HttpMgr::slot_http_finish(ReqId req_id, QString res, ErrorCodes err, Modules mod)
{
    if(mod == Modules::REGISTERMOD)
    {
        // 发送信号通知指定模块http的相应结束了
        emit sig_reg_mod_finish(req_id, res, err);
    }
    if(mod == Modules::RESETMOD)
    {
        // 发送信号通知指定模块http的相应结束了
        emit sig_reset_mod_finish(req_id, res, err);
    }
    if(mod == Modules::LOGINMOD)
    {
        // 发送信号通知指定模块http的相应结束了
        emit sig_login_mod_finish(req_id, res, err);
    }
}
