#include "client.h"

#include <QtCore/QDataStream>
#include <QtCore/QIODevice>
#include <QtCore/QtDebug>  // qDebug
#include <QtCore/QVariant>
#include <QtNetwork/QAbstractSocket>
#include <QtNetwork/QHostAddress>

Client::Client()
{

}

void Client::setName(const QString& name)
{
    name_ = name;
}

QString Client::address() const
{
    return address_;
}

void Client::setAddress(const QString& address)
{
    if (checkAddress(address))
    {
        address_ = address;
    }
}

quint16 Client::port() const
{
    return port_;
}

void Client::setPort(quint16 port)
{
    if (port >= 1024 && port <= 49151)
    {
        port_ = port;
    }
}

bool Client::connect(const QString& address, quint16 port)
{
    setAddress(address);
    setPort(port);
    return connect();
}

bool Client::connect()
{
    socket.bind(QHostAddress::Any, 0, QUdpSocket::DontShareAddress);
    sendHello();

    return QObject::connect(&socket,
        SIGNAL(readyRead()),
        SLOT(onReadyRead()));
}

bool Client::disconnect()
{
    const bool result = socket.disconnect(SIGNAL(readyRead()));
    socket.flush();

    return result;
}

void Client::sendHello()
{
    qDebug() << "sending hello to" << address_ << port_;

    const Message message{ MessageType::Hello , name_, "" };
    QByteArray datagram;
    QDataStream stream(&datagram, QIODevice::WriteOnly);
    stream << message;
    socket.writeDatagram(datagram, QHostAddress(address_), port_);
}

void Client::sendMessage(const QString& text)
{
    qDebug() << "sending" << text << "to" << address_ << port_;

    const Message message{ MessageType::Text , name_, text };
    QByteArray datagram;
    QDataStream stream(&datagram, QIODevice::WriteOnly);
    stream << message;
    socket.writeDatagram(datagram, QHostAddress(address_), port_);
}

void Client::processPendingDatagram()
{
    // check datagram size
    const qint64 pendingDatagramSize = socket.pendingDatagramSize();
    if (pendingDatagramSize == -1)
    {
        qDebug() << "pendingDatagramSize error";
        return;
    }
    else if (pendingDatagramSize == 0)
    {
        qDebug() << "pending datagram is empty";
        socket.read(nullptr, 0);
        return;
    }

    // read datagram and check result
    QByteArray datagram(pendingDatagramSize, '\0');
    QHostAddress fromAddress;
    quint16 fromPort = 0U;
    const qint64 readResult = socket.readDatagram(
        datagram.data(), pendingDatagramSize);
    if (readResult == -1)
    {
        qDebug() << "read error";
        return;
    }
    else if (readResult != pendingDatagramSize)
    {
        qDebug() << "size mismatch";
        return;
    }

    QDataStream stream(&datagram, QIODevice::ReadOnly);
    Message message;
    stream >> message;

    parseMessage(message);
}

void Client::parseMessage(const Message& datagram)
{
    // client only processes data messages
    const MessageType messageType = datagram.type;
    switch (messageType)
    {
    case MessageType::Text:
        qDebug() << "received" << datagram.message << "from" << datagram.sender;
        Q_EMIT messageReceived(QString("<%1> %2").arg(datagram.sender, datagram.message));
        break;

    default:
        break;
    }
}

bool Client::checkAddress(const QString& address)
{
    return QHostAddress(address).protocol() == QAbstractSocket::IPv4Protocol;
}

Q_SLOT void Client::onReadyRead()
{
    qDebug() << "ready read";
    while (socket.hasPendingDatagrams())
    {
        processPendingDatagram();
    }
}
