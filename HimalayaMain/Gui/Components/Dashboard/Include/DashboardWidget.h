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
    class CmdRetortLockStatus;
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
    static bool CheckSelectedProgram(bool& bRevertSelectProgram,
                                     QString ProgramID = "");//the return value(true) means the work flow can go continuely.

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
    int m_TimeProposed;
    QDateTime m_EndDateTime;
    bool m_CheckEndDatetimeAgain;
    QString m_strResetEndTime;
    QString m_strInputCassetteBoxTitle;
    bool m_ProgramStartReady;
    QString m_strProgramComplete;
    QString m_strTakeOutSpecimen;
    QString m_strRetortContaminated;
    QDateTime m_StartDateTime;
    QString m_strProgramIsAborted;
    static QString m_strMsgUnselect;
    bool m_IsWaitingCleaningProgram;
    int m_CurProgramStepIndex;
    MainMenu::CMainWindow::UserRole_t m_CurrentUserRole;        //!< Current user role
    bool m_ProcessRunning;                      //!< Process running state
    bool m_IsDraining;

public slots:
    void OnUnselectProgram();


private slots:
    void PrepareSelectedProgramChecking(const QString& selectedProgramId, bool bCheckEndDatetimeAgain);
    void OnProgramSelectedReply(const MsgClasses::CmdProgramSelectedReply& cmd);
    void OnSelectEndDateTime(const QDateTime&);
    void OnProgramStartReadyUpdated();
    void OnProgramWillComplete();
    void OnProgramAborted();
    void OnProgramBeginAbort();
    void OnProgramCompleted();
    void OnProgramRunBegin();
    void OnProgramPaused();
    void OnStationSuckDrain(const MsgClasses::CmdStationSuckDrain & cmd);
    void OnRetortLockStatusChanged(const MsgClasses::CmdRetortLockStatus& cmd);
    void OnCurrentProgramStepInforUpdated(const MsgClasses::CmdCurrentProgramStepInfor & cmd);
    void OnUserRoleChanged();
    void OnProcessStateChanged();
 signals:
    void AddItemsToFavoritePanel(bool bOnlyAddCleaningProgram = false);
    void ProgramSelected(QString & ProgramId, QList<QString>& SelectedStationList);
    void ProgramSelected(QString & ProgramId, int asapEndTime, bool bProgramStartReady);
    void UpdateSelectedStationList(QList<QString>&);
    void ProgramActionStarted(DataManager::ProgramActionType_t, int remainingTimeTotal, const QDateTime& startDateTime, bool IsResume);
    void ProgramActionStopped(DataManager::ProgramStatusType_t);
    void UpdateUserSetting(DataManager::CUserSettings&);
    void SetProgramNextActionAsStart();
    void SwitchToFavoritePanel();
    void UndoProgramSelection();

};

} // end namespace Dashboard

#endif // CDASHBOARDWIDGET2_H
