#ifndef SVCDASHBOARDWIDGET_H
#define SVCDASHBOARDWIDGET_H

#include <QWidget>
#include <QTimer>

class QGraphicsScene;
class CGraphicsItemPart;

namespace SVCDiagnostics {

namespace Ui {
    class CSVCDashboardWidget;
}

class CSVCDashboardWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit CSVCDashboardWidget(QWidget *parent = 0);
    ~CSVCDashboardWidget();
    
private:
    void paintEvent(QPaintEvent *p_PaintEvent);
    Ui::CSVCDashboardWidget *ui;
    QGraphicsScene *m_pScene;
    CGraphicsItemPart * m_pRetort;

private slots:

};

}//end of namespace

#endif // SVCDASHBOARDWIDGET_H
