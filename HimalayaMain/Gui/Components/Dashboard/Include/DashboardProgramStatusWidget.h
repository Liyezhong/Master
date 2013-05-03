#ifndef DASHBOARDPROGRAMSTATUS_H
#define DASHBOARDPROGRAMSTATUS_H

#include "MainMenu/Include/PanelFrame.h"


namespace Dashboard {

namespace Ui {
    class CDashboardProgramStatusWidget;
}

class CDashboardProgramStatusWidget : public MainMenu::CPanelFrame
{
    Q_OBJECT
    
public:
    explicit CDashboardProgramStatusWidget(QWidget *parent = 0);
    ~CDashboardProgramStatusWidget();
    
private:
    Ui::CDashboardProgramStatusWidget *ui;
private slots:
    void OnClose();
};
}
#endif // DASHBOARDPROGRAMSTATUS_H
