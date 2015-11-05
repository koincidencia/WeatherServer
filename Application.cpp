#include "Application.h"

Application::Application(int argc, char *argv[], int port, QFile &file)
    : QCoreApplication(argc,argv), logFile(file)
{
    connect(&server, &SocketServer::dataReady, this, &Application::serverDataReady);
    server.start(port);
}

void Application::serverDataReady(QDataStream& inStream)
{
    qint32 num=inStream.readRawData((char*)rawData,256);
    if (num < 0)
    {
        qWarning() << "Hiba a vett adatokban :(";
    }
    else
    {
        rawData[num]=0;
        qWarning() << "A kliens socketen üzenet érkezett: " << (char*)rawData;
        qWarning() << "Az érkezett bájtok száma: " << num;
    }
    /** Válasz küldése, a modul ezután megszíkítja a kapcsolatot és alvó üzemmódba lép.*/
    server.send("Minden OK.");

    /** Adat feldolgozása.*/
    parseRawData();

    /** Adatok elmentése*/
    saveToLogFile();
}

void Application::parseRawData()
{
    double ST=rawData[0]*256.0+rawData[1];
    this->temperature=175.0*ST/(65535.0)-45.0;

    double SRH=rawData[2]*256.0+rawData[3];
    this->humidity=100.0*SRH/65535.0;

    this->vbat=rawData[5]*256.0+rawData[4];

    timeObj = QDateTime::currentDateTime();
    qDebug() << "-----------------------------------------------";
    qDebug() << timeObj.toString(QString("yyyy.MM.dd hh:mm:ss"));
    qDebug() << "Temperature: \t" << this->temperature << " °C";
    qDebug() << "Humidity: \t" << this->humidity << " %";
    qDebug() << "Bat. voltage: \t" << this->vbat << " mV";
}

void Application::saveToLogFile()
{
    if(logFile.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append))
    {
        QTextStream stream(&logFile);
        stream << timeObj.toString(QString("yyyy.MM.dd hh:mm:ss")) << "\t";
        stream << this->temperature << "\t";
        stream << this->humidity << "\t";
        stream << this->vbat << endl;
        logFile.close();
    }
    else
        qDebug() << "Nem lehet a fajlt megnyitni.";
}
