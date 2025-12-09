#include "serverwindow.h"
#include "ui_serverwindow.h"
#include <QDateTime>

ServerWindow::ServerWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ServerWindow)
    , m_server(new Server(this))
    , m_isRunning(false)
{
    ui->setupUi(this);
    
    connect(ui->btnStartStop, &QPushButton::clicked, this, &ServerWindow::onStartStopClicked);
    connect(ui->btnSend, &QPushButton::clicked, this, &ServerWindow::onSendClicked);
    connect(ui->lineEditMessage, &QLineEdit::returnPressed, this, &ServerWindow::onSendClicked);
    
    // Connect Server signals
    connect(m_server, &Server::serverStarted, this, &ServerWindow::onServerStarted);
    connect(m_server, &Server::serverStopped, this, &ServerWindow::onServerStopped);
    connect(m_server, &Server::clientConnected, this, &ServerWindow::onClientConnected);
    connect(m_server, &Server::clientDisconnected, this, &ServerWindow::onClientDisconnected);
    connect(m_server, &Server::messageReceived, this, &ServerWindow::onMessageReceived);
    
    updateStatus(false);
    appendLog(QStringLiteral("\u4f3a\u670d\u5668\u6e96\u5099\u5c31\u7dd2"));
}

ServerWindow::~ServerWindow()
{
    if (m_isRunning) {
        m_server->stopServer();
    }
    delete ui;
}

void ServerWindow::onStartStopClicked()
{
    if (!m_isRunning) {
        bool ok;
        quint16 port = ui->lineEditPort->text().toUShort(&ok);
        if (!ok || port == 0) {
            appendLog(QStringLiteral("\u932f\u8aa4: \u7121\u6548\u7684\u57e0\u865f"));
            return;
        }
        
        if (m_server->startServer(port)) {
            m_isRunning = true;
            updateStatus(true);
            appendLog(QStringLiteral("\u4f3a\u670d\u5668\u5df2\u5728\u57e0 %1 \u4e0a\u555f\u52d5").arg(port));
            ui->lineEditPort->setEnabled(false);
        } else {
            appendLog(QStringLiteral("\u932f\u8aa4: \u7121\u6cd5\u555f\u52d5\u4f3a\u670d\u5668"));
        }
    } else {
        m_server->stopServer();
        m_isRunning = false;
        updateStatus(false);
        appendLog(QStringLiteral("\u4f3a\u670d\u5668\u5df2\u505c\u6b62"));
        ui->lineEditPort->setEnabled(true);
        ui->labelClients->setText(QStringLiteral("\u9023\u7dda\u5ba2\u6236\u7aef: 0"));
    }
}

void ServerWindow::onSendClicked()
{
    QString message = ui->lineEditMessage->text().trimmed();
    if (message.isEmpty()) {
        return;
    }
    
    m_server->sendMessageToAll(QStringLiteral("[\u4f3a\u670d\u5668]: ") + message + "\n");
    appendLog(QStringLiteral("\u5ee3\u64ad: ") + message);
    ui->lineEditMessage->clear();
}

void ServerWindow::onServerStarted()
{
    appendLog(QStringLiteral("\u4f3a\u670d\u5668\u555f\u52d5\u6210\u529f"));
}

void ServerWindow::onServerStopped()
{
    appendLog(QStringLiteral("\u4f3a\u670d\u5668\u5df2\u505c\u6b62"));
}

void ServerWindow::onClientConnected(int count)
{
    ui->labelClients->setText(QStringLiteral("\u9023\u7dda\u5ba2\u6236\u7aef: %1").arg(count));
    appendLog(QStringLiteral("\u65b0\u5ba2\u6236\u7aef\u9023\u7dda (\u7e3d\u6578: %1)").arg(count));
}

void ServerWindow::onClientDisconnected(int count)
{
    ui->labelClients->setText(QStringLiteral("\u9023\u7dda\u5ba2\u6236\u7aef: %1").arg(count));
    appendLog(QStringLiteral("\u5ba2\u6236\u7aef\u65b7\u7dda (\u7e3d\u6578: %1)").arg(count));
}

void ServerWindow::onMessageReceived(const QString &message)
{
    appendLog(QStringLiteral("\u6536\u5230: ") + message);
}

void ServerWindow::appendLog(const QString &message)
{
    QString timestamp = QDateTime::currentDateTime().toString("hh:mm:ss");
    ui->textEditLog->append(QString("[%1] %2").arg(timestamp, message));
}

void ServerWindow::updateStatus(bool running)
{
    if (running) {
        ui->labelStatus->setText(QStringLiteral("\u72c0\u614b: \u57f7\u884c\u4e2d"));
        ui->labelStatus->setStyleSheet("font-weight: bold; color: green;");
        ui->btnStartStop->setText(QStringLiteral("\u505c\u6b62\u4f3a\u670d\u5668"));
        ui->btnSend->setEnabled(true);
    } else {
        ui->labelStatus->setText(QStringLiteral("\u72c0\u614b: \u672a\u555f\u52d5"));
        ui->labelStatus->setStyleSheet("font-weight: bold; color: red;");
        ui->btnStartStop->setText(QStringLiteral("\u555f\u52d5\u4f3a\u670d\u5668"));
        ui->btnSend->setEnabled(false);
    }
}
