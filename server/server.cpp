#include "server.h"
#include <QDebug>

Server::Server(QObject *parent)
    : QObject(parent)
    , m_tcpServer(new QTcpServer(this))
{
    connect(m_tcpServer, &QTcpServer::newConnection, this, &Server::onNewConnection);
}

Server::~Server()
{
    stopServer();
}

bool Server::startServer(quint16 port)
{
    if (m_tcpServer->listen(QHostAddress::Any, port)) {
        qDebug() << "Server started on port:" << port;
        qDebug() << "Waiting for clients to connect...";
        emit serverStarted();
        return true;
    } else {
        qDebug() << "Server failed to start:" << m_tcpServer->errorString();
        return false;
    }
}

void Server::stopServer()
{
    // 斷開所有客戶端連線
    for (QTcpSocket *client : m_clients) {
        client->disconnectFromHost();
        client->deleteLater();
    }
    m_clients.clear();
    
    // 停止伺服器
    if (m_tcpServer->isListening()) {
        m_tcpServer->close();
        qDebug() << "Server stopped.";
        emit serverStopped();
    }
}

void Server::onNewConnection()
{
    while (m_tcpServer->hasPendingConnections()) {
        QTcpSocket *clientSocket = m_tcpServer->nextPendingConnection();
        m_clients.append(clientSocket);
        
        connect(clientSocket, &QTcpSocket::disconnected, this, &Server::onClientDisconnected);
        connect(clientSocket, &QTcpSocket::readyRead, this, &Server::onReadyRead);
        
        qDebug() << "New client connected:" << clientSocket->peerAddress().toString() 
                 << ":" << clientSocket->peerPort();
        qDebug() << "Total clients:" << m_clients.size();
        
        emit clientConnected(m_clients.size());
        
        // 向新客戶端發送歡迎訊息
        QString welcomeMsg = QString("Welcome to server! You are client #%1\n").arg(m_clients.size());
        sendMessageToClient(clientSocket, welcomeMsg);
    }
}

void Server::onClientDisconnected()
{
    QTcpSocket *clientSocket = qobject_cast<QTcpSocket*>(sender());
    if (clientSocket) {
        qDebug() << "Client disconnected:" << clientSocket->peerAddress().toString() 
                 << ":" << clientSocket->peerPort();
        
        m_clients.removeOne(clientSocket);
        clientSocket->deleteLater();
        
        qDebug() << "Total clients:" << m_clients.size();
        emit clientDisconnected(m_clients.size());
    }
}

void Server::onReadyRead()
{
    QTcpSocket *clientSocket = qobject_cast<QTcpSocket*>(sender());
    if (clientSocket) {
        QByteArray data = clientSocket->readAll();
        QString message = QString::fromUtf8(data);
        
        qDebug() << "Received from" << clientSocket->peerAddress().toString() 
                 << ":" << message.trimmed();
        
        emit messageReceived(message.trimmed());
        
        // 將訊息廣播給所有客戶端
        QString broadcastMsg = QString("[Client %1:%2]: %3")
                                .arg(clientSocket->peerAddress().toString())
                                .arg(clientSocket->peerPort())
                                .arg(message);
        broadcastMessage(broadcastMsg);
    }
}

void Server::sendMessageToClient(QTcpSocket *client, const QString &message)
{
    if (client && client->state() == QAbstractSocket::ConnectedState) {
        client->write(message.toUtf8());
        client->flush();
    }
}

void Server::sendMessageToAll(const QString &message)
{
    broadcastMessage(message);
}

void Server::broadcastMessage(const QString &message)
{
    for (QTcpSocket *client : m_clients) {
        sendMessageToClient(client, message);
    }
}
