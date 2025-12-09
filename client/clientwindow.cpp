#include "clientwindow.h"
#include "ui_clientwindow.h"
#include <QDateTime>

ClientWindow::ClientWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ClientWindow)
    , m_client(new Client(this))
    , m_isConnected(false)
{
    ui->setupUi(this);
    
    connect(ui->btnConnect, &QPushButton::clicked, this, &ClientWindow::onConnectClicked);
    connect(ui->btnSend, &QPushButton::clicked, this, &ClientWindow::onSendClicked);
    connect(ui->lineEditMessage, &QLineEdit::returnPressed, this, &ClientWindow::onSendClicked);
    
    // Connect Client signals
    connect(m_client, &Client::connected, this, &ClientWindow::onClientConnected);
    connect(m_client, &Client::disconnected, this, &ClientWindow::onClientDisconnected);
    connect(m_client, &Client::messageReceived, this, &ClientWindow::onMessageReceived);
    connect(m_client, &Client::errorOccurred, this, &ClientWindow::onClientError);
    
    updateStatus(false);
    appendMessage(QStringLiteral("\u6e96\u5099\u9023\u7dda\u5230\u4f3a\u670d\u5668..."));
}

ClientWindow::~ClientWindow()
{
    if (m_isConnected) {
        m_client->disconnectFromServer();
    }
    delete ui;
}

void ClientWindow::onConnectClicked()
{
    if (!m_isConnected) {
        QString host = ui->lineEditHost->text().trimmed();
        bool ok;
        quint16 port = ui->lineEditPort->text().toUShort(&ok);
        
        if (host.isEmpty() || !ok || port == 0) {
            appendMessage(QStringLiteral("\u932f\u8aa4: \u7121\u6548\u7684\u4e3b\u6a5f\u6216\u57e0\u865f"));
            return;
        }
        
        m_client->connectToServer(host, port);
        appendMessage(QStringLiteral("\u6b63\u5728\u9023\u7dda\u5230 %1:%2...").arg(host).arg(port));
    } else {
        m_client->disconnectFromServer();
        appendMessage(QStringLiteral("\u6b63\u5728\u65b7\u958b\u9023\u7dda..."));
    }
}

void ClientWindow::onSendClicked()
{
    if (!m_isConnected) {
        return;
    }
    
    QString message = ui->lineEditMessage->text().trimmed();
    if (message.isEmpty()) {
        return;
    }
    
    m_client->sendMessage(message + "\n");
    appendMessage(QStringLiteral("\u6211: ") + message);
    ui->lineEditMessage->clear();
}

void ClientWindow::onClientConnected()
{
    m_isConnected = true;
    updateStatus(true);
    appendMessage(QStringLiteral("\u6210\u529f\u9023\u7dda\u5230\u4f3a\u670d\u5668\uff01"));
}

void ClientWindow::onClientDisconnected()
{
    m_isConnected = false;
    updateStatus(false);
    appendMessage(QStringLiteral("\u5df2\u65b7\u958b\u9023\u7dda"));
}

void ClientWindow::onMessageReceived(const QString &message)
{
    appendMessage(message.trimmed());
}

void ClientWindow::onClientError(const QString &error)
{
    appendMessage(QStringLiteral("\u932f\u8aa4: ") + error);
}

void ClientWindow::appendMessage(const QString &message)
{
    QString timestamp = QDateTime::currentDateTime().toString("hh:mm:ss");
    ui->textEditMessages->append(QString("[%1] %2").arg(timestamp, message));
}

void ClientWindow::updateStatus(bool connected)
{
    m_isConnected = connected;
    
    if (connected) {
        ui->labelStatus->setText(QStringLiteral("\u72c0\u614b: \u5df2\u9023\u7dda"));
        ui->labelStatus->setStyleSheet("font-weight: bold; color: green;");
        ui->btnConnect->setText(QStringLiteral("\u65b7\u958b\u9023\u7dda"));
        ui->lineEditHost->setEnabled(false);
        ui->lineEditPort->setEnabled(false);
        ui->lineEditMessage->setEnabled(true);
        ui->btnSend->setEnabled(true);
    } else {
        ui->labelStatus->setText(QStringLiteral("\u72c0\u614b: \u672a\u9023\u7dda"));
        ui->labelStatus->setStyleSheet("font-weight: bold; color: red;");
        ui->btnConnect->setText(QStringLiteral("\u9023\u7dda"));
        ui->lineEditHost->setEnabled(true);
        ui->lineEditPort->setEnabled(true);
        ui->lineEditMessage->setEnabled(false);
        ui->btnSend->setEnabled(false);
    }
}
