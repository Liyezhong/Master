/****************************************************************************/
/*! \file DashboardWidget.h
 *
 *  \brief Dashboard Widget Class Definition.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2013-01-03
 *   $Author:  $ Swati Tiwari
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

#ifndef DASHBOARDWIDGET_H
#define DASHBOARDWIDGET_H

#include "MainMenu/Include/PanelFrame.h"
#include "MainMenu/Include/MainWindow.h"
#include "MainMenu/Include/MessageDlg.h"
#include "Dashboard/Include/DashboardScene.h"
#include "Dashboard/Include/DashboardComboBox.h"
#include "Dashboard/Include/DashboardDataTypes.h"
#include "HimalayaDataContainer/Helper/Include/Global.h"
#include "HimalayaDataContainer/Containers/Programs/Include/DataProgramList.h"
#include <QFrame>
#include <QGridLayout>


namespace Dashboard {

class CDashboardProgramStatusWidget;
class CCassetteNumberInputWidget;

namespace Ui {
    class CDashboardWidget;
}

/****************************************************************************/
/**
 * \brief This is the main widget for Dash Board.
 */
/****************************************************************************/
class CDashboardWidget : public MainMenu::CPanelFrame
{
    Q_OBJECT

private:
    Ui::CDashboardWidget *mp_Ui;                                //!< User interface
    MainMenu::CMainWindow *mp_MainWindow;                           //!< Reference to MainWindow.
    Core::CDataConnector *mp_DataConnector;                     //!< Global data container
    CDashboardScene *mp_DashboardScene;                         //!< Displays the station items
    QFrame *mp_Separator;                                       //!< Separator Line between View & Operation
    DataManager::ProgramActionType_t m_ProgramNextAction;       //!< Program Next Action
    DataManager::CDataProgramList *mp_ProgramList;
    QStringList m_FavProgramIDs;

    CDashboardProgramStatusWidget *mp_ProgramStatusWidget;
    static QString m_SelectedProgramId;
    QString m_NewSelectedProgramId;
    QButtonGroup m_btnGroup;                                    //!< Button Group
    MainMenu::CMainWindow::UserRole_t m_CurrentUserRole;        //!< Current user role
    bool m_UserRoleChanged;                                     //!< Flag to Verify the Change in User Role
    MainMenu::CMessageDlg   *mp_MessageDlg;                      //!< Message Dialogue
    int m_TimeProposed;
    QDateTime m_EndDateTime;
    int m_ParaffinStepIndex;
    bool m_IsWaitingCleaningProgram;
    bool m_IsResumeRun;
    QList<QString> m_StationList;
    int m_CurProgramStepIndex;
    bool m_IsDraining;
    bool m_CheckEndDatetimeAgain;
    bool m_ProcessRunning;                      //!< Process running state
    bool m_ProgramStartReady;
    QString m_strProgram;
    QString m_strInformation;
    QString m_strCleanNotRun;
    QString m_strOK, m_strNo;
    QString m_strNotStartRMSOFF;
    QString m_strNotStartExpiredReagent;
    QString m_strStartExpiredReagent;
    QString m_strConfirmation, m_strAbortProgram;
    QString m_strProgramComplete, m_strTakeOutSpecimen;
    QString m_strRetortContaminated;
    QString m_strProgramIsAborted, m_strNeedMeltParaffin;
    QString m_strResetEndTime, m_strInputCassetteBoxTitle;
    QString m_strNotFoundStation;
    QString m_strCheckEmptyStation;
    DataManager::CHimalayaUserSettings* m_pUserSetting;

    void EnablePlayButton(bool bSetEnable);
    void EnableAbortButton(bool bSetEnable);
    void EnableRetortSlider(bool bSetEnable);
    bool IsParaffinInProgram(const DataManager::CProgram* p_Program);
    int GetASAPTime(const DataManager::CProgram*, int, int, int, bool&);
    void PrepareSelectedProgramChecking();
    void TakeOutSpecimenAndWaitRunCleaning();
    void RetranslateUI();
public:
    explicit CDashboardWidget(Core::CDataConnector *p_DataConnector, MainMenu::CMainWindow *p_Parent = NULL);
    ~CDashboardWidget();

    void CheckPreConditionsToRunProgram();
    bool CheckPreConditionsToPauseProgram();
    bool CheckPreConditionsToAbortProgram();
    static const QString& SelectedProgramId();
    static bool CheckSelectedProgram(bool& bRevertSelectProgram, QString ProgramID = "");//the return value means the work flow can go continuely.
    static QString m_strWarning;
    static QString m_strYes, m_strCancel;
    static QString m_strMsgUnselect;

protected:
    void DrawSeparatorLine();
    void changeEvent(QEvent *p_Event);

signals:
    void ProgramAction(const QString& ProgramID, DataManager::ProgramActionType_t ActionType);
    void ProgramActionStarted(DataManager::ProgramActionType_t, int remainingTimeTotal, const QDateTime& startDateTime, bool IsResume);
    void ProgramActionStopped(DataManager::ProgramStatusType_t);
    void UpdateProgramName(QString SelectedProgramName);
    void UpdateDashboardSceneReagentStations(QString& ProgramID);
    void ProgramSelected(QString & ProgramId, int asapEndTime, QList<QString>& SelectedStationList);
    void UpdateUserSetting(DataManager::CUserSettings&);

public slots:
    void RetortSliderPositionChanged(MainMenu::CSliderControl::Position_t Position);
    void OnUserRoleChanged();
    void OnButtonClicked(int whichBtn);
    void OnUnselectProgram();
    void AddItemsToComboBox(bool bOnlyAddCleaningProgram = false);
    void OnActivated(int);
    void OnComboBoxButtonPress();
    void OnProgramStartReadyUpdated();
    void OnProgramWillComplete();
    void OnProgramAborted();
    void OnProgramBeginAbort();
    void OnProgramCompleted();
    void OnProgramRunBegin();
    void OnProcessStateChanged();

    void OnRetortLockStatusChanged(const MsgClasses::CmdRetortLockStatus& cmd);
    void OnProgramSelectedReply(const MsgClasses::CmdProgramSelectedReply& cmd);
    void OnCurrentProgramStepInforUpdated(const MsgClasses::CmdCurrentProgramStepInfor &);
    void OnStationSuckDrain(const MsgClasses::CmdStationSuckDrain & cmd);
    void OnSelectDateTime(const QDateTime&);
};

}
#endif // DASHBOARDWIDGET_H
