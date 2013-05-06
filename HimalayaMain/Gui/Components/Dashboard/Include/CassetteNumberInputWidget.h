#ifndef CASSETTEINPUT_H
#define CASSETTEINPUT_H

#include "MainMenu/Include/DialogFrame.h"
namespace MainMenu
{
    class CScrollWheel;
}

namespace Dashboard {
    namespace Ui {
        class CCassetteNumberInputWidget;
    }

    class CCassetteNumberInputWidget : public MainMenu::CDialogFrame
    {
        Q_OBJECT
    
    public:
        explicit CCassetteNumberInputWidget(QWidget *parent = 0);
        ~CCassetteNumberInputWidget();
        int CassetteNumber();
    private:
        Ui::CCassetteNumberInputWidget *ui;
        MainMenu::CScrollWheel *mp_singleWheel;
        MainMenu::CScrollWheel *mp_tenWheel;
        MainMenu::CScrollWheel *mp_hundredWheel;
        int m_CassetteNumber;
    private slots:
        void OnOK();
        void OnCancel();
    };
}

#endif // CASSETTEINPUT_H
