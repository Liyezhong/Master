#ifndef BOTTLECHECKSTATUSDLG_H
#define BOTTLECHECKSTATUSDLG_H

#include "MainMenu/Include/DialogFrame.h"
#include "MainMenu/Include/BaseTable.h"
#include "Settings/Include/BottleCheckStatusModel.h"

namespace Settings {

namespace Ui {
    class CBottleCheckStatusDlg;
}

class CBottleCheckStatusDlg : public MainMenu::CDialogFrame
{
    Q_OBJECT
    
public:
    explicit CBottleCheckStatusDlg(QWidget *parent = 0);
    ~CBottleCheckStatusDlg();

protected:
    void changeEvent(QEvent *p_Event);

private:
    void RetranslateUI();
    Ui::CBottleCheckStatusDlg *ui;
    MainMenu::CBaseTable *mp_TableWidget;
    Settings::CBottleCheckStatusModel m_BottleCheckStatusModel;

};
}
#endif // BOTTLECHECKSTATUSDLG_H
