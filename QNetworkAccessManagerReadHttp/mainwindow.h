#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QtNetwork>
#include <QFile>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void Get(QUrl u);
private slots:
    void on_pushButton_clicked();
    void finished();
private:
    Ui::MainWindow *ui;
    QNetworkAccessManager manager;
    QUrl url;
    QNetworkReply *reply;
    QString html_text;
};

#endif // MAINWINDOW_H
