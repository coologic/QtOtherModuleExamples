#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTcpSocket>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {
    ui->setupUi(this);
    connect(this, &MainWindow::ServerRecved, this, MainWindow::ServerRecvedSlot);
    model_ = new QStandardItemModel(this);
    ui->tableView->setModel(model_);
    ui->tableView->setColumnWidth(0,400);
    tcp_server_ = new TcpServer(this);
    if(!tcp_server_->listen(QHostAddress::Any, 8000)) {
        qDebug()<<tcp_server_->errorString();    //错误信息
        return;
    }
    qDebug()<<"listening";    //错误信息
    model_->insertRow(0,new QStandardItem("listening"));
    connect(tcp_server_,
        &TcpServer::ClientConnected,
        this,
        &MainWindow::ClientConnected);//监听
    connect(tcp_server_,
        &TcpServer::ClientDisconnected,
        this,
        &MainWindow::ClientDisconnected);//监听
}

MainWindow::~MainWindow() {
    tcp_server_->close();
    delete ui;
}

void MainWindow::ClientConnected(qintptr handle, QTcpSocket* socket) {
    model_->insertRow(0,new QStandardItem(QString::number(handle)+" connected"));
    connect(socket, &QTcpSocket::readyRead,
            [=]() {
        emit ServerRecved(handle, socket, socket->readAll());
    });
}

void MainWindow::ClientDisconnected(qintptr handle) {
    model_->insertRow(0,new QStandardItem(QString::number(handle)+" disconnected"));
}

void MainWindow::ServerRecvedSlot(qintptr handle,
                                  QTcpSocket *socket,
                                  const QByteArray &data) {
    Q_UNUSED(handle);
    qDebug()<<socket->peerAddress().toString()<<socket->peerPort()<<data;
    QString send_data = QString("%1 %2 %3").
            arg(socket->peerAddress().toString()).
            arg(socket->peerPort()).
            arg(QString(data));
    model_->insertRow(0,new QStandardItem(send_data));
    socket->write(send_data.toLatin1());
}
