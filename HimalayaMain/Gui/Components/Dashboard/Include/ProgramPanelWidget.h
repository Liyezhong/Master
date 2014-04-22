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

/****************************************************************************/
/*!
 *  \brief  Definition/Declaration of class Dashboard::CProgramPanelWidget
 */
/****************************************************************************/
class CProgramPanelWidget : public MainMenu::CPanelFrame
{
    Q_OBJECT
    
public:
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function CProgramPanelWidget
     *
     *  \return from CProgramPanelWidget
     */
    /****************************************************************************/
    explicit CProgramPanelWidget(QWidget *parent = 0);
    ~CProgramPanelWidget();

    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function SetPtrToMainWindow
     *
     *  \return from SetPtrToMainWindow
     */
    /****************************************************************************/
    void SetPtrToMainWindow(MainMenu::CMainWindow *p_MainWindow, Core::CDataConnector *p_DataConnector);
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function ChangeStartButtonToStopState
     *
     *  \return from ChangeStartButtonToStopState
     */
    /****************************************************************************/
    void ChangeStartButtonToStopState();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function ChangeStartButtonToStartState
     *
     *  \return from ChangeStartButtonToStartState
     */
    /****************************************************************************/
    void ChangeStartButtonToStartState();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function EnableStartButton
     *
     *  \return from EnableStartButton
     */
    /****************************************************************************/
    void EnableStartButton(bool bEnable);
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function EnablePauseButton
     *
     *  \return from EnablePauseButton
     */
    /****************************************************************************/
    void EnablePauseButton(bool bEnable);
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function IsResumeRun
     *
     *  \return from IsResumeRun
     */
    /****************************************************************************/
    void IsResumeRun(bool bSet);
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function IsResumeRun
     *
     *  \return from IsResumeRun
     */
    /****************************************************************************/
    bool IsResumeRun();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function IsAbortEnabled
     *
     *  \return from IsAbortEnabled
     */
    /****************************************************************************/
    bool IsAbortEnabled();
signals:
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal AddItemsToFavoritePanel
     */
    /****************************************************************************/
    void AddItemsToFavoritePanel(bool bOnlyAddCleaningProgram);
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal PrepareSelectedProgramChecking
     */
    /****************************************************************************/
    void PrepareSelectedProgramChecking(const QString& selectedProgramId, bool bCheckEndDatetimeAgain = false);
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal OnSelectEndDateTime
     */
    /****************************************************************************/
    void OnSelectEndDateTime(const QDateTime &);
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal ProgramSelected
     */
    /****************************************************************************/
    void ProgramSelected(QString& ProgramId, int asapEndTime, bool bProgramStartReady, QList<QString>& selectedStationList);
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal ProgramActionStopped
     */
    /****************************************************************************/
    void ProgramActionStopped(DataManager::ProgramStatusType_t);
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal ProgramActionStarted
     */
    /****************************************************************************/
    void ProgramActionStarted(DataManager::ProgramActionType_t, int remainingTimeTotal, const QDateTime& startDateTime, bool IsResume);
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal CurrentProgramStepInforUpdated
     */
    /****************************************************************************/
    void CurrentProgramStepInforUpdated(const MsgClasses::CmdCurrentProgramStepInfor & Command);
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal UndoProgramSelection
     */
    /****************************************************************************/
    void UndoProgramSelection();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal UpdateProgram
     */
    /****************************************************************************/
    void UpdateProgram(DataManager::CProgram &);
protected:
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function changeEvent
     *
     *  \return from changeEvent
     */
    /****************************************************************************/
    void changeEvent(QEvent *p_Event);

public slots:
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of OnProgramStartReadyUpdated
     */
    /****************************************************************************/
     void OnProgramStartReadyUpdated();

private slots:
     /****************************************************************************/
     /*!
      *  \brief  Definition/Declaration of OnButtonClicked
      */
     /****************************************************************************/
    void OnButtonClicked(int whichBtn);
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of OnProgramSelected
     */
    /****************************************************************************/
    void OnProgramSelected(QString& ProgramId, int asapEndTime, bool bProgramStartReady, QList<QString>& selectedStationList);
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of SelectEndDateTime
     */
    /****************************************************************************/
    void SelectEndDateTime(const QDateTime &);
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of OnProgramActionStarted
     */
    /****************************************************************************/
    void OnProgramActionStarted(DataManager::ProgramActionType_t ProgramActionType, int remainingTimeTotal,
                                const QDateTime& startDateTime, bool IsResume);//in seconds
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of OnProgramActionStopped
     */
    /****************************************************************************/
    void OnProgramActionStopped(DataManager::ProgramStatusType_t ProgramStatusType);
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of SwitchToFavoritePanel
     */
    /****************************************************************************/
    void SwitchToFavoritePanel();

private:
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
     *  \brief  Definition/Declaration of function CheckPreConditionsToRunProgram
     *
     *  \return from CheckPreConditionsToRunProgram
     */
    /****************************************************************************/
    void CheckPreConditionsToRunProgram();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function CheckPreConditionsToPauseProgram
     *
     *  \return from CheckPreConditionsToPauseProgram
     */
    /****************************************************************************/
    bool CheckPreConditionsToPauseProgram();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function CheckPreConditionsToAbortProgram
     *
     *  \return from CheckPreConditionsToAbortProgram
     */
    /****************************************************************************/
    bool CheckPreConditionsToAbortProgram();


    Ui::CProgramPanelWidget *ui;       ///<  Definition/Declaration of variable ui
    QButtonGroup m_btnGroup;                                    //!< Button Group
    DataManager::ProgramActionType_t m_ProgramNextAction;       //!< Program Next Action
    bool m_IsResumeRun;       ///<  Definition/Declaration of variable m_IsResumeRun
    QString m_SelectedProgramId;       ///<  Definition/Declaration of variable m_SelectedProgramId
    Core::CDataConnector *mp_DataConnector;                     //!< Global data container
    DataManager::CHimalayaUserSettings* m_pUserSetting;       ///<  Definition/Declaration of variable m_pUserSetting
    QDateTime m_EndDateTime;       ///<  Definition/Declaration of variable m_EndDateTime
    MainMenu::CMessageDlg   *mp_MessageDlg;                      //!< Message Dialogue
    DataManager::CDataProgramList *mp_ProgramList;       ///<  Definition/Declaration of variable mp_ProgramList
    QList<QString> m_StationList;                                //!< Station list occupied
    QString m_strNotStartRMSOFF;       ///<  Definition/Declaration of variable m_strNotStartRMSOFF
    QString m_strConfirmation;       ///<  Definition/Declaration of variable m_strConfirmation
    QString m_strAbortProgram;       ///<  Definition/Declaration of variable m_strAbortProgram

    QString m_strWarning;       ///<  Definition/Declaration of variable m_strWarning
    QString m_strYes;       ///<  Definition/Declaration of variable m_strYes
    QString m_strCancel;       ///<  Definition/Declaration of variable m_strCancel
    QString m_strProgram;       ///<  Definition/Declaration of variable m_strProgram
    QString m_strInformation;       ///<  Definition/Declaration of variable m_strInformation
    QString m_strOK;       ///<  Definition/Declaration of variable m_strOK
    QString m_strNo;       ///<  Definition/Declaration of variable m_strNo
    QString m_strNotStartExpiredReagent;       ///<  Definition/Declaration of variable m_strNotStartExpiredReagent
    QString m_strStartExpiredReagent;       ///<  Definition/Declaration of variable m_strStartExpiredReagent
    QString m_strNeedMeltParaffin;       ///<  Definition/Declaration of variable m_strNeedMeltParaffin
};

}// end namespace Dashboard

#endif // PROGRAMPANELWIDGET_H

