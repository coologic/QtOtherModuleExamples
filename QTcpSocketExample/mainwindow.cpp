#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTcpServer>
#include <QTcpSocket>
#include <QDebug>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {
    ui->setupUi(this);
    server = new QTcpServer();
    connect(server,
        &QTcpServer::newConnection,
        this,
        &MainWindow::server_New_Connect);//监听
    if(!server->listen(QHostAddress::Any, 80100)) {
        qDebug()<<server->errorString();    //错误信息
    }
    //客户端建立
    m_socket = new QTcpSocket;
    m_socket->connectToHost("127.0.0.1",80100,QTcpSocket::ReadWrite);
    if(!m_socket->waitForConnected()) {
        qDebug()<<"client connect error";
    }
    m_socket->write("i'm in");
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::server_New_Connect() {
    //获取客户端连接
    m_server_socket = server->nextPendingConnection();//根据当前新连接创建一个QTepSocket
    //连接QTcpSocket的信号槽，以读取新数据
    QObject::connect(m_server_socket, &QTcpSocket::readyRead, this, &MainWindow::socket_Read_Data);
    //当断开连接时发出的信号
//    QObject::connect(socket_temp, &QTcpSocket::disconnected, this, &MainWindow::socket_Disconnected);
    qDebug()<<"new clinet connect in";
}

void MainWindow::socket_Read_Data(){
    qDebug()<<m_server_socket->readAll();
}
