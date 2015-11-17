/***************************************************************************
**                                                                        **
**  QCustomPlot, an easy to use, modern plotting widget for Qt            **
**  Copyright (C) 2011-2015 Emanuel Eichhammer                            **
**                                                                        **
**  This program is free software: you can redistribute it and/or modify  **
**  it under the terms of the GNU General Public License as published by  **
**  the Free Software Foundation, either version 3 of the License, or     **
**  (at your option) any later version.                                   **
**                                                                        **
**  This program is distributed in the hope that it will be useful,       **
**  but WITHOUT ANY WARRANTY; without even the implied warranty of        **
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         **
**  GNU General Public License for more details.                          **
**                                                                        **
**  You should have received a copy of the GNU General Public License     **
**  along with this program.  If not, see http://www.gnu.org/licenses/.   **
**                                                                        **
****************************************************************************
**           Author: Emanuel Eichhammer                                   **
**  Website/Contact: http://www.qcustomplot.com/                          **
**             Date: 25.04.15                                             **
**          Version: 1.3.1                                                **
****************************************************************************/

/************************************************************************************************************
**                                                                                                         **
**  This is the example code for QCustomPlot.                                                              **
**                                                                                                         **
**  It demonstrates basic and some advanced capabilities of the widget. The interesting code is inside     **
**  the "setup(...)Demo" functions of MainWindow.                                                          **
**                                                                                                         **
**  In order to see a demo in action, call the respective "setup(...)Demo" function inside the             **
**  MainWindow constructor. Alternatively you may call setupDemo(i) where i is the index of the demo       **
**  you want (for those, see MainWindow constructor comments). All other functions here are merely a       **
**  way to easily create screenshots of all demos for the website. I.e. a timer is set to successively     **
**  setup all the demos and make a screenshot of the window area and save it in the ./screenshots          **
**  directory.                                                                                             **
**                                                                                                         **
*************************************************************************************************************/

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QDesktopWidget>
#include <QScreen>
#include <QMessageBox>
#include <QMetaEnum>

#include <QQueue>
#include <QDateTime>

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent), ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  setGeometry(400, 250, 542, 390);

  ui->customPlot->addGraph();
  ui->customPlot2->addGraph();
  ui->customPlot3->addGraph();
  setWindowTitle("Title");

  startTime=startTime.currentDateTimeUtc();

  ui->customPlot->xAxis->setLabel("t");
  ui->customPlot->yAxis->setLabel("T[°C]");
  ui->customPlot->yAxis->setRange(-10, 30);
  ui->customPlot->xAxis->setTickLabelType(QCPAxis::ltDateTime);
  ui->customPlot->xAxis->setDateTimeFormat("yyyy.MM.dd.\nhh:mm");

  ui->customPlot2->xAxis->setLabel("t");
  ui->customPlot2->yAxis->setLabel("RH[%]");
  ui->customPlot2->yAxis->setRange(-1, 101);
  ui->customPlot2->xAxis->setTickLabelType(QCPAxis::ltDateTime);
  ui->customPlot2->xAxis->setDateTimeFormat("yyyy.MM.dd.\nhh:mm");


  ui->customPlot3->xAxis->setLabel("t");
  ui->customPlot3->yAxis->setLabel("Vbat[mV]");
  ui->customPlot3->yAxis->setRange(2700, 4300);
  ui->customPlot3->xAxis->setTickLabelType(QCPAxis::ltDateTime);
  ui->customPlot3->xAxis->setDateTimeFormat("yyyy.MM.dd.\nhh:mm");

}
MainWindow::~MainWindow(){}
void MainWindow::plotData(QQueue<double>& data, QDateTime& time)
{
    timeVec.append(time.toTime_t());
    tempVec.append(data.dequeue());
    humVec.append(data.dequeue());
    vbatVec.append(data.dequeue());

    Replot(ui->customPlot, tempVec, timeVec);
    Replot(ui->customPlot2, humVec, timeVec);
    Replot(ui->customPlot3, vbatVec, timeVec);

    statusBar()->clearMessage();
}

void MainWindow::Replot(QCustomPlot* cp, QVector<double>& data, QVector<double>& time)
{
   cp->graph()->setData(time, data);
   cp->xAxis->setRange(startTime.toTime_t(), time.last());
   cp->replot();
}



















