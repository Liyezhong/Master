#ifndef REAGENTFILEVIEW_H
#define REAGENTFILEVIEW_H

#include "MainMenu/Include/MainWindow.h"
#include "MainMenu/Include/PanelFrame.h"
#include "MainMenu/Include/FileView.h"
#include "Global/Include/Exception.h"

namespace Reagents {
/****************************************************************************/
/**
 * \brief This class contains the Event View object
 */
/****************************************************************************/
class CReagentFileView : public MainMenu::CPanelFrame
{
    Q_OBJECT

public:
    explicit CReagentFileView(QWidget *p_Parent = 0);
    void SetPanelSize(const qint32 &Width, const qint32 &Height);
//    void DisableButton();
    ~CReagentFileView();

private:
    MainMenu::CFileView *mp_FileView;   //!< Content of this widget
    void RetranslateUI();

protected:
    void changeEvent(QEvent *p_Event);
signals:
    /****************************************************************************/
    /**
     * \brief This class contains the Event View object
     *
     * \param[in] FileName = Name of the file
     */
    /****************************************************************************/
    void SelectedLogFile(const QString &FileName);
};
}
#endif // REAGENTFILEVIEW_H
