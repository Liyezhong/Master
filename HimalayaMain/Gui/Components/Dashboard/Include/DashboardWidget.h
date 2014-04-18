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

class CDashboardWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit CDashboardWidget(Core::CDataConnector *p_DataConnector,
                               MainMenu::CMainWindow *p_Parent = NULL);
    ~CDashboardWidget();
    bool IsAbortEnabled();
protected:
    void changeEvent(QEvent *p_Event);

private:
    bool IsParaffinInProgram(const DataManager::CProgram* p_Program);
    int GetASAPTime(int, int, int, bool&);
    void RetranslateUI();
    void TakeOutSpecimenAndWaitRunCleaning();
    Ui::CDashboardWidget *ui;
    Core::CDataConnector *mp_DataConnector;          //!< Data object
    MainMenu::CMainWindow *mp_MainWindow;           //!< Reference to main window.
    DataManager::CDataProgramList *mp_ProgramList;
    QString m_NewSelectedProgramId;
    int m_ParaffinStepIndex;
    DataManager::CHimalayaUserSettings* m_pUserSetting;
    MainMenu::CMessageDlg   *mp_MessageDlg;                      //!< Message Dialogue
    QString m_strCheckSafeReagent;
    QString m_strNotFoundStation;
    QString m_strCheckEmptyStation;
    static QString m_SelectedProgramId;
    QList<QString> m_StationList;
    int m_TimeProposedForProgram;           //!< Time costed only for the whole program, exclude the time for the delayed time
    int m_CostedTimeBeforeParaffin;         //!< Time costed before the program step of paraffin.
    QDateTime m_EndDateTime;
    QDateTime m_ParaffinStartHeatingTime;
    QString m_strResetEndTime;
    QString m_strInputCassetteBoxTitle;
    bool m_ProgramStartReady;
    QString m_strProgramComplete;
    QString m_strTakeOutSpecimen;
    QString m_strRetortContaminated;
    QDateTime m_StartDateTime;
    QString m_strProgramIsAborted;
    bool m_IsWaitingCleaningProgram;
    int m_CurProgramStepIndex;
    MainMenu::CMainWindow::UserRole_t m_CurrentUserRole;        //!< Current user role
    bool m_ProcessRunning;                      //!< Process running state
    bool m_IsDraining;
    bool m_bRetortLocked;
    QString m_strRetortNotLock;
    QString m_strNotStartRMSOFF;
    QString m_strNotStartExpiredReagent;
    int m_iWhichStepHasNoSafeReagent;
    QString m_strStartExpiredReagent;
public slots:
    void OnUnselectProgram();


private slots:
    void PrepareSelectedProgramChecking(const QString& selectedProgramId);
    void OnProgramSelectedReply(const MsgClasses::CmdProgramSelectedReply& cmd);
    void OnSelectEndDateTime(const QDateTime&);
    void RequstAsapDateTime();
    void OnProgramStartReadyUpdated();
    void OnProgramWillComplete();
    void OnProgramAborted();
    void OnProgramBeginAbort();
    void OnProgramCompleted();
    void OnProgramRunBegin();
    void OnProgramPaused();
    void OnStationSuckDrain(const MsgClasses::CmdStationSuckDrain & cmd);
    void OnRetortLockStatusChanged(const MsgClasses::CmdLockStatus& cmd);
    void OnCurrentProgramStepInforUpdated(const MsgClasses::CmdCurrentProgramStepInfor & cmd);
    void OnUserRoleChanged();
    void OnProcessStateChanged();
    void CheckPreConditionsToRunProgram();
 signals:
    void ResetFocus(bool reset);
    void AddItemsToFavoritePanel(bool bOnlyAddCleaningProgram = false);
    void ProgramSelected(QString & programId, QList<QString>& selectedStationList);
    void ProgramSelected(QString & programId, int asapEndTime, bool bProgramStartReady, QList<QString>& selectedStationList);
    void UpdateSelectedStationList(QList<QString>&);
    void ProgramActionStarted(DataManager::ProgramActionType_t, int remainingTimeTotal, const QDateTime& startDateTime, bool IsResume);
    void ProgramActionStopped(DataManager::ProgramStatusType_t);
    void UpdateUserSetting(DataManager::CUserSettings&);
    void SwitchToFavoritePanel();
    void UndoProgramSelection();
    void OnInteractStart();    
    void UpdateProgram(DataManager::CProgram &);
    void SendAsapDateTime(int asapDateTime);

};

} // end namespace Dashboard

#endif // CDASHBOARDWIDGET2_H
