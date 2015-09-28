/****************************************************************************/
#include "Global/Include/Utils.h"
#include "Global/Include/Exception.h"
#include "Reagents/Include/ReagentSubMenuWidget.h"
#include "ui_ReagentSubMenuWidget.h"

namespace Reagents {

/****************************************************************************/
/*!
 *  \brief Constructor
 *
 *  \iparam p_Parent = Reference to Parent object
 */
/****************************************************************************/
CReagentSubMenuWidget::CReagentSubMenuWidget(QWidget *p_Parent) : MainMenu::CPanelFrame(p_Parent),
    mp_Ui(new Ui::CReagentSubMenuWidget),
    m_IsRetranslateUi(true)
{
    mp_Ui->setupUi(GetContentFrame());
    SetPanelTitle(tr(""));
    QPalette Palette = mp_Ui->ReagentSubMenulist->palette();
    Palette.setColor(QPalette::Base, Qt::transparent);
    Palette.setColor(QPalette::Highlight, Qt::transparent);
    Palette.setColor(QPalette::HighlightedText, Qt::black);
    mp_Ui->ReagentSubMenulist->setPalette(Palette);
    mp_Ui->ReagentSubMenulist->setCurrentRow(0);
    CONNECTSIGNALSIGNAL(mp_Ui->ReagentSubMenulist, currentRowChanged(int), this, CurrentRowChanged(int))

}

/****************************************************************************/
/*!
 *  \brief Destructor
 */
/****************************************************************************/
CReagentSubMenuWidget::~CReagentSubMenuWidget()
{
    try {

        delete mp_Ui;

    }
    catch (...) {
        // to please Lint.
    }
}

/****************************************************************************/
/*!
 *  \brief Event handler for change events
 *
 *  \iparam p_Event = Change event
 */
/****************************************************************************/
void CReagentSubMenuWidget::changeEvent(QEvent *p_Event)
{
   QWidget::changeEvent(p_Event);
   switch (p_Event->type()) {
        case QEvent::LanguageChange:
            if (this->isVisible()) {
                mp_Ui->retranslateUi(this);
                m_IsRetranslateUi = false;
            }
            else {
                m_IsRetranslateUi = true;
            }
            RetranslateUI();
            break;
        default:
            break;
    }

}

/****************************************************************************/
/*!
 *  \brief Translates the strings in UI to the selected language
 */
/****************************************************************************/
void CReagentSubMenuWidget::RetranslateUI()
{
   MainMenu::CPanelFrame::SetPanelTitle(QApplication::translate("Reagents::CReagentSubMenuWidget", "", 0, QApplication::UnicodeUTF8));
}

void CReagentSubMenuWidget::showEvent(QShowEvent *p_Event)
{
    Q_UNUSED(p_Event);
    if (m_IsRetranslateUi) {
        mp_Ui->retranslateUi(this);
        m_IsRetranslateUi =false;
    }
}

}


