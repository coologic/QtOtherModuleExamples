#ifndef TCP_SERVER_PRIVATE_H
#define TCP_SERVER_PRIVATE_H
#include <QTcpSocket>
namespace tcp_server_private {
class TcpSocket : public QTcpSocket {
    Q_OBJECT
public:
    /**
     * @brief 构造函数
     * @param socketDescriptor 连接句柄
     * @param parent 父QObject
     */
    TcpSocket(qintptr handle, QObject *parent = 0);
signals:
    /*
     * 已断开连接信号
     */
    void ClientDisconnected(qintptr);
public slots:
    /**
     * @brief 断开连接
     * @param handle 连接句柄
     */
    void DisconnectSocket(qintptr handle);
private:
    qintptr handle_;
};
/**
 * @brief Tcp多客户端服务器私有类
 */
class TcpServerPrivate {
public:
    QMap<int, TcpSocket *> clients; ///所有连接的map
};
}//tcp_server_private
#endif // TCP_SERVER_PRIVATE_H
