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

    class CProgramRunningPanelWidget : public Dashboard::CIconTitlePanelFrame
    {
        Q_OBJECT

    public:
        explicit CProgramRunningPanelWidget(QWidget *parent = 0);
        ~CProgramRunningPanelWidget();
        void SetUserSettings(DataManager::CUserSettings *pUserSettings);
        int  GetStepRemainingTime();
        int GetProgramRemainingTime();
        const QString GetEndDateTime();
        void EnableProgramDetailButton(bool bEnable);
    public slots:
        void OnProgramActionStarted(DataManager::ProgramActionType_t ProgramActionType, int remainingTimeTotal,
                                    const QDateTime& startDateTime, bool IsResume);//in seconds
        void OnProgramActionStopped(DataManager::ProgramStatusType_t ProgramStatusType);
        void OnCurrentProgramStepInforUpdated(const MsgClasses::CmdCurrentProgramStepInfor &);


    protected:
        void changeEvent(QEvent *p_Event);

    signals:
        void AbortClicked(int);
    private slots:
        void UpdateProgress();
        void ProgramSelected(QString& programId, int asapEndTime, bool bProgramStartReady, QList<QString>& selectedStationList);
        void OnUserSetEndDateTime(const QDateTime& dateTime);
        void OnUserSettingsUpdated();
        void OnProcessStateChanged();
        void OnProgramDetail();
    private:
        void UpdateDateTime();
        void RetranslateUI();
        void GetStationNameList(QList<QString>& stationNameList);
        Ui::CProgramRunningPanelWidget *ui;
        QTimer* mp_ProgressTimer;
        int m_CurStepRemainingTime;
        int m_CurRemainingTime;
        int m_remainingTimeTotal;
        int m_curRemainingTimeTotal;
        QString m_strAborted;
        QString m_strCompleted;
        QString m_strAborting;
        QDateTime m_ProgramEndDateTime;
        DataManager::CUserSettings *mp_UserSettings;
        Global::DateFormat m_CurDateFormat;
        Global::TimeFormat m_CurTimeFormat;
        QString m_DateTimeStr;
        QString m_selectedProgramId;
        QList<QString> m_selectedStationList;
        int m_CurProgramStepIndex;
        bool m_isAborting;
    };
}

#endif // PROGRAMRUNNINGPANELWIDGET_H
