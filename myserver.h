#pragma once
#include <QtWidgets>
#include <QTcpSocket>
#include <QTcpServer>
#include <QVector>

class MyServer : public QWidget {
    Q_OBJECT
private:
    QTcpServer* m_ptcpServer;
    QList<QTcpSocket*> clients;
    QTextEdit* m_ptxt;
    quint16 m_nNextBlockSize;
private:
    void sendToClient(QTcpSocket* pSocket, const QString& str);
public:
    MyServer(int nPort, QWidget* pwgt = 0);
public slots:
    void deleteFromList();
    virtual void slotNewConnection();
            void slotReadClient();


};
