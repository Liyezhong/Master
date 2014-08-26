#ifndef PROGRAMRUNNINGPANELWIDGET_H
#define PROGRAMRUNNINGPANELWIDGET_H

#include "Dashboard/Include/IconTitlePanelFrame.h"
#include "HimalayaDataContainer/Helper/Include/Global.h"
#include <QTime>
#include "Global/Include/GlobalDefines.h"

class QDateTime;


namespace MsgClasses
{
    class CmdCurrentProgramStepInfor;
}

namespace DataManager
{
    class CUserSettings;
}

namespace Dashboard {
    namespace Ui {
        class CProgramRunningPanelWidget;
    }

/****************************************************************************/
/*!
 *  \brief  Definition/Declaration of class Dashboard::CProgramRunningPanelWidget
 */
/****************************************************************************/
    class CProgramRunningPanelWidget : public Dashboard::CIconTitlePanelFrame
    {
        Q_OBJECT

    public:
        /****************************************************************************/
        /*!
         *  \brief  Definition/Declaration of function CProgramRunningPanelWidget
         *
         *  \param parent = QWidget type parameter
         *
         *  \return from CProgramRunningPanelWidget
         */
        /****************************************************************************/
        explicit CProgramRunningPanelWidget(QWidget *parent = 0);
        ~CProgramRunningPanelWidget();
        /****************************************************************************/
        /*!
         *  \brief  Definition/Declaration of function SetUserSettings
         *
         *  \param pUserSettings = DataManager::CUserSettings type parameter
         *
         *  \return from SetUserSettings
         */
        /****************************************************************************/
        void SetUserSettings(DataManager::CUserSettings *pUserSettings);
        /****************************************************************************/
        /*!
         *  \brief  Definition/Declaration of function GetStepRemainingTime
         *
         *  \return from GetStepRemainingTime
         */
        /****************************************************************************/
        int  GetStepRemainingTime();
        /****************************************************************************/
        /*!
         *  \brief  Definition/Declaration of function GetProgramRemainingTime
         *
         *  \return from GetProgramRemainingTime
         */
        /****************************************************************************/
        int GetProgramRemainingTime();
        /****************************************************************************/
        /*!
         *  \brief  Definition/Declaration of function GetEndDateTime
         *
         *  \return from GetEndDateTime
         */
        /****************************************************************************/
        const QString GetEndDateTime();
        /****************************************************************************/
        /*!
         *  \brief  Definition/Declaration of function EnableProgramDetailButton
         *
         *  \param bEnable = bool type parameter
         *
         *  \return from EnableProgramDetailButton
         */
        /****************************************************************************/
        void EnableProgramDetailButton(bool bEnable);
    public slots:
        /****************************************************************************/
        /*!
         *  \brief  Definition/Declaration of OnProgramActionStarted
         *  \param  ProgramActionType = program operation type
         *  \param  remainingTimeTotal = the total remaining time in seconds
         *  \param  start DateTime for the selected program
         *  \param  whether resume to run or not
         */
        /****************************************************************************/
        void OnProgramActionStarted(DataManager::ProgramActionType_t ProgramActionType, int remainingTimeTotal,
                                    const QDateTime& startDateTime, bool IsResume);//in seconds
        /****************************************************************************/
        /*!
         *  \brief  Definition/Declaration of OnProgramActionStopped
         *  \param  ProgramStatusType = program satus type(paused/completed/aborted)
         */
        /****************************************************************************/
        void OnProgramActionStopped(DataManager::ProgramStatusType_t ProgramStatusType);
        /****************************************************************************/
        /*!
         *  \brief  Definition/Declaration of OnCurrentProgramStepInforUpdated
         */
        /****************************************************************************/
        void OnCurrentProgramStepInforUpdated(const MsgClasses::CmdCurrentProgramStepInfor &);


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

    signals:
        /****************************************************************************/
        /*!
         *  \brief  Definition/Declaration of signal AbortClicked
         */
        /****************************************************************************/
        void AbortClicked(int);
    private slots:
        /****************************************************************************/
        /*!
         *  \brief  Definition/Declaration of slot UpdateProgress
         */
        /****************************************************************************/
        void UpdateProgress();
        /****************************************************************************/
        /*!
         *  \brief  Definition/Declaration of slot ProgramSelected
         */
        /****************************************************************************/
        void ProgramSelected(QString& programId, int asapEndTime, bool bProgramStartReady, bool bIsFirstStepFixation, QList<QString>& selectedStationList);
        /****************************************************************************/
        /*!
         *  \brief  Definition/Declaration of slot UpdateEndDateTime
         */
        /****************************************************************************/
        void UpdateEndDateTime(int endTimeDiff);
        /****************************************************************************/
        /*!
         *  \brief  Definition/Declaration of slot OnUserSetEndDateTime
         */
        /****************************************************************************/
        void OnUserSetEndDateTime(const QDateTime& dateTime);
        /****************************************************************************/
        /*!
         *  \brief  Definition/Declaration of slot OnUserSettingsUpdated
         */
        /****************************************************************************/
        void OnUserSettingsUpdated();
        /****************************************************************************/
        /*!
         *  \brief  Definition/Declaration of slot OnProcessStateChanged
         */
        /****************************************************************************/
        void OnProcessStateChanged();
        /****************************************************************************/
        /*!
         *  \brief  Definition/Declaration of slot OnProgramDetail
         */
        /****************************************************************************/
        void OnProgramDetail();
        /****************************************************************************/
        /*!
         *  \brief Update Program Timer's Status
         */
        /****************************************************************************/
        void UpdateProgramTimerStatus(bool enable);
    private:
        void UpdateDateTime();
        /****************************************************************************/
        /*!
         *  \brief  Definition/Declaration of function UpdateDateTime
         *
         *  \param selDateTime =  QDateTime type parameter
         *
         *  \return from UpdateDateTime
         */
        /****************************************************************************/
        void UpdateDateTime(const QDateTime & selDateTime);
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
         *  \brief  Definition/Declaration of function GetStationNameList
         *
         *  \param stationNameList = QList<QString> type parameter
         *
         *  \return from GetStationNameList
         */
        /****************************************************************************/
        void GetStationNameList(QList<QString>& stationNameList);
        Ui::CProgramRunningPanelWidget *ui;       ///<  Definition/Declaration of variable ui
        QTimer* mp_ProgressTimer;       ///<  Definition/Declaration of variable mp_ProgressTimer
        int m_CurStepRemainingTime;       ///<  Definition/Declaration of variable m_CurStepRemainingTime
        int m_CurRemainingTime;       ///<  Definition/Declaration of variable m_CurRemainingTime
        int m_remainingTimeTotal;       ///<  Definition/Declaration of variable m_remainingTimeTotal
        int m_curRemainingTimeTotal;       ///<  Definition/Declaration of variable m_curRemainingTimeTotal
        QString m_strAborted;       ///<  Definition/Declaration of variable m_strAborted
        QString m_strCompleted;       ///<  Definition/Declaration of variable m_strCompleted
        QString m_strAborting;       ///<  Definition/Declaration of variable m_strAborting
        QDateTime m_ProgramEndDateTime;       ///<  Definition/Declaration of variable m_ProgramEndDateTime
        DataManager::CUserSettings *mp_UserSettings;       ///<  Definition/Declaration of variable mp_UserSettings
        Global::DateFormat m_CurDateFormat;       ///<  Definition/Declaration of variable m_CurDateFormat
        Global::TimeFormat m_CurTimeFormat;       ///<  Definition/Declaration of variable m_CurTimeFormat
        QString m_DateTimeStr;       ///<  Definition/Declaration of variable m_DateTimeStr
        QString m_selectedProgramId;       ///<  Definition/Declaration of variable m_selectedProgramId
        QList<QString> m_selectedStationList;       ///<  Definition/Declaration of variable m_selectedStationList
        int m_CurProgramStepIndex;       ///<  Definition/Declaration of variable m_CurProgramStepIndex
        bool m_isAborting;       ///<  Definition/Declaration of variable m_isAborting
    };
}

#endif // PROGRAMRUNNINGPANELWIDGET_H

