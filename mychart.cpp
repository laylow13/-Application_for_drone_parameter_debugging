#include "mychart.h"


myChart::myChart(QWidget *parent) :
     QChartView(parent)
     , m_bMiddleButtonPressed(false)
     , m_oPrePos(0, 0)
    {
    }
myChart::~myChart()
    {
    }
void myChart::mouseMoveEvent(QMouseEvent *pEvent)
    {
         if (m_bMiddleButtonPressed)
         {
             QPoint oDeltaPos = pEvent->pos() - m_oPrePos;
             this->chart()->scroll(-oDeltaPos.x(), oDeltaPos.y());
             m_oPrePos = pEvent->pos();
         }
         QChartView::mouseMoveEvent(pEvent);
    }
void myChart::mousePressEvent(QMouseEvent *pEvent)
    {
         if (pEvent->button() == Qt::MiddleButton)
         {
             m_bMiddleButtonPressed = true;
             m_oPrePos = pEvent->pos();
             this->setCursor(Qt::OpenHandCursor);
         }
         QChartView::mousePressEvent(pEvent);
    }
void myChart::mouseReleaseEvent(QMouseEvent *pEvent)
    {
         if (pEvent->button() == Qt::MiddleButton)
         {
             m_bMiddleButtonPressed = false;
             this->setCursor(Qt::ArrowCursor);
         }
         QChartView::mouseReleaseEvent(pEvent);
    }
void myChart::wheelEvent(QWheelEvent *pEvent)
    {
         qreal rVal = std::pow(0.999, pEvent->delta());
         QRectF oPlotAreaRect = this->chart()->plotArea();
         QPointF oCenterPoint = oPlotAreaRect.center();
         oPlotAreaRect.setWidth(oPlotAreaRect.width() * rVal);
         oPlotAreaRect.setHeight(oPlotAreaRect.height() * rVal);
         QPointF oNewCenterPoint(2 * oCenterPoint - pEvent->pos() - (oCenterPoint - pEvent->pos()) / rVal);
         oPlotAreaRect.moveCenter(oNewCenterPoint);
         this->chart()->zoomIn(oPlotAreaRect);
         QChartView::wheelEvent(pEvent);
    }
