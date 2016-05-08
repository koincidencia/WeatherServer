#include "Application.h"

Application::Application(int argc, char *argv[], int port, QFile &file)
    : QApplication(argc,argv), logFile(file)
{
    connect(&server, &SocketServer::dataReady, this, &Application::serverDataReady);

    server.start(port);

    QObject::connect(this, &Application::dataReady, &w, &MainWindow::plotData);
    QObject::connect(this, &Application::dataFromLogReady, &w, &MainWindow::plotLog);
    connect(&w, &MainWindow::loadLogFile, this, &Application::LoadLogFile);

    w.show();
}

void Application::serverDataReady(QDataStream& inStream)
{
    qint32 num=inStream.readRawData((char*)rawData,256);
    if (num < 0)
    {
        qDebug() << "Hiba a vett adatokban :(";
    }
    else
    {
        rawData[num]=0;
        qDebug() << "A kliens socketen üzenet érkezett: " << (char*)rawData;
        qDebug() << "Az érkezett bájtok száma: " << num;
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

    data.clear();
    data.enqueue(temperature);
    data.enqueue(humidity);
    data.enqueue(vbat);

    emit dataReady(data,timeObj);
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


void Application::LoadLogFile(QString path)
{
    QFile fileToRead(path);
    if(fileToRead.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QVector<double> timeVec, tempVec, humVec, vbatVec;
        QVector<QVector<double>> dataVec;

        QTextStream stream(&fileToRead);
        QString line;
        QStringList strList;
        QDateTime time;
        line = stream.readLine();
        while(!line.isNull())
        {
            strList = line.split('\t');
            time = QDateTime::fromString(strList.first(),QString("yyyy.MM.dd hh:mm:ss"));
            temperature = strList[1].toDouble();
            humidity = strList[2].toDouble();
            vbat = strList[3].toDouble();

            timeVec.append(time.toTime_t());
            tempVec.append(temperature);
            humVec.append(humidity);
            vbatVec.append(vbat);
            dataVec.clear();
            dataVec.append(tempVec);
            dataVec.append(humVec);
            dataVec.append(vbatVec);

            line = stream.readLine();
        }
        qDebug() << "dataFromLogReady emittálás...";
        emit dataFromLogReady(timeVec, dataVec);
        qDebug() << "dataFromLogReady emittálva.";
        fileToRead.close();
    }
    else
        qDebug() << "Nem lehet a fajlt megnyitni.";
}
