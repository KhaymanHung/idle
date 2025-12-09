#ifndef SERVERWINDOW_H
#define SERVERWINDOW_H

#include <QMainWindow>
#include "server.h"

QT_BEGIN_NAMESPACE
namespace Ui { class ServerWindow; }
QT_END_NAMESPACE

class ServerWindow : public QMainWindow
{
    Q_OBJECT

public:
    ServerWindow(QWidget *parent = nullptr);
    ~ServerWindow();

private slots:
    void onStartStopClicked();
    void onSendClicked();
    void onServerStarted();
    void onServerStopped();
    void onClientConnected(int count);
    void onClientDisconnected(int count);
    void onMessageReceived(const QString &message);

private:
    Ui::ServerWindow *ui;
    Server *m_server;
    bool m_isRunning;
    
    void appendLog(const QString &message);
    void updateStatus(bool running);
};

#endif // SERVERWINDOW_H
