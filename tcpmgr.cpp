#include "tcpmgr.h"
#include <QAbstractSocket>
#include <QJsonDocument>

TcpMgr::~TcpMgr()
{

}

TcpMgr::TcpMgr(): _host(""), _port(0), _b_recv_pending(false), _message_id(0), _message_len(0)
{
    connect(&_socket, &QTcpSocket::connected, [&](){
        qDebug() << "Connected to server";
        // 连接建立后发送信号
        emit sig_conn_success(true);
    });

    connect(&_socket, &QTcpSocket::readyRead, [&](){
        // 当有数据可读时，读取所有数据
        // 读取所有数据并追加到缓冲区
        _buffer.append(_socket.readAll());

        QDataStream stream(&_buffer, QIODevice::ReadOnly);
        stream.setVersion(QDataStream::Qt_5_0);

        // 无限循环
        forever{
            // 先解析头部
            if(!_b_recv_pending)
            {
                // 检查缓冲区中的数据是否足够解析出一个消息头(消息id+消息长度)
                // sizeof 的返回值类型是 size_t
                if(_buffer.size() < static_cast<int>(sizeof(quint16) * 2))
                {
                    // 消息长度不够，返回继续接收数据，往_buffer中追加
                    return;
                }

                // 读取消息id和长度，但读取的信息不从缓冲区中移除
                stream >> _message_id >> _message_len;

                // 移除buffer中前4个字节的数据
                // 提取从指定位置开始到末尾的所有数据
                _buffer = _buffer.mid(sizeof(quint16) * 2);

                // 输出读取的数据
                qDebug() << "Message id=" << _message_id << ", message len=" << _message_len;
            }

            // buffer剩余长度是否满足消息体长度，不满足则退出继续等待接收
            if(_buffer.size() < _message_len)
            {
                _b_recv_pending = true;
                return;
            }

            _b_recv_pending = false;
            // 读取消息体
            QByteArray messageBody = _buffer.mid(0, _message_len);
            qDebug() << "receive body msg =" << messageBody;

            _buffer = _buffer.mid(_message_len);
            handleMsg(ReqId(_message_id), _message_len, messageBody);
        }
    });

    // 处理错误
    connect(&_socket, QOverload<QAbstractSocket::SocketError>::of(&QTcpSocket::errorOccurred), [&](QAbstractSocket::SocketError socketError){
        qDebug() << "error=" << _socket.errorString();
        switch (socketError) {
        case QTcpSocket::ConnectionRefusedError:
            qDebug() << "Connection refused";
            emit sig_conn_success(false);
            break;
        case QTcpSocket::RemoteHostClosedError:
            qDebug() << "Remote host closed connection";
            break;
        case QTcpSocket::HostNotFoundError:
            qDebug() << "host not found";
            emit sig_conn_success(false);
            break;
        case QTcpSocket::SocketTimeoutError:
            qDebug() << "connection timeout";
            emit sig_conn_success(false);
            break;
        case QTcpSocket::NetworkError:
            qDebug() << "network error";
            break;
        default:
            qDebug() << "other error";
            break;
        }
    });

    // 处理连接断开
    connect(&_socket, &QTcpSocket::disconnected, [&](){
        qDebug() << "disconnected from server";
    });

    // 连接发送信号用于发送数据
    connect(this, &TcpMgr::sig_send_data, this, &TcpMgr::slot_send_data);

    // 注册处理函数
    initHandlers();

}

void TcpMgr::initHandlers()
{
    _handlers.insert(ID_CHAT_LOGIN_RSP, [this](ReqId id, int len, QByteArray data){
        Q_UNUSED(len);
        qDebug() << "handle id=" << id << ", data=" << data;
        // 将QByteArray转换为QJsonDocument
        QJsonDocument jsonDoc = QJsonDocument::fromJson(data);

        // 检查转换是否成功
        if(jsonDoc.isNull())
        {
            qDebug() << "failed to create QJsonDocument";
            return;
        }

        QJsonObject jsonObj = jsonDoc.object();
        if(!jsonObj.contains("error"))
        {
            int err = ErrorCodes::ERR_JSON;
            qDebug() << "login failed, err is json parse err" << err;
            emit sig_login_failed(err);
            return;
        }

        int err = jsonObj["error"].toInt();
        if(err != ErrorCodes::SUCCESS)
        {
            qDebug() << "login failed, err=" << err;
            emit sig_login_failed(err);
            return;
        }

        emit sig_switch_chatDlg();
    });
}

void TcpMgr::handleMsg(ReqId id, int len, QByteArray data)
{
    auto find_iter = _handlers.find(id);
    if(find_iter == _handlers.end())
    {
        qDebug() << "not found id=" << id << ", to handle";
        return;
    }
    find_iter.value()(id, len, data);
}

void TcpMgr::slot_tcp_connect(ServerInfo si)
{
    qDebug() << "receive tcp connect signal";
    // 尝试连接到服务器
    qDebug() << "connecting to server...";
    _host = si.Host;
    _port = static_cast<uint16_t>(si.Port.toUInt());
    // 连接服务器，连接的结果通过在构造函数里声明的connect里的回调函数通知
    _socket.connectToHost(_host, _port);
}

void TcpMgr::slot_send_data(ReqId id, QString data)
{

}
