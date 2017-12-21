#include "tcp_server.h"
#include "tcp_server_private.h"
//构造函数
TcpServer::TcpServer(QObject *parent)
    : QTcpServer(parent),
      private_(new tcp_server_private::TcpServerPrivate) {
}
//析构函数
TcpServer::~TcpServer() {
    for(tcp_server_private::TcpSocket *client : private_->clients.values()) {
        client->disconnectFromHost();
        auto handle = client->socketDescriptor();
        client->deleteLater();
        //告知其他调用者 当前socket断开，避免有需要在socket后执行的方法
        emit ClientDisconnected(handle);
    }
    if(private_)
        delete private_;
    this->close();
}
//重写-有连接到来
void TcpServer::incomingConnection(qintptr handle) {
    //超出最大练级数量关闭连接
    if (private_->clients.size() > maxPendingConnections()) {
        QTcpSocket tcp;
        tcp.setSocketDescriptor(handle);
        tcp.disconnectFromHost();
        return;
    }
    auto client_socket = new tcp_server_private::TcpSocket(handle);
    Q_ASSERT(client_socket->socketDescriptor() == handle);
    //socket断开连接的信号与server槽连接
    connect(client_socket,
            &tcp_server_private::TcpSocket::ClientDisconnected,
            this,
            &TcpServer::ClientDisconnectedSlot);
    //主动断开连接的操作
    connect(this,
            &TcpServer::InitiativeDisConnectClient,
            client_socket,
            &tcp_server_private::TcpSocket::DisconnectSocket);
    //map记录
    private_->clients.insert(handle, client_socket);
    qDebug()<<handle<<"connected";
    emit ClientConnected(handle, client_socket);
}
//客户端已断开槽
void TcpServer::ClientDisconnectedSlot(qintptr handle) {
    //map中移除
    private_->clients.remove(handle);
    qDebug()<<handle<<"disconnected";
    //发出信号
    emit ClientDisconnected(handle);
}
