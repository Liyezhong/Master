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

protected:
    void changeEvent(QEvent *p_Event);

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    void RetranslateUI();
    Ui::CProgramPanelWidget *ui;
};

}// end namespace Dashboard

#endif // PROGRAMPANELWIDGET_H
