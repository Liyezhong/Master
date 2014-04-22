#ifndef CDASHBOARDWIDGET2_H
#define CDASHBOARDWIDGET2_H

#include <QWidget>
#include <HimalayaDataContainer/Helper/Include/Global.h>
#include  <QDateTime>
#include "MainMenu/Include/MainWindow.h"

namespace Core
{
    class CDataConnector;
}

namespace MsgClasses
{
    class CmdProgramSelectedReply;
    class CmdStationSuckDrain;
    class CmdLockStatus;
    class CmdCurrentProgramStepInfor;
}


namespace MainMenu
{
    class CMainWindow;
    class CMessageDlg;
}

namespace DataManager
{
    class CDataProgramList;
    class CProgram;
    class CHimalayaUserSettings;
    class CUserSettings;
}

namespace Dashboard {

namespace Ui {
    class CDashboardWidget;
}

/****************************************************************************/
/*!
 *  \brief  Definition/Declaration of class Dashboard::CDashboardWidget
 */
/****************************************************************************/
class CDashboardWidget : public QWidget
{
    Q_OBJECT
    
public:
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function CDashboardWidget
     *
     *  \return from CDashboardWidget
     */
    /****************************************************************************/
    explicit CDashboardWidget(Core::CDataConnector *p_DataConnector,
                               MainMenu::CMainWindow *p_Parent = NULL);
    ~CDashboardWidget();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function IsAbortEnabled
     *
     *  \return from IsAbortEnabled
     */
    /****************************************************************************/
    bool IsAbortEnabled();
protected:
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function changeEvent
     *
     *  \return from changeEvent
     */
    /****************************************************************************/
    void changeEvent(QEvent *p_Event);

private:
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function IsParaffinInProgram
     *
     *  \return from IsParaffinInProgram
     */
    /****************************************************************************/
    bool IsParaffinInProgram(const DataManager::CProgram* p_Program);
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function GetASAPTime
     *
     *  \return from GetASAPTime
     */
    /****************************************************************************/
    int GetASAPTime(int, int, int, bool&);
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function RetranslateUI
     *
     *  \return from RetranslateUI
     */
    /****************************************************************************/
    void RetranslateUI();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function TakeOutSpecimenAndWaitRunCleaning
     *
     *  \return from TakeOutSpecimenAndWaitRunCleaning
     */
    /****************************************************************************/
    void TakeOutSpecimenAndWaitRunCleaning();
    Ui::CDashboardWidget *ui;       ///<  Definition/Declaration of variable ui
    Core::CDataConnector *mp_DataConnector;          //!< Data object
    MainMenu::CMainWindow *mp_MainWindow;           //!< Reference to main window.
    DataManager::CDataProgramList *mp_ProgramList;       ///<  Definition/Declaration of variable mp_ProgramList
    QString m_NewSelectedProgramId;       ///<  Definition/Declaration of variable m_NewSelectedProgramId
    int m_ParaffinStepIndex;       ///<  Definition/Declaration of variable m_ParaffinStepIndex
    DataManager::CHimalayaUserSettings* m_pUserSetting;       ///<  Definition/Declaration of variable m_pUserSetting
    MainMenu::CMessageDlg   *mp_MessageDlg;                      //!< Message Dialogue
    QString m_strCheckSafeReagent;       ///<  Definition/Declaration of variable m_strCheckSafeReagent
    QString m_strNotFoundStation;       ///<  Definition/Declaration of variable m_strNotFoundStation
    QString m_strCheckEmptyStation;       ///<  Definition/Declaration of variable m_strCheckEmptyStation
    static QString m_SelectedProgramId;       ///<  Definition/Declaration of variable m_SelectedProgramId
    QList<QString> m_StationList;       ///<  Definition/Declaration of variable m_StationList
    int m_TimeProposed;       ///<  Definition/Declaration of variable m_TimeProposed
    QDateTime m_EndDateTime;       ///<  Definition/Declaration of variable m_EndDateTime
    bool m_CheckEndDatetimeAgain;       ///<  Definition/Declaration of variable m_CheckEndDatetimeAgain
    QString m_strResetEndTime;       ///<  Definition/Declaration of variable m_strResetEndTime
    QString m_strInputCassetteBoxTitle;       ///<  Definition/Declaration of variable m_strInputCassetteBoxTitle
    bool m_ProgramStartReady;       ///<  Definition/Declaration of variable m_ProgramStartReady
    QString m_strProgramComplete;       ///<  Definition/Declaration of variable m_strProgramComplete
    QString m_strTakeOutSpecimen;       ///<  Definition/Declaration of variable m_strTakeOutSpecimen
    QString m_strRetortContaminated;       ///<  Definition/Declaration of variable m_strRetortContaminated
    QDateTime m_StartDateTime;       ///<  Definition/Declaration of variable m_StartDateTime
    QString m_strProgramIsAborted;       ///<  Definition/Declaration of variable m_strProgramIsAborted
    bool m_IsWaitingCleaningProgram;       ///<  Definition/Declaration of variable m_IsWaitingCleaningProgram
    int m_CurProgramStepIndex;       ///<  Definition/Declaration of variable m_CurProgramStepIndex
    MainMenu::CMainWindow::UserRole_t m_CurrentUserRole;        //!< Current user role
    bool m_ProcessRunning;                      //!< Process running state
    bool m_IsDraining;       ///<  Definition/Declaration of variable m_IsDraining

public slots:
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of OnUnselectProgram
     */
    /****************************************************************************/
    void OnUnselectProgram();


private slots:
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of PrepareSelectedProgramChecking
     */
    /****************************************************************************/
    void PrepareSelectedProgramChecking(const QString& selectedProgramId, bool bCheckEndDatetimeAgain);
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of OnProgramSelectedReply
     */
    /****************************************************************************/
    void OnProgramSelectedReply(const MsgClasses::CmdProgramSelectedReply& cmd);
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of OnSelectEndDateTime
     */
    /****************************************************************************/
    void OnSelectEndDateTime(const QDateTime&);
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of OnProgramStartReadyUpdated
     */
    /****************************************************************************/
    void OnProgramStartReadyUpdated();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of OnProgramWillComplete
     */
    /****************************************************************************/
    void OnProgramWillComplete();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of OnProgramAborted
     */
    /****************************************************************************/
    void OnProgramAborted();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of OnProgramBeginAbort
     */
    /****************************************************************************/
    void OnProgramBeginAbort();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of OnProgramCompleted
     */
    /****************************************************************************/
    void OnProgramCompleted();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of OnProgramRunBegin
     */
    /****************************************************************************/
    void OnProgramRunBegin();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of OnProgramPaused
     */
    /****************************************************************************/
    void OnProgramPaused();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of OnStationSuckDrain
     */
    /****************************************************************************/
    void OnStationSuckDrain(const MsgClasses::CmdStationSuckDrain & cmd);
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of OnRetortLockStatusChanged
     */
    /****************************************************************************/
    void OnRetortLockStatusChanged(const MsgClasses::CmdLockStatus& cmd);
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of OnCurrentProgramStepInforUpdated
     */
    /****************************************************************************/
    void OnCurrentProgramStepInforUpdated(const MsgClasses::CmdCurrentProgramStepInfor & cmd);
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of OnUserRoleChanged
     */
    /****************************************************************************/
    void OnUserRoleChanged();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of OnProcessStateChanged
     */
    /****************************************************************************/
    void OnProcessStateChanged();
 signals:
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal AddItemsToFavoritePanel
     */
    /****************************************************************************/
    void AddItemsToFavoritePanel(bool bOnlyAddCleaningProgram = false);
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal ProgramSelected
     */
    /****************************************************************************/
    void ProgramSelected(QString & programId, QList<QString>& selectedStationList);
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal ProgramSelected
     */
    /****************************************************************************/
    void ProgramSelected(QString & programId, int asapEndTime, bool bProgramStartReady, QList<QString>& selectedStationList);
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal UpdateSelectedStationList
     */
    /****************************************************************************/
    void UpdateSelectedStationList(QList<QString>&);
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal ProgramActionStarted
     */
    /****************************************************************************/
    void ProgramActionStarted(DataManager::ProgramActionType_t, int remainingTimeTotal, const QDateTime& startDateTime, bool IsResume);
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal ProgramActionStopped
     */
    /****************************************************************************/
    void ProgramActionStopped(DataManager::ProgramStatusType_t);
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal UpdateUserSetting
     */
    /****************************************************************************/
    void UpdateUserSetting(DataManager::CUserSettings&);
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal SwitchToFavoritePanel
     */
    /****************************************************************************/
    void SwitchToFavoritePanel();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal UndoProgramSelection
     */
    /****************************************************************************/
    void UndoProgramSelection();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal OnInteractStart
     */
    /****************************************************************************/
    void OnInteractStart();    
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal UpdateProgram
     */
    /****************************************************************************/
    void UpdateProgram(DataManager::CProgram &);
};

} // end namespace Dashboard

#endif // CDASHBOARDWIDGET2_H

