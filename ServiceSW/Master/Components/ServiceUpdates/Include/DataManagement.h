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
 * \brief This widget displays Import&Export
 */
/****************************************************************************/
class CDataManagement : public QWidget
{
    Q_OBJECT

public:
    /****************************************************************************/
    /**
     * \brief  Default constructor.
     * \iparam p_Parent = parent of widget
     */
    /****************************************************************************/
    explicit CDataManagement(QWidget *p_Parent = 0);

    /****************************************************************************/
    /**
     * \brief Destructor.
     */
    /****************************************************************************/
    ~CDataManagement(void);

Q_SIGNALS:
    /****************************************************************************/
    /**
     * \brief Signal emit for import&export request.
     */
    /****************************************************************************/
    void ServiceImportExportRequested(bool IsImport);

public Q_SLOTS:
    /****************************************************************************/
    /**
     * \brief Slot for export finished
     * \iparam Failed = export result.
     */
    /****************************************************************************/
    void ExportFinished(bool Failed);

    /****************************************************************************/
    /**
     * \brief Slot for import finished
     *  \iparam Failed = import result.
     */
    /****************************************************************************/
    void ImportFinished(bool Failed);

private Q_SLOTS:
    /****************************************************************************/
    /**
     * \brief Slot for export.
     */
    /****************************************************************************/
    void Export(void);

    /****************************************************************************/
    /**
     * \brief Slot for import.
     */
    /****************************************************************************/
    void Import(void);

private:
    Ui::CDataManagement   *mp_Ui;       //!< user interface

    MainMenu::CWaitDialog *mp_WaitDialog;   //!< wait dialog

    MainMenu::CMessageDlg *mp_MessageDlg;   //!< message dialog

};

} // end namespace ServiceUpdates

#endif //SERVICEUPDATES_DATAMANAGEMENT_H
