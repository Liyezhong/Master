#ifndef PROGRAMRUNNINGPANELWIDGET_H
#define PROGRAMRUNNINGPANELWIDGET_H

#include "Dashboard/Include/IconTitlePanelFrame.h"
#include "HimalayaDataContainer/Helper/Include/Global.h"
#include <QTime>

class QDateTime;


namespace MsgClasses
{
    class CmdCurrentProgramStepInfor;
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

    public slots:
        void OnProgramActionStarted(DataManager::ProgramActionType_t ProgramActionType, int remainingTimeTotal,
                                    const QDateTime& startDateTime, bool IsResume);//in seconds
        void OnProgramActionStopped(DataManager::ProgramStatusType_t ProgramStatusType);
        void OnCurrentProgramStepInforUpdated(const MsgClasses::CmdCurrentProgramStepInfor &);

    protected:
        void changeEvent(QEvent *p_Event);

    private slots:
        void UpdateProgress();
        void ProgramSelected(QString& ProgramId, int asapEndTime, bool bProgramStartReady);
    private:
        void RetranslateUI();
        Ui::CProgramRunningPanelWidget *ui;
        QTimer* mp_ProgressTimer;
        QTime m_CurStepRemainingTime;
        QTime m_CurRemainingTime;
        int m_remainingTimeTotal;
        int m_curRemainingTimeTotal;
        QString m_strAborted;
        QString m_strCompleted;
        QString m_strAborting;
        QDateTime m_ProgramEndDateTime;


    };
}

#endif // PROGRAMRUNNINGPANELWIDGET_H
