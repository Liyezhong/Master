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
    MainMenu::CMainWindow *mp_MainWindow;           //!< Pointer to MainWindow
    Ui::CDataManagementWidget *mp_Ui;
    bool m_ProcessRunning;                          //!< Process running state
    MainMenu::CMainWindow::UserRole_t m_CurrentUserRole;    //! < Current user role

    void RetranslateUI();
    void ResetButtons();

protected:
    void changeEvent(QEvent *p_Event);
    void showEvent(QShowEvent *);


private slots:
    void ServiceExportDialog();
    void SaveToUSBDialog();
    void ImportDialog();
    void SWUpdate();
    void RemoteSWUpdate();

signals:

    void ExecSending(const QString, const QStringList &);
    void EmitSWUpdate(bool);
};

} //end of namespace
#endif // DATAMANAGEMENTWIDGET_H
