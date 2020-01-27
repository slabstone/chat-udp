#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <cstdint>  // uint8_t

#include <QtCore/QDataStream>
#include <QtCore/QString>

enum class MessageType : std::uint8_t
{
    Hello,
    Bye,
    Service,
    Text
};

struct Message
{
    MessageType type;
    QString sender;
    QString message;
};

inline QDataStream &operator<<(QDataStream& stream, const Message& message)
{
    stream << static_cast<std::uint8_t>(message.type)
        << message.sender << message.message;
    return stream;
}

inline QDataStream &operator>>(QDataStream& stream, Message& message)
{
    quint8 type;
    stream >> type;
    message.type = static_cast<MessageType>(type);
    stream >> message.sender >> message.message;
    return stream;
}

#endif  // PROTOCOL_H
