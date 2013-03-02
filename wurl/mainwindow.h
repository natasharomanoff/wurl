#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork/QNetworkAccessManager>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void logRequest(QString method, QNetworkRequest &request);
    void logResponse(QNetworkReply &reply, QString &data, QString &headers);
    void displayResponseHeaders(QNetworkReply *reply);

public slots:
    void replyFinished(QNetworkReply *reply);
    void onButtonGet();
    void onButtonPost();
    void onButtonPut();
    void onButtonSend();
    void onFileBrowse();
    void onAddChangeHeaders();
    void onAddChangeParameters();


    // Actions
    void onFileExit();
    void onSettingsLoad();
    void onSettingsSave();
private:
    QNetworkAccessManager *networkManager;
    QString currentRequestId;
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
