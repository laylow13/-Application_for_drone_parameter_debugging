#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork>
#include <QtCharts>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_connectBtn_clicked(bool checked);

    void on_refreshBtn_clicked();
    void processPendingDatagram();
//    void on_outKp_textEdited(const QString &arg1)/*;*/

    void on_outKp_editingFinished();

    void on_outKi_editingFinished();

    void on_outKd_editingFinished();

    void on_inKi_editingFinished();

    void on_inKd_editingFinished();

    void on_inKp_editingFinished();

    void on_outKp_2_editingFinished();

    void on_outKi_2_editingFinished();

    void on_outKd_2_editingFinished();

    void on_inKp_2_editingFinished();

    void on_inKd_2_editingFinished();

    void on_inKi_2_editingFinished();

    void on_outKi_3_editingFinished();

    void on_outKp_3_editingFinished();

    void on_outKd_3_editingFinished();

    void on_inKp_3_editingFinished();

    void on_inKi_3_editingFinished();

    void on_inKd_3_editingFinished();

    void on_dispPitchBtn_clicked(bool checked);

    void on_dispRollBtn_clicked(bool checked);

    void on_dispYawBtn_clicked(bool checked);

private:
    Ui::MainWindow *ui;
    QUdpSocket *sender;
    QUdpSocket *receiver;
    QChart *chart;
    QLineSeries *pitchDesSeries;
    QLineSeries *pitchCurSeries;
    QLineSeries *rollDesSeries;
    QLineSeries *rollCurSeries;
    QLineSeries *yawDesSeries;
    QLineSeries *yawCurSeries;
    QValueAxis *axisX;
    QValueAxis *axisY;

    void dataParser(QString data);
    void chartInit();

};
#endif // MAINWINDOW_H
