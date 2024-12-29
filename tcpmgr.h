#ifndef TCPMGR_H
#define TCPMGR_H

#include <QTcpSocket>
#include "singleton.h"
#include "global.h"
#include <functional>
#include <QObject>

class TcpMgr: public QObject, public Singleton<TcpMgr>, public std::enable_shared_from_this<TcpMgr>
{
    Q_OBJECT
    friend class Singleton<TcpMgr>;
public:
    ~TcpMgr();

private:
    TcpMgr();
    void initHandlers();
    void handleMsg(ReqId id, int len, QByteArray data);

    QTcpSocket _socket;
    QString _host;
    uint16_t _port;
    // 接收数据缓冲区
    QByteArray _buffer;
    // 用于表示接收的数据是否完整
    bool _b_recv_pending;
    // 数据包的id和长度
    quint16 _message_id;
    quint16 _message_len;

    QMap<ReqId, std::function<void(ReqId id, int len, QByteArray data)>> _handlers;

public slots:
    void slot_tcp_connect(ServerInfo);
    void slot_send_data(ReqId id, QString data);

signals:
    void sig_conn_success(bool b_success);
    void sig_send_data(ReqId id, QString data);
    void sig_switch_chatDlg();
    void sig_login_failed(int);
};

#endif // TCPMGR_H
