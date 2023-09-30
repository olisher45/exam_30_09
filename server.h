#ifndef SERVER_H
#define SERVER_H

#include <QByteArray>

class Server
{
private:
    Server();
    Server(const Server& );
    Server& operator = (Server &);
    static inline Server *p_instance = nullptr;

public:
    static Server* getInstance();

    QByteArray parse(QString userMessage);
};

#endif // SERVER_H
