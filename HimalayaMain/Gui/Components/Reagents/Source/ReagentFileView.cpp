#include "Reagents/Include/ReagentFileView.h"
#include <QApplication>
#include <QEvent>

namespace Reagents {
/****************************************************************************/
/*!
 *  \brief Constructor
 *
 *  \iparam p_Parent = Parent object
 */
/****************************************************************************/
CReagentFileView::CReagentFileView(QWidget *p_Parent) : MainMenu::CPanelFrame(p_Parent)
{
    mp_FileView = new MainMenu::CFileView(this);
    SetContent(mp_FileView->layout());
    SetPanelTitle(tr("EventView"));
}

/****************************************************************************/
/*!
 *  \brief Destructor
 */
/****************************************************************************/
CReagentFileView::~CReagentFileView()
{
    try {
        delete mp_FileView;
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
void CReagentFileView::changeEvent(QEvent *p_Event)
{
    QWidget::changeEvent(p_Event);
    switch (p_Event->type()) {
        case QEvent::LanguageChange:
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
void CReagentFileView::RetranslateUI()
{
   MainMenu::CPanelFrame::SetPanelTitle(QApplication::translate("Settings::CFileViewWidget", "EventView", 0, QApplication::UnicodeUTF8));
}

/****************************************************************************/
/**
 * \brief Set the panel size of the Eventviewr window
 *
 * \iparam Width = width of the panel
 * \iparam Height = Height of the panel
 */
/****************************************************************************/
void CReagentFileView::SetPanelSize(const qint32 &Width, const qint32 &Height)
{
    mp_FileView->SetEventViewerPanelSize(Width, Height);
}

}//End of namespace Settings

