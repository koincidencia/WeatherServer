#pragma once
#ifndef APPLICATION_H
#define APPLICATION_H
#include <QApplication>
#include <QTimer>
#include <QDataStream>
#include <QFile>
#include <QDateTime>
#include <QString>
#include <QTextStream>
#include <QQueue>
#include "SocketClient.h"
#include "SocketServer.h"

/**
 * @brief Az alkalmazás osztály.s
 */
class Application : public QApplication
{
    // A socket rendszer nem működik, ha nem megfelelő QObject az osztályunk.
    // Ez a makró kiegészíti az osztályt a Qt QObject-jéhez szükséges funkcionalitásokkal.
    Q_OBJECT

public:
    /** Konstruktor. */
    Application(int argc, char *argv[], int port, QFile &file);
    ~Application() = default;
signals:
    void dataReady(QQueue<double>&, QDateTime&);

private:
    /** Szerver oldali kommunikáció. */
    SocketServer server;


    quint8 rawData[256];
    double temperature;
    double humidity;
    double vbat;
    QQueue<double> data;

    QFile &logFile;
    QDateTime timeObj;

    QString logFilePath;
    void parseRawData();
    void saveToLogFile();

private slots:

    /** A server dataReady signaljához kötött slot. "Mizu?" kérdés esetén
     * ez küldi vissza a választ. */
    void serverDataReady(QDataStream& inStream);

};

#endif // APPLICATION_H
