#ifndef PROGRAMPANELWIDGET_H
#define PROGRAMPANELWIDGET_H

#include "MainMenu/Include/PanelFrame.h"

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
    
private:
    Ui::CProgramPanelWidget *ui;
};
}// end namespace Dashboard
#endif // PROGRAMPANELWIDGET_H
