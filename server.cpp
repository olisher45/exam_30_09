#include "server.h"
#include <fstream>
#include <QString>
#include <QDebug>
#include <ctime>
#include <QMap>
#include <QPair>
#include <random>

Server::Server()
{

}

Server *Server::getInstance()
{
    if (!p_instance)
        p_instance = new Server();
    return p_instance;
}

QMap<QString,QPair<QString, bool>> login_password;

// login : (пароль, авторизован ли пользователь)

int key = 0;

QString auth(QString login, QString password, int newkey) {
    std::fstream file("log.txt", std::ios::out | std::ios::app);
    if (key == 0) {
        srand(time(0));
        key = rand()+1;
        return QString("code:")+QString::number(key);
    } else {
        if (login_password.contains(login)) {
            if (login_password[login].first == password and newkey == key) {
                login_password[login].second = true;
                key = 0;
                if (file.is_open()) {
                    file << "auth: " << login.toStdString() << "\n";
                    file.close();
                }

                return "auth+";
            } else {
                key = 0;
                return "auth-";
            }
        }
    }
}

QString reg(QString login, QString password) {
    if (login_password.contains(login)) {
        return "reg-";
    }
    login_password[login] = qMakePair(password, false);
    return "reg+";
}

QString querry(QString login, QString password, QString text) {
    std::fstream file("log.txt", std::ios::out | std::ios::app);
    if (login_password.contains(login)) {
        if (login_password[login].first == password and login_password[login].second == true) {
            if (file.is_open()) {
                file << login.toStdString() << ": " << text.toStdString() << "\n";
                file.close();
            }
            return "querry: 1";
        }
    }
    return "querry: 0";
}

QByteArray Server::parse(QString userMessage) {
    userMessage.chop(2);
    QStringList data = userMessage.split(';');
    if (key == 0) {
        if (data[0] == "auth" and data.size() == 3) {
            return QByteArray(auth(data[1],data[2], 0).toUtf8());
        } else if (data[0] == "reg") {
            return QByteArray(reg(data[1],data[2]).toUtf8());
        } else if (data[0] == "querry") {
            return QByteArray(querry(data[1],data[2],data[3]).toUtf8());
        }
    } else {
        if (data[0] == "auth" and data.size() == 3) {
            key = 0;
            return QByteArray(QString("auth-").toUtf8());
        } else if (data[0] == "auth" and data.size() == 4) {
            return QByteArray(auth(data[1],data[2], data[3].toInt()).toUtf8());
        }
    }

}
