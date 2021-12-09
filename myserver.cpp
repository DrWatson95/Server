#include "myserver.h"
#include "ui_myserver.h"

void MyServer::deleteFromList(){
    for (size_t i = 0; i < clients.size(); ++i) {
        if(((QTcpSocket*)sender()) == clients[i])
            clients.erase(clients.begin() + i);
    }
}

void MyServer::sendIsSuccessAuth(QTcpSocket *pSocket, bool isSuccess)
{
    QByteArray arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    //out.setVersion(QDataStream::Qt_6_2);
    out << quint8(MessageID::Authorization) << quint8(isSuccess);
    pSocket->write(arrBlock);
}

void MyServer::sendIsSuccessReg(QTcpSocket *pSocket, bool isSuccess)
{
    QByteArray arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    //out.setVersion(QDataStream::Qt_6_2);
    out << quint8(MessageID::Registration) << quint8(isSuccess);
    pSocket->write(arrBlock);
}

bool MyServer::connectDB()
{
    mw_db = QSqlDatabase::addDatabase("QSQLITE");
    mw_db.setDatabaseName("authorisation");
    if(!mw_db.open())
    {
        qDebug() << "Cannot open database: " << mw_db.lastError();
        return false;
    }
    /*mw_db.setUserName("elton");
    mw_db.setHostName("epica");
    mw_db.setPassword("password");*/
    return true;
}


void MyServer::sendToClient(QTcpSocket *pSocket)
{
    QByteArray arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    bool t = true;
    out << quint8(MessageID::CheckConnected) << quint8(t);
    pSocket->write(arrBlock);
}

void MyServer::sendToAllClients(QString text,QTcpSocket* pClientSocket)
{
    QByteArray arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    //QString IpAddress = pClientSocket->peerAddress().toString();
    //IpAddress.erase(IpAddress.begin(),IpAddress.begin()+7);

    out << quint8(MessageID::UsefulExchange) << text;
    for (size_t i = 0;i < clients.size(); ++i) {
        clients[i]->write(arrBlock);
    }
}

MyServer::MyServer(int nPort, QWidget* pwgt) : QWidget (pwgt) , m_nNextBlockSize(0)
{
    user_counter = 0;
    m_loginSuccesfull = false;
    m_ptcpServer = new QTcpServer(this);
    if(!m_ptcpServer->listen(QHostAddress::Any, nPort)){
        QMessageBox::critical(0,"Server Error","Unable to start the server:" + m_ptcpServer->errorString());
        m_ptcpServer->close();
        return;
    }
    connect(m_ptcpServer,SIGNAL(newConnection()),this,SLOT(slotNewConnection()));
    m_ptxt = new QTextEdit;
    m_ptxt->setReadOnly(true);

    if(!connectDB())
    {
        qDebug() << "Failed to connect DB";
    }
    QSqlQuery query;

    db_input = "CREATE TABLE IF NOT EXISTS userlist ( "
               "number INTEGER,"
               "name VARCHAR(20) PRIMARY KEY NOT NULL, "
               "pass VARCHAR(12));";
    if(!query.exec(db_input))
    {
            qDebug() << "Unable to create a table" << query.lastError()/* << " : " << query.lastQuery()*/;
    }

    //Layout setup
    QVBoxLayout* pvbxLayout = new QVBoxLayout;
    pvbxLayout->addWidget(new QLabel("<H1>Server</H1>"));
    pvbxLayout->addWidget(m_ptxt);
    setLayout(pvbxLayout);
}

/**
*   \brief Реакция на новое соединение
*
*   \details Вызывается по сигналу MyServer::newConnection.
*   Метод связывает клиента с сервером и отправляет положительный ответ об установке соединения.
*dfdfsdf
*   \return void
*
**/

void MyServer::slotNewConnection()
{
    QTcpSocket* pClientSocket = m_ptcpServer->nextPendingConnection();
    clients.push_back(pClientSocket);
    //qDebug() << pClientSocket;
    connect(pClientSocket, SIGNAL(disconnected()),this ,SLOT(deleteFromList()));
    connect(pClientSocket, SIGNAL(disconnected()), pClientSocket, SLOT(deleteLater()));
    connect(pClientSocket, SIGNAL(readyRead()),this ,SLOT(slotReadClient()));
    sendToClient(pClientSocket);
}


void MyServer::slotReadClient()
{
    QTcpSocket* pClientSocket = (QTcpSocket*)sender();
    QDataStream in(pClientSocket);
    quint8 id;
    in >> id;
    switch (id) {
    case static_cast<quint8>(MessageID::Authorization):{
        //in.device()->seek(1);
        QString m_username, m_userpass;
        in >> m_username >> m_userpass;
        authorizeUser(m_username,m_userpass,pClientSocket);
        qDebug() << "Authorization";
        break;
    }
    case static_cast<quint8>(MessageID::Registration):{
        //in.device()->seek(1);
        QString m_username, m_userpass;
        in >> m_username >> m_userpass;
        //qDebug("Hereeeeeeeeeeee");
        //qDebug() << m_username;
        //qDebug() << m_userpass;
        registerUser(m_username,m_userpass,pClientSocket);
        qDebug() << "Registration";
        break;
    }
    case static_cast<quint8>(MessageID::UsefulExchange):{
        QString allMessage;
        in >> allMessage;
        sendToAllClients(allMessage,pClientSocket);
        m_ptxt->append(allMessage);
        qDebug() << "UsefulExchange:" << allMessage;
        break;
    }
    default:
            break;
    }

}


void MyServer::authorizeUser(QString m_username, QString m_userpass, QTcpSocket* pClientSocket)
{
    QString str_t       = " SELECT * "
                          " FROM userlist "
                          " WHERE name = '%1'";
    QString username    = "";
    QString userpass    = "";
    //qDebug() << "authorizeUser";
    db_input    = str_t.arg(m_username);
    QSqlQuery query;
    QSqlRecord rec;
    if(!query.exec(db_input))
    {
        qDebug() << "Unable to execute query - exiting" << query.lastError() << " : " << query.lastQuery();
    }
    rec = query.record();
    query.next();
    username    = query.value(rec.indexOf("name")).toString();
    userpass    = query.value(rec.indexOf("pass")).toString();
    if(m_username != username || m_userpass != userpass)
    {
        qDebug() << "Password missmatch" << username << " " << userpass;
        m_loginSuccesfull = false;
        sendIsSuccessAuth(pClientSocket,m_loginSuccesfull);
    }
    else
    {
        m_loginSuccesfull = true;
        sendIsSuccessAuth(pClientSocket,m_loginSuccesfull);
    }
}

void MyServer::registerUser(QString m_username, QString m_userpass,QTcpSocket* pClientSocket)
{
    QSqlQuery* query = new QSqlQuery(mw_db);
    QString str_t= "SELECT COUNT(*) FROM userlist;";
    QString db_input = str_t;
    if(!query->exec(db_input)){
        qDebug() << "Unable to get number " << query->lastError() << " : " << query->lastQuery();
        return;
    }
    else{
        query->next();
        user_counter = query->value(0).toInt();
        qDebug() << user_counter;
    }
    user_counter++;
    str_t = "INSERT INTO userlist(number, name, pass) VALUES(%1, '%2', '%3');";
    db_input = str_t .arg(user_counter).arg(m_username).arg(m_userpass);
    if(!query->exec(db_input)){
        sendIsSuccessReg(pClientSocket,false);
        qDebug() << "FAILURE";
    }else{
        qDebug() << "SUCCESS";
        sendIsSuccessReg(pClientSocket,true);
    }
}




