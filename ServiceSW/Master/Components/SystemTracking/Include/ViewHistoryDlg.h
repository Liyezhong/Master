/****************************************************************************/
/*! \file ViewHistoryDlg.h
 *
 *  \brief View History for modules definition.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2013-05-10
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

#ifndef SYSTEMTRACKING_VIEWHISTORYDLG_H
#define SYSTEMTRACKING_VIEWHISTORYDLG_H

#include <QWidget>
#include "MainMenu/Include/DialogFrame.h"
#include <QTreeView>
#include <QStandardItemModel>
#include <QStandardItem>

#include "Core/Include/ServiceGUIConnector.h"
#include "DataManager/Containers/InstrumentHistory/Include/ModuleDataList.h"
#include "DataManager/Containers/InstrumentHistory/Include/Module.h"
#include "DataManager/Containers/InstrumentHistory/Include/SubModule.h"

namespace SystemTracking {

namespace Ui {
    class CViewHistoryDlg;
}

/****************************************************************************/
/**
 * \brief This dialog displays selected modulelist information
 */
/****************************************************************************/
class CViewHistoryDlg : public MainMenu::CDialogFrame
{
    Q_OBJECT

public:
    CViewHistoryDlg(QWidget *p_Parent = NULL);
    ~CViewHistoryDlg(void);

    void SetModuleListPath(const QString &FilePath);

public Q_SLOTS:
    void UpdateGUI(void);

private:
    /****************************************************************************/
    /*!
     *  \brief  To add data item to the table
     *  \iparam ModuleList = Module List object
     */
    /****************************************************************************/
    void AddItem(DataManager::CModuleDataList *p_ModuleList);

    Ui::CViewHistoryDlg *mp_Ui;                     //!< User interface

    QTreeView *mp_TreeView;                         //!< Tree view for module list
    QStandardItemModel *mp_Model;                   //!< Model for tree

    DataManager::CModuleDataList *mp_ModuleList;    //!< Selected modulelist

    QString m_ModuleListFilePath;
};


} // end of namespace SystemTracking

#endif // SYSTEMTRACKING_VIEWHISTORYDLG_H
