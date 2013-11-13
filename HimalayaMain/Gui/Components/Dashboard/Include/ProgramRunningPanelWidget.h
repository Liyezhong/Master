#ifndef PROGRAMRUNNINGPANELWIDGET_H
#define PROGRAMRUNNINGPANELWIDGET_H

#include "Dashboard/Include/IconTitlePanelFrame.h"

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

    private:
        Ui::CProgramRunningPanelWidget *ui;
    };
}

#endif // PROGRAMRUNNINGPANELWIDGET_H
