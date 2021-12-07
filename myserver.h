#pragma once
#include <QtWidgets>
#include <QTcpSocket>
#include <QTcpServer>
#include <QVector>
#include "/home/konstantin/QtProjects/MyClient/globalcommonheader.h"
#include <QtSql/QtSql>


class MyServer : public QWidget {
    Q_OBJECT
private:
    QTcpServer* m_ptcpServer;
    QList<QTcpSocket*> clients;
    QTextEdit* m_ptxt;
    quint16 m_nNextBlockSize;
    QString db_input;
    int user_counter;
    bool m_loginSuccesfull;
    QSqlDatabase mw_db;
private:
    void sendToClient(QTcpSocket* pSocket);
    void authorizeUser(QString,QString,QTcpSocket*);
    void registerUser(QString,QString,QTcpSocket*);
    void sendIsSuccessAuth(QTcpSocket *pSocket, bool isSuccess);
    void sendIsSuccessReg(QTcpSocket *pSocket, bool isSuccess);
    void sendToAllClients(QString);
public:
    bool connectDB();
    MyServer(int nPort, QWidget* pwgt = 0);
public slots:
    void deleteFromList();
    virtual void slotNewConnection();
            void slotReadClient();
signals:
    //void AuthInfReceived(QString,QString);
private slots:
};
