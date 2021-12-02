#pragma once
#include <QtWidgets>
#include <QTextEdit>
#include <QTcpSocket>
#include <QTcpServer>

//class QTcpServer;
//class QТextEdit;
//class QТcpSocket;

class MyServer : public QWidget {
    Q_OBJECT
private:
    QTcpServer* m_ptcpServer;

    QTextEdit* m_ptxt;
    quint16 m_nNextBlockSize;
private:
    void sendToClient(QTcpSocket* pSocket, const QString& str);
public:
    MyServer(int nPort, QWidget* pwgt = 0);
public slots:
    virtual void slotNewConnection();
            void slotReadClient();

};
