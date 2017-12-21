#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>
#include "tcp_server.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
signals:
    void ServerRecved(qintptr, QTcpSocket*, const QByteArray &);
private slots:
    void ClientConnected(qintptr handle, QTcpSocket *socket);
    void ClientDisconnected(qintptr handle);
    /**
     * @brief 服务端收到消息的信号
     *    若想要统一管理或做日志处理可连接此信号
     * @param 收到消息的连接句柄
     * @param 收到消息的socket指针
     * @param 收到的消息内容
     */
    void ServerRecvedSlot(qintptr handle, QTcpSocket *socket, const QByteArray &data);
private:
    Ui::MainWindow *ui;
    TcpServer *tcp_server_;
    QStandardItemModel *model_;
};

#endif // MAINWINDOW_H
