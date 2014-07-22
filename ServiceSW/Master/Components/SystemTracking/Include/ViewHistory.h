/****************************************************************************/
/*! \file ViewHistory.h
 *
 *  \brief View History for modules definition.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2013-01-16
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

#ifndef SYSTEMTRACKING_VIEWHISTORY_H
#define SYSTEMTRACKING_VIEWHISTORY_H

#include <QStandardItemModel>
#include <QWidget>

#include "MainMenu/Include/BaseTable.h"
#include "MainMenu/Include/MessageDlg.h"

#include "Core/Include/ServiceGUIConnector.h"

#include "SystemTracking/Include/ViewHistoryDlg.h"

namespace SystemTracking {

namespace Ui {
class CViewHistory;
}

/****************************************************************************/
/**
 * \brief This widget displays history of ModuleList
 */
/****************************************************************************/
class CViewHistory : public QWidget
{
    Q_OBJECT

public:
    /****************************************************************************/
    /**
     * \brief  Default constructor.
     * \iparam p_DataConnector = service Gui connector, parent = parent of widget
     */
    /****************************************************************************/
    explicit CViewHistory(Core::CServiceGUIConnector *p_DataConnector, QWidget *p_Parent = 0);

    /****************************************************************************/
    /**
     * \brief Destructor.
     */
    /****************************************************************************/
    ~CViewHistory(void);

    /****************************************************************************/
    /*!
     *  \brief  To add data item to the table
     *  \iparam TimeStamp = time stamp
     */
    /****************************************************************************/
    void AddItem(QString TimeStamp);

public Q_SLOTS:
    /****************************************************************************/
    /*!
     *  \brief Slot for updating the GUI
     */
    /****************************************************************************/
    void UpdateGUI(void);

    /****************************************************************************/
    /*!
     *  \brief  Pop's up a dialog for show detail.
     */
    /****************************************************************************/
    void ExecDialog(void);

    /****************************************************************************/
    /*!
     *  \brief  Pop's up a dialog for show diff.
     */
    /****************************************************************************/
    void ExecDiffDialog(void);

private Q_SLOTS:
     void SelectionChanged(QModelIndex Index);

private:
    Ui::CViewHistory *mp_Ui;                            //!< User Interface

    MainMenu::CBaseTable *mp_TableWidget;               //!< Base table for Instrument history files
    QStandardItemModel *mp_Model;                       //!< Model for the table

    SystemTracking::CViewHistoryDlg *mp_ViewHistoryDlg; //!< Dialog to view module history
    MainMenu::CMessageDlg *mp_MessageDialog;            //!< Information message dialog

    Core::CServiceGUIConnector *mp_DataConnector;                        //!< Global data connector
    ServiceDataManager::CInstrumentHistory *mp_InstrumentHistoryArchive;        //!< Instrument History Archive
    ServiceDataManager::CModuleDataList *mp_ModuleList;                         //!< Current module list object
    QModelIndexList m_SelectedRowValues;                                 //!< ModelListIndex for selected table rows
    QModelIndex m_IndexofFirst;                                          //!< First row Model Index
    QModelIndex m_IndexofLast;                                           //!< Last selected row index

};

} // end namespace SystemTracking

#endif //SYSTEMTRACKING_VIEWHISTORY_H
