#ifndef CASSETTEINPUT_H
#define CASSETTEINPUT_H

#include "MainMenu/Include/PanelFrame.h"
namespace MainMenu
{
    class CScrollWheel;
}

namespace Dashboard {
    namespace Ui {
        class CCassetteNumberInputWidget;
    }

    class CCassetteNumberInputWidget : public MainMenu::CPanelFrame
    {
        Q_OBJECT
    
    public:
        explicit CCassetteNumberInputWidget(QWidget *parent = 0);
        ~CCassetteNumberInputWidget();
    
    private:
        Ui::CCassetteNumberInputWidget *ui;
        MainMenu::CScrollWheel *mp_singleWheel;
        MainMenu::CScrollWheel *mp_tenWheel;
        MainMenu::CScrollWheel *mp_hundredWheel;
    private slots:
        void OnOK();
    };
}

#endif // CASSETTEINPUT_H
