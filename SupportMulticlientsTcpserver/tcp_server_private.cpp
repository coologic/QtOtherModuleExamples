#include "tcp_server_private.h"
namespace tcp_server_private {
//构造函数
TcpSocket::TcpSocket(qintptr handle, QObject *parent)
    : QTcpSocket(parent), handle_(handle) {
    this->setSocketDescriptor(handle_);
    //断开连接消息
    connect(this,&TcpSocket::disconnected,
            [&](){
        this->deleteLater();
        emit this->ClientDisconnected(handle_);
    });
}
//主动断开连接槽
void TcpSocket::DisconnectSocket(qintptr handle) {
    if (handle == handle_)
        disconnectFromHost();
}
}


