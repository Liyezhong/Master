#ifndef PROGRAMPANELWIDGET_H
#define PROGRAMPANELWIDGET_H

#include "MainMenu/Include/PanelFrame.h"
#include <QButtonGroup>
#include "HimalayaDataContainer/Helper/Include/Global.h"
#include <QDateTime>
#include "HimalayaDataContainer/Containers/Programs/Include/Program.h"


namespace MsgClasses
{
    class CmdCurrentProgramStepInfor;
}

namespace MainMenu
{
    class CMainWindow ;
    class CMessageDlg;
}

namespace Core
{
    class CDataConnector;
}

namespace DataManager
{
    class CHimalayaUserSettings;
    class CDataProgramList;
}

namespace Dashboard {

namespace Ui {
    class CProgramPanelWidget;
}

class CProgramPanelWidget : public MainMenu::CPanelFrame
{
    Q_OBJECT
    
public:
    explicit CProgramPanelWidget(QWidget *parent = 0);
    ~CProgramPanelWidget();

    void SetPtrToMainWindow(MainMenu::CMainWindow *p_MainWindow, Core::CDataConnector *p_DataConnector);
    void ChangeStartButtonToStopState();
    void ChangeStartButtonToStartState();
    void EnableStartButton(bool bEnable);
    void EnablePauseButton(bool bEnable);
    void IsResumeRun(bool bSet);
    bool IsResumeRun();
    bool IsAbortEnabled();
signals:
    void ResetFocus(bool reset);
    void AddItemsToFavoritePanel(bool bOnlyAddCleaningProgram);
    void PrepareSelectedProgramChecking(const QString& selectedProgramId, bool bCheckEndDatetimeAgain = false);
    void OnSelectEndDateTime(const QDateTime &);
    void ProgramSelected(QString& ProgramId, int asapEndTime, bool bProgramStartReady, QList<QString>& selectedStationList);
    void ProgramActionStopped(DataManager::ProgramStatusType_t);
    void ProgramActionStarted(DataManager::ProgramActionType_t, int remainingTimeTotal, const QDateTime& startDateTime, bool IsResume);
    void CurrentProgramStepInforUpdated(const MsgClasses::CmdCurrentProgramStepInfor & Command);
    void UndoProgramSelection();
    void UpdateProgram(DataManager::CProgram &);
protected:
    void changeEvent(QEvent *p_Event);

public slots:
     void OnProgramStartReadyUpdated();

private slots:
    void OnButtonClicked(int whichBtn);
    void OnProgramSelected(QString& ProgramId, int asapEndTime, bool bProgramStartReady, QList<QString>& selectedStationList);
    void SelectEndDateTime(const QDateTime &);
    void OnProgramActionStarted(DataManager::ProgramActionType_t ProgramActionType, int remainingTimeTotal,
                                const QDateTime& startDateTime, bool IsResume);//in seconds
    void OnProgramActionStopped(DataManager::ProgramStatusType_t ProgramStatusType);
    void SwitchToFavoritePanel();

private:
    void RetranslateUI();
    void CheckPreConditionsToRunProgram();
    bool CheckPreConditionsToPauseProgram();
    bool CheckPreConditionsToAbortProgram();


    Ui::CProgramPanelWidget *ui;
    QButtonGroup m_btnGroup;                                    //!< Button Group
    DataManager::ProgramActionType_t m_ProgramNextAction;       //!< Program Next Action
    bool m_IsResumeRun;
    QString m_SelectedProgramId;
    Core::CDataConnector *mp_DataConnector;                     //!< Global data container
    DataManager::CHimalayaUserSettings* m_pUserSetting;
    QDateTime m_EndDateTime;
    MainMenu::CMessageDlg   *mp_MessageDlg;                      //!< Message Dialogue
    DataManager::CDataProgramList *mp_ProgramList;
    QList<QString> m_StationList;                                //!< Station list occupied
    QString m_strNotStartRMSOFF;
    QString m_strConfirmation;
    QString m_strAbortProgram;

    QString m_strWarning;
    QString m_strYes, m_strCancel;
    QString m_strProgram;
    QString m_strInformation;
    QString m_strOK, m_strNo;
    QString m_strNotStartExpiredReagent;
    QString m_strStartExpiredReagent;
    QString m_strNeedMeltParaffin;
};

}// end namespace Dashboard

#endif // PROGRAMPANELWIDGET_H
