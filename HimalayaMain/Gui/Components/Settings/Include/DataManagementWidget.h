/****************************************************************************/
/*! \file DataManagementWidget.h
 *
 *  \brief DataManagementWidget definition.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2011-09-29
 *   $Author:  $ M.Scherer
 *
 *  \b Company:
 *
 *       Leica Biosystems Nussloch GmbH.
 *
 *  (C) Copyright 2010 by Leica Biosystems Nussloch GmbH. All rights reserved.
 *  This is unpublished proprietary source code of Leica. The copyright notice
 *  does not evidence any actual or intended publication.
 *
 */
/****************************************************************************/

#ifndef DATAMANAGEMENTWIDGET_H
#define DATAMANAGEMENTWIDGET_H

#include "MainMenu/Include/MainWindow.h"
#include "MainMenu/Include/DataManagement.h"
#include "MainMenu/Include/PanelFrame.h"

namespace Settings {

namespace Ui {
    class CDataManagementWidget;
}

/****************************************************************************/
/**
 * \brief Frame for the data import / export widget
 */
/****************************************************************************/
class CDataManagementWidget : public MainMenu::CPanelFrame
{
    Q_OBJECT

public:
    explicit CDataManagementWidget(QWidget *p_Parent = 0);
    ~CDataManagementWidget();
    void SetPtrToMainWindow(MainMenu::CMainWindow *p_MainWindow);

private:
    MainMenu::CDataManagement *mp_DataManagement;   //!< Content of this widget
    MainMenu::CMainWindow *mp_MainWindow;           //!< Pointer to MainWindow
    Ui::CDataManagementWidget *mp_Ui;
    void RetranslateUI();

protected:
    void changeEvent(QEvent *p_Event);

private slots:
    void ExportDialog();
    void SaveUSBDialog();
    void ImportDialog();

signals:

    void ExecSending(const QString, const QStringList &);

};

} //end of namespace
#endif // DATAMANAGEMENTWIDGET_H
