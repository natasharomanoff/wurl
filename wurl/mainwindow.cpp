#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QMessageBox>
#include <QMimeDatabase>
#include <QUuid>
#include "wurlutils.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    networkManager = new QNetworkAccessManager(this);

    connect(networkManager,
            SIGNAL(finished(QNetworkReply*)),
            this,
            SLOT(replyFinished(QNetworkReply*)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::replyFinished(QNetworkReply *reply) {
    QByteArray baReply = reply->readAll();
    QString strReply(baReply);

    ui->pteResponseBody->setPlainText(strReply);
    displayResponseHeaders(reply);
    QString headers = WurlUtils::httpHeadersAsString(reply);
    logResponse(*reply, strReply, headers);

}

void MainWindow::onButtonGet() {
    QNetworkRequest request(QUrl(ui->leUrl->text()));
    request.setRawHeader("User-Agent", "POSTER 1.0");
    request.setRawHeader("Content-Type", ui->leContentType->text().toUtf8());
    networkManager->get(request);
    qDebug("onButtonGet");
    logRequest("GET", request);
}

void MainWindow::onButtonPost(){
    QNetworkRequest request(QUrl(ui->leUrl->text()));
    request.setRawHeader("User-Agent", "POSTER 1.0");
    request.setRawHeader("Content-Type", ui->leContentType->text().toUtf8());

    QByteArray data = ui->pteRequestBody->document()->toPlainText().toUtf8();
    networkManager->post(request, data);
    qDebug("onButtonPost");
}

void MainWindow::onButtonPut() {
    QNetworkRequest request(QUrl(ui->leUrl->text()));
    request.setRawHeader("User-Agent", "POSTER 1.0");
    request.setRawHeader("Content-Type", ui->leContentType->text().toUtf8());

    QByteArray data = ui->pteRequestBody->document()->toPlainText().toUtf8();
    networkManager->put(request, data);

    qDebug("onButtonPut");
}

void MainWindow::onButtonSend() {
    qDebug("onButtonSend");
}

void MainWindow::onFileBrowse() {
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                    "./",
                                                    tr("All Files (*)"));
    ui->leFile->setText(fileName);
    QMimeDatabase mimeDb;
    QMimeType mimeType = mimeDb.mimeTypeForFile(fileName, QMimeDatabase::MatchDefault);
    ui->leContentType->setText(mimeType.name());
}

void MainWindow::onAddChangeHeaders() {
    QString name = ui->leHeaderName->text();
    QString value = ui->leHeaderValue->text();

    if ( name.isEmpty() || value.isEmpty())
        return;

    QTableWidgetItem *itemName = new QTableWidgetItem(name);

    QTableWidgetItem *itemValue = new QTableWidgetItem(value);

    int row = ui->twHeaders->rowCount();
    ui->twHeaders->insertRow(row);
    ui->twHeaders->setItem(row, 0, itemName);
    ui->twHeaders->setItem(row, 1, itemValue);
    ui->leHeaderName->clear();
    ui->leHeaderValue->clear();

}

void MainWindow::onAddChangeParameters() {
    qDebug("onAddChangeParameters");
}


void MainWindow::onFileExit() {
    this->close();
}

void MainWindow::onSettingsLoad() {
     qDebug("onSettingsLoad");
}

void MainWindow::onSettingsSave() {
     qDebug("onSettingsSave");
}

void MainWindow::logRequest(QString method, QNetworkRequest &request)
{

    int row = ui->twRequestLog->rowCount();
    currentRequestId = WurlUtils::createId();

    QTableWidgetItem *iId          = new QTableWidgetItem(currentRequestId);
    QTableWidgetItem *iMethod      = new QTableWidgetItem(method);
    QTableWidgetItem *iUrl         = new QTableWidgetItem(request.url().toString());
    QTableWidgetItem *iContentType = new QTableWidgetItem(request.header(QNetworkRequest::ContentTypeHeader).toString());
    QTableWidgetItem *iData        = new QTableWidgetItem(ui->pteRequestBody->document()->toPlainText());


    ui->twRequestLog->insertRow(row);
    ui->twRequestLog->setItem(row, 0, iId);
    ui->twRequestLog->setItem(row, 1, iMethod);
    ui->twRequestLog->setItem(row, 2, iUrl);
    ui->twRequestLog->setItem(row, 3, iContentType);
    ui->twRequestLog->setItem(row, 4, iData);

}

void MainWindow::logResponse(QNetworkReply &reply, QString &data, QString &headers) {

    QTableWidgetItem *itemId = new QTableWidgetItem(currentRequestId);
    QTableWidgetItem *itemCode = new QTableWidgetItem(reply.attribute(QNetworkRequest::HttpStatusCodeAttribute).toString());
    QTableWidgetItem *itemHeader = new QTableWidgetItem(headers);
    QTableWidgetItem *itemData = new QTableWidgetItem(data);

    int row = ui->twResponseLog->rowCount();
    ui->twResponseLog->insertRow(row);
    ui->twResponseLog->setItem(row, 0, itemId);
    ui->twResponseLog->setItem(row, 1, itemCode);
    ui->twResponseLog->setItem(row, 2, itemHeader);
    ui->twResponseLog->setItem(row, 3, itemData);

}

void MainWindow::displayResponseHeaders(QNetworkReply *reply)
{
    ui->twResponseHeader->clearContents();
    ui->twResponseHeader->setRowCount(0);

    QList<QByteArray> baReplyHeaders = reply->rawHeaderList();
    for(int i = 0; i < baReplyHeaders.size(); i++)
    {
        QString headerFieldName(baReplyHeaders[i]);
        QString headerFieldValue(reply->rawHeader(baReplyHeaders[i]));
        QTableWidgetItem *fieldName = new QTableWidgetItem(headerFieldName);
        QTableWidgetItem *fieldValue = new QTableWidgetItem(headerFieldValue);

        int row = ui->twResponseHeader->rowCount();
        ui->twResponseHeader->insertRow(row);
        ui->twResponseHeader->setItem(row, 0, fieldName);
        ui->twResponseHeader->setItem(row, 1, fieldValue);
    }
}



