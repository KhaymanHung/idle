#include "client.h"
#include <QDebug>

Client::Client(QObject *parent)
    : QObject(parent)
    , m_tcpSocket(new QTcpSocket(this))
{
    connect(m_tcpSocket, &QTcpSocket::connected, this, &Client::onConnected);
    connect(m_tcpSocket, &QTcpSocket::disconnected, this, &Client::onDisconnected);
    connect(m_tcpSocket, &QTcpSocket::readyRead, this, &Client::onReadyRead);
    connect(m_tcpSocket, &QAbstractSocket::errorOccurred, this, &Client::onError);
}

Client::~Client()
{
    disconnectFromServer();
}

void Client::connectToServer(const QString &host, quint16 port)
{
    qDebug() << "Connecting to server at" << host << ":" << port;
    m_tcpSocket->connectToHost(host, port);
}

void Client::disconnectFromServer()
{
    if (m_tcpSocket->state() == QAbstractSocket::ConnectedState) {
        m_tcpSocket->disconnectFromHost();
        qDebug() << "Disconnecting from server...";
    }
}

void Client::sendMessage(const QString &message)
{
    if (m_tcpSocket->state() == QAbstractSocket::ConnectedState) {
        m_tcpSocket->write(message.toUtf8());
        m_tcpSocket->flush();
    } else {
        qDebug() << "Error: Not connected to server!";
    }
}

bool Client::isConnected() const
{
    return m_tcpSocket->state() == QAbstractSocket::ConnectedState;
}

void Client::onConnected()
{
    qDebug() << "Successfully connected to server!";
    qDebug() << "Type messages to send (or 'quit' to exit):";
    emit connected();
}

void Client::onDisconnected()
{
    qDebug() << "Disconnected from server.";
    emit disconnected();
}

void Client::onReadyRead()
{
    QByteArray data = m_tcpSocket->readAll();
    QString message = QString::fromUtf8(data);
    qDebug().noquote() << message.trimmed();
    emit messageReceived(message);
}

void Client::onError(QAbstractSocket::SocketError socketError)
{
    Q_UNUSED(socketError);
    qDebug() << "Socket error:" << m_tcpSocket->errorString();
    emit errorOccurred(m_tcpSocket->errorString());
}
