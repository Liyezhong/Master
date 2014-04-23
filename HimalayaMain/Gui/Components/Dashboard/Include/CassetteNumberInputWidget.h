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

/****************************************************************************/
/*!
 *  \brief  Definition/Declaration of class Dashboard::CCassetteNumberInputWidget
 */
/****************************************************************************/
    class CCassetteNumberInputWidget : public MainMenu::CDialogFrame
    {
        Q_OBJECT
    
    public:
        /****************************************************************************/
        /*!
         *  \brief  Definition/Declaration of function CCassetteNumberInputWidget
         *
         *  \param pParent = QWidget type parameter
         *  \param pMainWindow
         *
         *  \return from CCassetteNumberInputWidget
         */
        /****************************************************************************/
        explicit CCassetteNumberInputWidget(QWidget *pParent = 0, QMainWindow *pMainWindow = 0);
        ~CCassetteNumberInputWidget();
        /****************************************************************************/
        /*!
         *  \brief  Definition/Declaration of function CassetteNumber
         *
         *  \return from CassetteNumber
         */
        /****************************************************************************/
        int CassetteNumber();

    private:
        Ui::CCassetteNumberInputWidget *ui;       ///<  Definition/Declaration of variable ui
        MainMenu::CScrollWheel *mp_singleWheel;       ///<  Definition/Declaration of variable mp_singleWheel
        MainMenu::CScrollWheel *mp_tenWheel;       ///<  Definition/Declaration of variable mp_tenWheel
        MainMenu::CScrollWheel *mp_hundredWheel;       ///<  Definition/Declaration of variable mp_hundredWheel
        int m_CassetteNumber;       ///<  Definition/Declaration of variable m_CassetteNumber
        QString m_strMsg;       ///<  Definition/Declaration of variable m_strMsg
        QString m_strWarning;       ///<  Definition/Declaration of variable m_strWarning
        QString m_strOK;       ///<  Definition/Declaration of variable m_strOK
        /****************************************************************************/
        /*!
         *  \brief  Definition/Declaration of function RetranslateUI
         *
         *  \return from RetranslateUI
         */
        /****************************************************************************/
        void RetranslateUI();
    private slots:
        /****************************************************************************/
        /*!
         *  \brief  Definition/Declaration of OnOK
         */
        /****************************************************************************/
        void OnOK();
        /****************************************************************************/
        /*!
         *  \brief  Definition/Declaration of OnCancel
         */
        /****************************************************************************/
        void OnCancel();
    };
}

#endif // CASSETTEINPUT_H

