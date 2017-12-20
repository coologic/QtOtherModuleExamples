#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {
    ui->setupUi(this);
    reply = Q_NULLPTR;
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::Get(QUrl u) {
    QNetworkRequest request;
    url=u;
    request.setUrl(url);
    if(reply != Q_NULLPTR) {//更改reply指向位置钱一定要保证之前的定义了自动delete
        reply->deleteLater();
    }
    reply = manager.get(request);
    qDebug() << "start get";
    connect(reply, &QNetworkReply::finished, this, &MainWindow::finished);
}

void MainWindow::on_pushButton_clicked() {
    html_text = "";
    Get(QUrl("http://www.sina.com/"));

}

void MainWindow::finished() {
    QByteArray bytes = reply->readAll();
    const QVariant redirectionTarget = reply->attribute(QNetworkRequest::RedirectionTargetAttribute);
    reply->deleteLater();
    reply = Q_NULLPTR;
    if (!redirectionTarget.isNull()) {//如果网址跳转重新请求
        const QUrl redirectedUrl = url.resolved(redirectionTarget.toUrl());
        qDebug()<<"redirectedUrl:"<<redirectedUrl.url();
        Get(redirectedUrl);
        return;
    }
    qDebug()<<"finished";
    html_text = bytes;
    qDebug()<<"get ready,read size:"<<html_text.size();
    ui->textEdit->setPlainText(html_text);
//    QFile f("result.html");//写出文件
//    f.open(QFile::WriteOnly);
//    f.write(bytes);
//    f.close();
}
