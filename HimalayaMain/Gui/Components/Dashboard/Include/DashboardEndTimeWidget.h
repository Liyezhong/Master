/****************************************************************************/
/*! \file DashboardEndTimeWidget.h
 *
 *  \brief DashboardComboBox definition.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 19-Mar-2013
 *   $Author:  $ Santhosha Kumar Raju
 *
 *  \b Company:
 *
 *       Leica Microsystems Ltd. Shanghai.
 *
 *  (C) Copyright 2012 by Leica Microsystems Shanghai. All rights reserved.
 *  This is unpublished proprietary source code of Leica. The copyright notice
 *  does not evidence any actual or intended publication.
 *
 */
/****************************************************************************/

#ifndef DASHBOARD_DASHBOARDENDTIMEWIDGET_H
#define DASHBOARD_DASHBOARDENDTIMEWIDGET_H


#include <QWidget>
#include "Dashboard/Include/DashboardDateTimeWidget.h"
#include "Core/Include/DataConnector.h"
#include "MainMenu/Include/MainWindow.h"
#include "HimalayaDataContainer/Containers/Programs/Include/Program.h"
#include "HimalayaDataContainer/Containers/Programs/Include/ProgramStep.h"
#include "DataManager/Containers/UserSettings/Include/UserSettings.h"

class QPlastiqueStyle;
namespace Dashboard {

namespace Ui {
    class CDashboardEndTimeWidget;
}

/****************************************************************************/
/**
 * \brief This is a touchscreen optimized customized widget for showing
 *  end time, reagent name, step time, progress bar of the currently running
 *  program on the dashboard
 */
/****************************************************************************/
class CDashboardEndTimeWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CDashboardEndTimeWidget(Core::CDataConnector *p_DataConnector, QWidget *p_Parent = NULL);
    ~CDashboardEndTimeWidget();
    void InitEndTimeWidgetItems();
    void UpdateEndTimeWidgetItems(DataManager::CProgram const *p_Program, int ProgramEndTimeInSecs);

protected:
    void paintEvent(QPaintEvent *);

public slots:
    void OnUserRoleChanged();
    void OnUserSettingsUpdated();
    void OnEndTimeButtonClicked();
    void UpdateDateTime(const QDateTime & selDateTime);
    void OnCurrentProgramStepInforUpdated(const MsgClasses::CmdCurrentProgramStepInfor& cmd);
    void UpdateProgress();
    void OnProgramActionStarted(DataManager::ProgramActionType_t ProgramActionType, int remainingTimeTotal,
                                const QDateTime& startDateTime, bool IsResume);//in seconds
    void OnProgramActionStopped(DataManager::ProgramActionType_t ProgramActionType);
private:
    Ui::CDashboardEndTimeWidget *mp_Ui;
    QPixmap         m_btnPixmap;
    QPixmap         m_progBarPixmap;
    QPixmap         m_backgroundPixmap;
    MainMenu::CMainWindow::UserRole_t m_CurrentUserRole;        //!< Current user role
    bool m_UserRoleChanged;                                     //!< Flag to Verify the Change in User Role
    Core::CDataConnector *mp_DataConnector;
    DataManager::CUserSettings *mp_UserSettings;
    Global::DateFormat m_CurDateFormat;
    Global::TimeFormat m_CurTimeFormat;
    QDateTime m_ProgramEndDateTime;
    QTime m_CurRemainingTime;
    Dashboard::CDashboardDateTimeWidget *mp_wdgtDateTime;
    DataManager::CProgram const *mp_Program;
    QPlastiqueStyle* mp_PlastiqueStyle;
    QTimer* mp_ProgressTimer;
    QDateTime m_startDateTime;
signals:
    void OnSelectDateTime(const QDateTime& selDateTime);

};

} // end namespace Dashboard

#endif // DASHBOARD_DASHBOARDENDTIMEWIDGET_H
