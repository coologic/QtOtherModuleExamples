#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QHostAddress>
#include <QUdpSocket>
#include <QHostAddress>
#include <QDebug>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {
    ui->setupUi(this);
    QUdpSocket *m_server=new QUdpSocket;
    m_server->bind(8000);
    m_server->open(QUdpSocket::ReadWrite);
    connect(m_server,
            &QUdpSocket::readyRead,
            this,
            [=](){
        while (m_server->hasPendingDatagrams()) {
            QByteArray datagram;
            QHostAddress address;
            quint16 port = 8000;
            datagram.resize(m_server->pendingDatagramSize());
            m_server->readDatagram(datagram.data(),
                                   datagram.size(),
                                   &address,
                                   &port);
            qDebug()<<"server rev:"<<datagram;
            return;
        }});
    QUdpSocket *m_socket=new QUdpSocket;
    m_socket->writeDatagram("client in", QHostAddress::LocalHost, 8000);
    m_socket->waitForBytesWritten();
}

MainWindow::~MainWindow() {
    delete ui;
}
