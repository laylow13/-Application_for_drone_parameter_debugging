// 对QChartView进行方法重写，代码参考自:枫影竹韵 https://blog.csdn.net/weixin_41882459/article/details/107713316

#ifndef MYCHART_H
#define MYCHART_H

#include <math.h>
//#include
#include <QWidget>
#include <QtCharts/QChartView>
using namespace QtCharts;

class myChart : public QChartView
{
    Q_OBJECT

public:
    explicit myChart(QWidget *parent = nullptr);
    ~myChart();

protected:
 virtual void mouseMoveEvent(QMouseEvent *pEvent) override;
 virtual void mousePressEvent(QMouseEvent *pEvent) override;
 virtual void mouseReleaseEvent(QMouseEvent *pEvent) override;
 virtual void wheelEvent(QWheelEvent *pEvent) override;
private:
 bool m_bMiddleButtonPressed;
 QPoint m_oPrePos;

};

#endif // MYCHART_H
