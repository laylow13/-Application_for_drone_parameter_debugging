#include "mainwindow.h"
#include <QtCharts>
QT_CHARTS_USE_NAMESPACE
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    this->setWindowTitle("四旋翼姿态控制上位机");
    ui->setupUi(this);
    sender = new QUdpSocket(this);
    receiver = new QUdpSocket(this);
    receiver->bind(45454,QUdpSocket::ShareAddress);
    connect(receiver,SIGNAL(readyRead()),
    this,SLOT(processPendingDatagram()));
    chartInit();
}



MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::chartInit()
{
    chart=ui->chart->chart();
    axisX=new QValueAxis();
    axisY=new QValueAxis();
    axisX->setRange(0,3000);
    axisX->setTickCount(20);
    axisX->setTitleText("角度");
    axisY->setRange(-90,90);
    axisY->setTickCount(10);
    axisY->setTitleText("时间");

    chart->addAxis(axisX,Qt::AlignBottom);
    chart->addAxis(axisY,Qt::AlignLeft);

    QPen redPen(Qt::red);
    redPen.setWidth(2);

    pitchDesSeries= new QLineSeries(this);
    pitchDesSeries->attachAxis(axisX);
    pitchDesSeries->attachAxis(axisY);
    pitchDesSeries->setName("pitchDes");
//    pitchDesSeries->setPen(redPen);

    pitchCurSeries= new QLineSeries(this);
    pitchCurSeries->attachAxis(axisX);
    pitchCurSeries->attachAxis(axisY);
    pitchCurSeries->setName("pitchCur");
//    pitchCurSeries->setPen(redPen);

    rollCurSeries= new QLineSeries(this);
    rollCurSeries->attachAxis(axisX);
    rollCurSeries->attachAxis(axisY);
    rollCurSeries->setName("rollCur");
//    rollCurSeries->setPen(redPen);

    rollDesSeries= new QLineSeries(this);
    rollDesSeries->attachAxis(axisX);
    rollDesSeries->attachAxis(axisY);
    rollDesSeries->setName("rollDes");
//    rollDesSeries->setPen(redPen);

    yawCurSeries= new QLineSeries(this);
    yawCurSeries->attachAxis(axisX);
    yawCurSeries->attachAxis(axisY);
    yawCurSeries->setName("yawCur");
//    yawCurSeries->setPen(redPen);

    yawDesSeries= new QLineSeries(this);
    yawDesSeries->attachAxis(axisX);
    yawDesSeries->attachAxis(axisY);
    yawDesSeries->setName("yawDes");
//    yawDesSeries->setPen(redPen);

//    chart.
    chart->legend();
}
void MainWindow::on_connectBtn_clicked(bool checked)
{
    QString ip=ui->IPLineEdit->text();
    QString port=ui->portLineEdit->text();
    QByteArray datagram;
    if(checked)
    {
         datagram= "connect";
    }
    else {
         datagram = "disconnect";
    }
    sender->writeDatagram(datagram.data(),datagram.size(),
                           QHostAddress(ip),port.toUInt());
}


void MainWindow::on_refreshBtn_clicked()
{
    QString ip=ui->IPLineEdit->text();
    QString port=ui->portLineEdit->text();
    QByteArray datagram;
    datagram= "refresh";
    sender->writeDatagram(datagram.data(),datagram.size(),
                           QHostAddress(ip),port.toUInt());
}
void MainWindow::processPendingDatagram()
{
    while(receiver->hasPendingDatagrams())  //拥有等待的数据报
        {
           QByteArray datagram; //拥于存放接收的数据报
           QString data;
           //让datagram的大小为等待处理的数据报的大小，这样才能接收到完整的数据
           datagram.resize(receiver->pendingDatagramSize());
           //接收数据报，将其存放到datagram中
           receiver->readDatagram(datagram.data(),datagram.size());
           //将数据报内容显示出来
           data=QString(datagram);
           dataParser(data);
        }
}
void MainWindow::dataParser(QString data)
{
//    data;
}

void MainWindow::on_outKp_editingFinished()
{
    QString ip=ui->IPLineEdit->text();
    QString port=ui->portLineEdit->text();
    QString data("okp1:");
    data.append(ui->outKp->text());
    QByteArray datagram;
    datagram=data.toLatin1();
    sender->writeDatagram(datagram.data(),datagram.size(),
                           QHostAddress(ip),port.toUInt());
}


void MainWindow::on_outKi_editingFinished()
{
    QString ip=ui->IPLineEdit->text();
    QString port=ui->portLineEdit->text();
    QString data("oki1:");
    data.append(ui->outKi->text());
    QByteArray datagram;
    datagram=data.toLatin1();
    sender->writeDatagram(datagram.data(),datagram.size(),
                           QHostAddress(ip),port.toUInt());
}


void MainWindow::on_outKd_editingFinished()
{
    QString ip=ui->IPLineEdit->text();
    QString port=ui->portLineEdit->text();
    QString data("okd1:");
    data.append(ui->outKd->text());
    QByteArray datagram;
    datagram=data.toLatin1();
    sender->writeDatagram(datagram.data(),datagram.size(),
                           QHostAddress(ip),port.toUInt());
}

void MainWindow::on_inKp_editingFinished()
{
    QString ip=ui->IPLineEdit->text();
    QString port=ui->portLineEdit->text();
    QString data("ikp1:");
    data.append(ui->inKp->text());
    QByteArray datagram;
    datagram=data.toLatin1();
    sender->writeDatagram(datagram.data(),datagram.size(),
                           QHostAddress(ip),port.toUInt());
}


void MainWindow::on_inKi_editingFinished()
{
    QString ip=ui->IPLineEdit->text();
    QString port=ui->portLineEdit->text();
    QString data("iki1:");
    data.append(ui->inKi->text());
    QByteArray datagram;
    datagram=data.toLatin1();
    sender->writeDatagram(datagram.data(),datagram.size(),
                           QHostAddress(ip),port.toUInt());
}


void MainWindow::on_inKd_editingFinished()
{
    QString ip=ui->IPLineEdit->text();
    QString port=ui->portLineEdit->text();
    QString data("ikd1:");
    data.append(ui->inKd->text());
    QByteArray datagram;
    datagram=data.toLatin1();
    sender->writeDatagram(datagram.data(),datagram.size(),
                           QHostAddress(ip),port.toUInt());
}

void MainWindow::on_outKp_2_editingFinished()
{
    QString ip=ui->IPLineEdit->text();
    QString port=ui->portLineEdit->text();
    QString data("okp2:");
    data.append(ui->outKp_2->text());
    QByteArray datagram;
    datagram=data.toLatin1();
    sender->writeDatagram(datagram.data(),datagram.size(),
                           QHostAddress(ip),port.toUInt());
}

void MainWindow::on_outKi_2_editingFinished()
{
    QString ip=ui->IPLineEdit->text();
    QString port=ui->portLineEdit->text();
    QString data("oki2:");
    data.append(ui->outKi_2->text());
    QByteArray datagram;
    datagram=data.toLatin1();
    sender->writeDatagram(datagram.data(),datagram.size(),
                           QHostAddress(ip),port.toUInt());
}


void MainWindow::on_outKd_2_editingFinished()
{
    QString ip=ui->IPLineEdit->text();
    QString port=ui->portLineEdit->text();
    QString data("okd2:");
    data.append(ui->outKd_2->text());
    QByteArray datagram;
    datagram=data.toLatin1();
    sender->writeDatagram(datagram.data(),datagram.size(),
                           QHostAddress(ip),port.toUInt());
}


void MainWindow::on_inKp_2_editingFinished()
{
    QString ip=ui->IPLineEdit->text();
    QString port=ui->portLineEdit->text();
    QString data("ikp2:");
    data.append(ui->inKp_2->text());
    QByteArray datagram;
    datagram=data.toLatin1();
    sender->writeDatagram(datagram.data(),datagram.size(),
                           QHostAddress(ip),port.toUInt());
}

void MainWindow::on_inKi_2_editingFinished()
{
    QString ip=ui->IPLineEdit->text();
    QString port=ui->portLineEdit->text();
    QString data("iki2:");
    data.append(ui->inKi_2->text());
    QByteArray datagram;
    datagram=data.toLatin1();
    sender->writeDatagram(datagram.data(),datagram.size(),
                           QHostAddress(ip),port.toUInt());
}

void MainWindow::on_inKd_2_editingFinished()
{
    QString ip=ui->IPLineEdit->text();
    QString port=ui->portLineEdit->text();
    QString data("ikd2:");
    data.append(ui->inKd_2->text());
    QByteArray datagram;
    datagram=data.toLatin1();
    sender->writeDatagram(datagram.data(),datagram.size(),
                           QHostAddress(ip),port.toUInt());
}

void MainWindow::on_outKp_3_editingFinished()
{
    QString ip=ui->IPLineEdit->text();
    QString port=ui->portLineEdit->text();
    QString data("okp3:");
    data.append(ui->outKp_3->text());
    QByteArray datagram;
    datagram=data.toLatin1();
    sender->writeDatagram(datagram.data(),datagram.size(),
                           QHostAddress(ip),port.toUInt());
}


void MainWindow::on_outKi_3_editingFinished()
{
    QString ip=ui->IPLineEdit->text();
    QString port=ui->portLineEdit->text();
    QString data("oki3:");
    data.append(ui->outKi_3->text());
    QByteArray datagram;
    datagram=data.toLatin1();
    sender->writeDatagram(datagram.data(),datagram.size(),
                           QHostAddress(ip),port.toUInt());
}


void MainWindow::on_outKd_3_editingFinished()
{
    QString ip=ui->IPLineEdit->text();
    QString port=ui->portLineEdit->text();
    QString data("okd3:");
    data.append(ui->outKd_3->text());
    QByteArray datagram;
    datagram=data.toLatin1();
    sender->writeDatagram(datagram.data(),datagram.size(),
                           QHostAddress(ip),port.toUInt());
}


void MainWindow::on_inKp_3_editingFinished()
{
    QString ip=ui->IPLineEdit->text();
    QString port=ui->portLineEdit->text();
    QString data("ikp3:");
    data.append(ui->inKp_3->text());
    QByteArray datagram;
    datagram=data.toLatin1();
    sender->writeDatagram(datagram.data(),datagram.size(),
                           QHostAddress(ip),port.toUInt());
}


void MainWindow::on_inKi_3_editingFinished()
{
    QString ip=ui->IPLineEdit->text();
    QString port=ui->portLineEdit->text();
    QString data("iki3:");
    data.append(ui->inKi_3->text());
    QByteArray datagram;
    datagram=data.toLatin1();
    sender->writeDatagram(datagram.data(),datagram.size(),
                           QHostAddress(ip),port.toUInt());
}


void MainWindow::on_inKd_3_editingFinished()
{
    QString ip=ui->IPLineEdit->text();
    QString port=ui->portLineEdit->text();
    QString data("ikd3:");
    data.append(ui->inKd_3->text());
    QByteArray datagram;
    datagram=data.toLatin1();
    sender->writeDatagram(datagram.data(),datagram.size(),
                           QHostAddress(ip),port.toUInt());
}


void MainWindow::on_dispPitchBtn_clicked(bool checked)
{
    if(checked)
    {
        chart->addSeries(pitchCurSeries);
        chart->addSeries(pitchDesSeries);
    }
    else {
        chart->removeSeries(pitchCurSeries);
        chart->removeSeries(pitchDesSeries);
    }

}


void MainWindow::on_dispRollBtn_clicked(bool checked)
{
    if(checked)
    {
        chart->addSeries(rollCurSeries);
        chart->addSeries(rollDesSeries);
    }
    else {
        chart->removeSeries(rollCurSeries);
        chart->removeSeries(rollDesSeries);
    }
}


void MainWindow::on_dispYawBtn_clicked(bool checked)
{
    if(checked)
    {
        chart->addSeries(yawCurSeries);
        chart->addSeries(yawDesSeries);
    }
    else {
        chart->removeSeries(yawCurSeries);
        chart->removeSeries(yawDesSeries);
    }
}

