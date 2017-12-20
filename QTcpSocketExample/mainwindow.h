#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
class QTcpSocket;
class QTcpServer;
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
private slots:
    void server_New_Connect();
    void socket_Read_Data();

private:
    Ui::MainWindow *ui;
    QTcpSocket *m_socket,*m_server_socket;
    QTcpServer *server;
};

#endif // MAINWINDOW_H
