#ifndef PROGRAMRUNNINGPANELWIDGET_H
#define PROGRAMRUNNINGPANELWIDGET_H

#include "Dashboard/Include/IconTitlePanelFrame.h"
#include "HimalayaDataContainer/Helper/Include/Global.h"

class QDateTime;

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

    private:
        Ui::CProgramRunningPanelWidget *ui;
    };
}

#endif // PROGRAMRUNNINGPANELWIDGET_H
