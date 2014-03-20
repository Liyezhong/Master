#ifndef DASHBOARDPROGRAMSTATUS_H
#define DASHBOARDPROGRAMSTATUS_H

#include "MainMenu/Include/DialogFrame.h"
#include "Programs/Include/StepModel.h"

namespace MainMenu
{
    class CBaseTable;
}

namespace Core
{
    class CDataConnector;
}

namespace DataManager
{
    class CProgram ;
}

namespace Dashboard {

namespace Ui {
    class CDashboardProgramStatusWidget;
}

class CDashboardProgramStatusWidget : public MainMenu::CDialogFrame
{
    Q_OBJECT
    
public:
    explicit CDashboardProgramStatusWidget(QWidget *parent = 0);
    ~CDashboardProgramStatusWidget();
     void InitDialog(DataManager::CProgram*, const Core::CDataConnector*, QList<QString>&, int, int,
                     int, const QString& endDateTime, bool bAbortButtonEnabled);

protected:
     void changeEvent(QEvent *p_Event);

private:
    Ui::CDashboardProgramStatusWidget *ui;
    MainMenu::CBaseTable *mp_TableWidget;
    Programs::CStepModel m_StepModel;
    void ResizeHorizontalSection();
    void RetranslateUI();
private slots:
    void OnClose();
    void OnAbort();
signals:
    void AbortClicked(int);
};
}
#endif // DASHBOARDPROGRAMSTATUS_H
