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
    /****************************************************************************/
    /**
     * \brief  Constructor.
     * \iparam p_Parent = parent of widget
     */
    /****************************************************************************/
    CViewHistoryDlg(QWidget *p_Parent = NULL);

    /****************************************************************************/
    /**
     * \brief Destructor.
     */
    /****************************************************************************/
    ~CViewHistoryDlg(void);

    /****************************************************************************/
    /*!
     *  \brief  To set Module list.
     *  \param  ModuleList = module list object
     */
    /****************************************************************************/
    void SetModuleList(ServiceDataManager::CModuleDataList *ModuleList);

public Q_SLOTS:
    /****************************************************************************/
    /*!
     *  \brief Slot for updating the GUI
     */
    /****************************************************************************/
    void UpdateGUI(void);

private:
    /****************************************************************************/
    /*!
     *  \brief  To add data item to the table
     *  \iparam p_ModuleList = pointer of Module List
     */
    /****************************************************************************/
    void AddItem(ServiceDataManager::CModuleDataList *p_ModuleList);

    Ui::CViewHistoryDlg *mp_Ui;                     //!< User interface

    QTreeView *mp_TreeView;                         //!< Tree view for module list
    QStandardItemModel *mp_Model;                   //!< Model for tree

    ServiceDataManager::CModuleDataList *mp_ModuleList;    //!< Selected modulelist
};


} // end of namespace SystemTracking

#endif // SYSTEMTRACKING_VIEWHISTORYDLG_H
