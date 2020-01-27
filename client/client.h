#ifndef CLIENT_H
#define CLIENT_H

#include <QtCore/QByteArray>
#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QtGlobal>  // quint16
#include <QtNetwork/QUdpSocket>

#include "common/protocol.h"  // Message

class Client : public QObject
{
    Q_OBJECT

public:
    Client();

    void setName(const QString& name);

    // get and set server network params
    QString address() const;
    void setAddress(const QString& address);
    quint16 port() const;
    void setPort(quint16 port);

    bool connect(const QString& address, quint16 port);
    bool connect();
    bool disconnect();

    // send message to server
    Q_SLOT void sendMessage(const QString& message);

private:
    void sendHello();

    void processPendingDatagram();
    void parseMessage(const Message& datagram);

    // validate input address
    bool checkAddress(const QString& address);

    // datagram processing
    Q_SLOT void onReadyRead();

    Q_SIGNAL void messageReceived(const QString& message);

    // network params
    QString address_;
    quint16 port_ = 0U;

    // network socket
    QUdpSocket socket;

    QString name_;
};

#endif  // CLIENT_H
