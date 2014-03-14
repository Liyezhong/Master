/****************************************************************************/
/*! \file DataManagement.h
 *
 *  \brief
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2013-05-08
 *   $Author:  $ Soumya. D
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

#ifndef SERVICEUPDATES_DATAMANAGEMENT_H
#define SERVICEUPDATES_DATAMANAGEMENT_H

#include <QWidget>

#include "MainMenu/Include/WaitDialog.h"
#include "MainMenu/Include/MessageDlg.h"


namespace ServiceUpdates {

namespace Ui {
    class CDataManagement;
}
/****************************************************************************/
/**
 * \brief
 */
/****************************************************************************/
class CDataManagement : public QWidget
{
    Q_OBJECT

public:
    explicit CDataManagement(QWidget *p_Parent = 0);
    ~CDataManagement(void);

Q_SIGNALS:
    void ServiceImportExportRequested(bool IsImport);

public Q_SLOTS:
    void ExportFinished(bool Failed);

    void ImportFinished(bool Failed);

private Q_SLOTS:
    void Export(void);

    void Import(void);

private:
    Ui::CDataManagement   *mp_Ui;

    MainMenu::CWaitDialog *mp_WaitDialog;

    MainMenu::CMessageDlg *mp_MessageDlg;

};

} // end namespace ServiceUpdates

#endif //SERVICEUPDATES_DATAMANAGEMENT_H
