/****************************************************************************/
/*! \file CurrentConfigurationDlg.h
 *
 *  \brief CurrentConfiguration dialog definition.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2013-05-02
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

#ifndef SYSTEMTRACKING_CURRENTCONFIGURATIONDLG_H
#define SYSTEMTRACKING_CURRENTCONFIGURATIONDLG_H

#include "MainMenu/Include/DialogFrame.h"
#include "MainMenu/Include/BaseTable.h"
#include "MainMenu/Include/TextDialog.h"
#include <QStandardItemModel>
#include <DataManager/Containers/InstrumentHistory/Include/Module.h>
#include <DataManager/Containers/InstrumentHistory/Include/SubModule.h>
#include "MainMenu/Include/MessageDlg.h"

namespace SystemTracking {

namespace Ui {
class CCurrentConfigurationDlg;
}

/****************************************************************************/
/**
 * \brief This is the dialog for displaying module information.
 */
/****************************************************************************/
class CCurrentConfigurationDlg : public MainMenu::CDialogFrame
{

    Q_OBJECT

public:
    explicit CCurrentConfigurationDlg(QWidget *p_Parent = NULL);
    ~CCurrentConfigurationDlg();

    /****************************************************************************/
    /*!
     *  \brief  To add data item to the table
     *  \iparam SubModuleName = Name of the module
     */
    /****************************************************************************/
    void AddItem(QString SubModuleName);

    /****************************************************************************/
    /*!
     *  \brief  To initialize the pop up dialog
     *  \iparam Module = Module object
     */
    /****************************************************************************/
    void InitDialog(DataManager::CModule *p_Module);


public slots:
    void ExecDialog();

private:
    Ui::CCurrentConfigurationDlg *mp_Ui;        //!< Userinterface
    MainMenu::CBaseTable *mp_TableWidget;       //!< Table for submodule list
    MainMenu::CTextDialog *mp_ExecDialog;       //!< Text dialog for submodule information
    QStandardItemModel m_Model;                 //!< Model for the table

    DataManager::CModule *mp_Module;            //!< Selected module name
    DataManager::CSubModule *mp_SubModule;      //!< Selected Submodule name
    QVariant m_ModuleName;                      //!< Stores module name
    MainMenu::CMessageDlg *mp_MessageDialog;    //!< Information message dialog

private slots:
     void SelectionChanged(QModelIndex Index);
};

}   // end namespace SystemTracking

#endif // SYSTEMTRACKING_CURRENTCONFIGURATIONDLG_H
