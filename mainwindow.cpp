#include "mainwindow.h"
#include <QtCharts>
QT_CHARTS_USE_NAMESPACE
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    ,isConnected(false)
    , m_bMiddleButtonPressed(false)
    , m_oPrePos(0, 0)

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
    axisX->setRange(0,3);
    axisX->setTickCount(20);
    axisX->setTitleText("时间");
    axisY->setRange(-90,90);
    axisY->setTickCount(10);
    axisY->setTitleText("角度");

    chart->addAxis(axisX,Qt::AlignBottom);
    chart->addAxis(axisY,Qt::AlignLeft);


    QPen redPen(Qt::red);
    redPen.setWidth(2);

    pitchDesSeries= new QLineSeries(chart);
    pitchDesSeries->setName("pitchDes");

    pitchCurSeries= new QLineSeries(chart);
    pitchCurSeries->setName("pitchCur");

    rollCurSeries= new QLineSeries(this);
    rollCurSeries->setName("rollCur");

    rollDesSeries= new QLineSeries(this);
    rollDesSeries->setName("rollDes");

    yawCurSeries= new QLineSeries(this);
    yawCurSeries->setName("yawCur");

    yawDesSeries= new QLineSeries(this);
    yawDesSeries->setName("yawDes");

    chart->addSeries(pitchDesSeries);
    chart->addSeries(pitchCurSeries);
    pitchDesSeries->attachAxis(axisX);
    pitchDesSeries->attachAxis(axisY);
    pitchCurSeries->attachAxis(axisX);
    pitchCurSeries->attachAxis(axisY);

    chart->addSeries(rollCurSeries);
    chart->addSeries(rollDesSeries);
    rollCurSeries->attachAxis(axisX);
    rollCurSeries->attachAxis(axisY);
    rollDesSeries->attachAxis(axisX);
    rollDesSeries->attachAxis(axisY);

    chart->addSeries(yawCurSeries);
    chart->addSeries(yawDesSeries);
    yawCurSeries->attachAxis(axisX);
    yawCurSeries->attachAxis(axisY);
    yawDesSeries->attachAxis(axisX);
    yawDesSeries->attachAxis(axisY);

    pitchCurSeries->setVisible(false);
    pitchDesSeries->setVisible(false);

    rollCurSeries->setVisible(false);
    rollDesSeries->setVisible(false);

    yawCurSeries->setVisible(false);
    yawDesSeries->setVisible(false);

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
         isConnected=true;
         pitchCurSeries->clear();
         pitchDesSeries->clear();
         yawCurSeries->clear();
         yawDesSeries->clear();
         rollCurSeries->clear();
         rollDesSeries->clear();
         axisX->setRange(0,3);
         count=0;
    }
    else {
         datagram = "disconnect";
         isConnected=false;

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
    while(receiver->hasPendingDatagrams())
        {
           QByteArray datagram;
           QString data;
           datagram.resize(receiver->pendingDatagramSize());
           receiver->readDatagram(datagram.data(),datagram.size());
           data=QString(datagram);
           dataParser(data);
        }
}
void MainWindow::dataParser(QString data)
{
    if(isConnected){
        QList<QString> list;
        QList<QStringList> datapool;
        QList<QString>::Iterator i;
        QList<QStringList>::Iterator j;
        QList<QString> compareTemplate={"okp1","oki1","okd1","ikp1","iki1","ikd1",
                                        "okp2","oki2","okd2","ikp2","iki2","ikd2",
                                       "okp3","oki3","okd3","ikp3","iki3","ikd3"};
        QList<QLineEdit*> lineEditList={ui->outKp,ui->outKi,ui->outKd,ui->inKp,ui->inKi,ui->inKd,
                                     ui->outKp_2,ui->outKi_2,ui->outKd_2,ui->inKp_2,ui->inKi_2,ui->inKd_2,
                                     ui->outKp_3,ui->outKi_3,ui->outKd_3,ui->inKp_3,ui->inKi_3,ui->inKd_3};
        QMap<QString,QLineSeries*> lineSeriesMap;
         lineSeriesMap.insert("pitchCur",pitchCurSeries);
         lineSeriesMap.insert("pitchDes",pitchDesSeries);
         lineSeriesMap.insert("rollCur",rollCurSeries);
         lineSeriesMap.insert("rollDes",rollDesSeries);
         lineSeriesMap.insert("yawCur",yawCurSeries);
         lineSeriesMap.insert("yawDes",yawDesSeries);
        list=data.split(";",QString::SkipEmptyParts);
        for(i = list.begin()+1; i != list.end(); ++i)
        {
            datapool.append(i->split(":"));
        }
        if(list.at(0)=="param")
        {
            for(j = datapool.begin(); j != datapool.end(); ++j)
            {
               lineEditList.at(compareTemplate.indexOf(j->at(0)))->setText(j->at(1));
            }
        }
        else {
//            quint32 timetag=list.at(0).split(":").at(1).toLong();
//            qreal xAxisPoint=(double)timetag/1000.0;
            for(j = datapool.begin(); j != datapool.end(); ++j)
            {
                if(lineSeriesMap.contains(j->at(0)))
                {
                    lineSeriesMap[j->at(0)]->append(count*0.05,j->at(1).toDouble());
                    count++;
                    if(count>50)
                    {
                        chart->scroll(chart->plotArea().width()/(axisX->max()-axisX->min())*0.05,0);
                    }
                }
            }
        }
    }
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
        pitchCurSeries->setVisible(true);
        pitchDesSeries->setVisible(true);
    }
    else {
        pitchCurSeries->setVisible(false);
        pitchDesSeries->setVisible(false);
    }
}


void MainWindow::on_dispRollBtn_clicked(bool checked)
{
    if(checked)
    {
        rollCurSeries->setVisible(true);
        rollDesSeries->setVisible(true);
    }
    else {
        rollCurSeries->setVisible(false);
        rollDesSeries->setVisible(false);
    }
}


void MainWindow::on_dispYawBtn_clicked(bool checked)
{
    if(checked)
    {

        yawCurSeries->setVisible(true);
        yawDesSeries->setVisible(true);
    }
    else {        
        yawCurSeries->setVisible(false);
        yawDesSeries->setVisible(false);
    }
}

//void MainWindow::mouseMoveEvent(QMouseEvent *pEvent)
//{
// if (m_bMiddleButtonPressed)
// {
// QPoint oDeltaPos = pEvent->pos() - m_oPrePos;
// this->chart->scroll(-oDeltaPos.x(), oDeltaPos.y());
// m_oPrePos = pEvent->pos();
// }
// __super::mouseMoveEvent(pEvent);
//}
//void MainWindow::mousePressEvent(QMouseEvent *pEvent)
//{
// if (pEvent->button() == Qt::MiddleButton)
// {
// m_bMiddleButtonPressed = true;
// m_oPrePos = pEvent->pos();
// this->setCursor(Qt::OpenHandCursor);
// }
// __super::mousePressEvent(pEvent);
//}
//void MainWindow::mouseReleaseEvent(QMouseEvent *pEvent)
//{
// if (pEvent->button() == Qt::MiddleButton)
// {
// m_bMiddleButtonPressed = false;
// this->setCursor(Qt::ArrowCursor);
// }
// __super::mouseReleaseEvent(pEvent);
//}
//void MainWindow::wheelEvent(QWheelEvent *pEvent)
//{
// qreal rVal = std::pow(0.999, pEvent->delta()); // 设置比例
// // 1. 读取视图基本信息
// QRectF oPlotAreaRect = this->chart->plotArea();
// QPointF oCenterPoint = oPlotAreaRect.center();
// // 2. 水平调整
// oPlotAreaRect.setWidth(oPlotAreaRect.width() * rVal);
// // 3. 竖直调整
// oPlotAreaRect.setHeight(oPlotAreaRect.height() * rVal);
// // 4.1 计算视点，视点不变，围绕中心缩放
// //QPointF oNewCenterPoint(oCenterPoint);
// // 4.2 计算视点，让鼠标点击的位置移动到窗口中心
// //QPointF oNewCenterPoint(pEvent->pos());
// // 4.3 计算视点，让鼠标点击的位置尽量保持不动(等比换算，存在一点误差)
// QPointF oNewCenterPoint(2 * oCenterPoint - pEvent->pos() - (oCenterPoint - pEvent->pos()) / rVal);
// // 5. 设置视点
// oPlotAreaRect.moveCenter(oNewCenterPoint);
// // 6. 提交缩放调整
// this->chart->zoomIn(oPlotAreaRect);
// __super::wheelEvent(pEvent);
//}


