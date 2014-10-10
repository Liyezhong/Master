#ifndef CDASHBOARDWIDGET_H
#define CDASHBOARDWIDGET_H

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
    class CmdRecoveryFromPowerFailure;
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
     *  \param p_DataConnector
     *  \param p_Parent
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
     *  \param p_Event = QEvent type parameter
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
     *  \param p_Program =  DataManager::CProgram type parameter
     *
     *  \return from IsParaffinInProgram
     */
    /****************************************************************************/
    bool IsParaffinInProgram(const DataManager::CProgram* p_Program);
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function IsFixationInFirstStep
     *
     *  \return true if the first program step is fixation reagent group
     */
    /****************************************************************************/
    bool IsFixationInFirstStep();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function GetParaffinHeatingDuration
     *
     *  \return the paraffin heating duration
     */
    /****************************************************************************/
    int GetParaffinHeatingDuration();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function GetASAPTime
     *
     *  \return from GetASAPTime
     */
    /****************************************************************************/
    int GetASAPTime(int, int, int, int&, bool&);
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function RetranslateUI
     *
     *  \return from RetranslateUI
     */
    /****************************************************************************/
    void RetranslateUI();
    void SetCassetteNumber();
    bool IsOKPreConditionsToRunProgram();
    Ui::CDashboardWidget *ui;       ///<  Definition/Declaration of variable ui
    Core::CDataConnector *mp_DataConnector;          //!< Data object
    MainMenu::CMainWindow *mp_MainWindow;           //!< Reference to main window.
    DataManager::CDataProgramList *mp_ProgramList;       ///<  Definition/Declaration of variable mp_ProgramList
    QString m_NewSelectedProgramId;       ///<  Definition/Declaration of variable m_NewSelectedProgramId
    int m_ParaffinStepIndex;       ///<  Definition/Declaration of variable m_ParaffinStepIndex
    DataManager::CHimalayaUserSettings* m_pUserSetting;       ///<  Definition/Declaration of variable m_pUserSetting
    MainMenu::CMessageDlg   *mp_MessageDlg;                      //!< Message Dialogue
    QString m_strCheckSafeReagent;
    QString m_strNotFoundStation;
    QString m_strCheckEmptyStation;
    static QString m_SelectedProgramId;
    QList<QString> m_StationList;
    int m_TimeProposedForProgram;           //!< Time costed only for the whole program, exclude the time for the delayed time
    int m_CostedTimeBeforeParaffin;         //!< Time costed before the program step of paraffin.
    int m_ParaffinHeatingDuration;
    QDateTime m_EndDateTime;
    int m_TimeDelta;
    QDateTime m_AsapEndDateTime;
    QDateTime m_ParaffinStartHeatingTime;
    QString m_strResetEndTime;
    QString m_strCannotStartParaffinMelt;
    QString m_strPromptProgramDelay;
    QString m_strInputCassetteBoxTitle;
    bool m_ProgramStartReady;
    QString m_strProgramComplete;
    QString m_strCleaningProgramComplete;
    QString m_strTissueProtectPassed;
    QString m_strOvenCoverOpen;
    QString m_strRetortCoverOpen;
    QString m_strTakeOutSpecimen;
    QString m_strRetortContaminated;
    QDateTime m_StartDateTime;
    QString m_strProgramIsAborted;
    bool m_IsWaitingCleaningProgram;
    int m_CurProgramStepIndex;
    MainMenu::CMainWindow::UserRole_t m_CurrentUserRole;        //!< Current user role
    bool m_ProcessRunning;                      //!< Process running state
    bool m_IsDrainingWhenPrgrmCompleted;
    bool m_bRetortLocked;
    QString m_strRetortNotLock;
    QString m_strNotStartRMSOFF;
    QString m_strNotStartExpiredReagent;
    int m_iWhichStepHasNoSafeReagent;
    QString m_strStartExpiredReagent;
    QString m_strChangeCassetteBoxTitle;
    QString m_strAddCassete;
    bool m_bIsFirstStepFixation;
    quint32 m_TotalCassette;
    bool m_HaveSucked;
public slots:
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of OnUnselectProgram
     */
    /****************************************************************************/
    void OnUnselectProgram();


private slots:
    void PrepareSelectedProgramChecking(const QString& selectedProgramId);
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of OnProgramSelectedReply
     */
    /****************************************************************************/
    void OnProgramSelectedReply(const MsgClasses::CmdProgramSelectedReply& cmd);
    void OnRecoveryFromPowerFailure(const MsgClasses::CmdRecoveryFromPowerFailure& cmd);
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of OnSelectEndDateTime
     */
    /****************************************************************************/
    void OnSelectEndDateTime(const QDateTime&);
    void RequestAsapDateTime();
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
     *  \brief  Definition/Declaration of OnTissueProtectPassed
     */
    /****************************************************************************/
    void OnTissueProtectPassed();

    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of OnOvenCoverOpen
     */
    /****************************************************************************/
    void OnOvenCoverOpen();

    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of OnRetortCoverOpen
     */
    /****************************************************************************/
    void OnRetortCoverOpen();

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
     *  \brief  Definition/Declaration of OnPauseButtonEnble
     */
    /****************************************************************************/
    void OnPauseButtonEnable(bool bEnable);
    /****************************************************************************/
    /*!
    *  \brief  Definition/Declaration of OnPauseTimeout15Mintues
    */
    /****************************************************************************/
    void OnPauseTimeout15Mintues();
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
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of CheckPreConditionsToRunProgram
     */
    /****************************************************************************/
    void CheckPreConditionsToRunProgram();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function TakeOutSpecimenAndWaitRunCleaning
     *
     *  \return from TakeOutSpecimenAndWaitRunCleaning
     */
    /****************************************************************************/
    void TakeOutSpecimenAndWaitRunCleaning();

 signals:
    /****************************************************************************/
    /*!
     *  \brief  refresh the program selection
     *  \param reset = whether reset the selected program or not
     */
    /****************************************************************************/
    void ResetFocus(bool reset);
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal AddItemsToFavoritePanel
     *  \param  bOnlyAddCleaningProgram = whether only show cleaning program or not
     */
    /****************************************************************************/
    void AddItemsToFavoritePanel(bool bOnlyAddCleaningProgram = false);
    /****************************************************************************/
    /*!
     *  \brief  when selete a program, emit signal ProgramSelected
     *  \param  programId = the seleted program ID
     *  \param  selectedStationList = the used station for the selected program
     */
    /****************************************************************************/
    void ProgramSelected(QString & programId, QList<QString>& selectedStationList);
    /****************************************************************************/
    /*!
     *  \brief  when selete a program, emit signal ProgramSelected
     *  \param  programId = the seleted program ID
     *  \param asapEndTime = as soon as possible end time
     *  \param bProgramStartReady = whether program start is ready or not
     *  \param bIsFirstStepFixation = the first program step is fixation reagent
     *  \param selectedStationList = the selected stations
     */
    /****************************************************************************/
    void ProgramSelected(QString & programId, int asapEndTime, bool bProgramStartReady, bool bIsFirstStepFixation, QList<QString>& selectedStationList);
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal UpdateSelectedStationList
     */
    /****************************************************************************/
    void UpdateSelectedStationList(QList<QString>&);
    /****************************************************************************/
    /*!
     *  \brief  Declaration of signal ProgramActionStarted
     *  \param programActionType = opration type, eg.Start/pause/abort.
     *  \param  remainingTimeTotal = the total remaining time
     *  \param  startDateTime = program start datetime
     *  \param IsResume = is resume to run program
     */
    /****************************************************************************/
    void ProgramActionStarted(DataManager::ProgramActionType_t programActionType, int remainingTimeTotal, const QDateTime& startDateTime, bool IsResume);
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
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal SendAsapDateTime
     *  \param asapDateTime = as soon as possible datetime
     *  \param bIsFirstStepFixation = whether the first step is fixation reagent or not
     */
    /****************************************************************************/
    void SendAsapDateTime(int asapDateTime, bool bIsFirstStepFixation);

};

} // end namespace Dashboard

#endif // CDASHBOARDWIDGET_H

