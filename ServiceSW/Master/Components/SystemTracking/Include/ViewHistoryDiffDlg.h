/****************************************************************************/
/*! \file ViewHistoryDiffDlg.h
 *
 *  \brief View History Difference dialog for modulelists.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2013-06-18
 *   $Author:  $ Soumya. D
 *
 *  \b Company:
 *
 *       Leica Biosystems R&D Center Shanghai.
 *
 *  (C) Copyright 2010 by Leica Biosystems R&D Center Shanghai. All rights reserved.
 *  This is unpublished proprietary source code of Leica. The copyright notice
 *  does not evidence any actual or intended publication.
 *
 */
/****************************************************************************/
#ifndef SYSTEMTRACKING_VIEWHISTORYDIFFDLG_H
#define SYSTEMTRACKING_VIEWHISTORYDIFFDLG_H

//#include "Core/Include/ServiceDialogFrame.h"
#include "MainMenu/Include/DialogFrame.h"
#include "MainMenu/Include/MessageDlg.h"
#include <QStandardItemModel>
#include <QTreeView>

namespace ServiceDataManager {
    class CModuleDataList;
}

namespace SystemTracking {

namespace Ui {
    class CViewHistoryDiffDlg;
}

/****************************************************************************/
/**
 * \brief This widget displays difference between two module timestamps
 */
/****************************************************************************/
class CViewHistoryDiffDlg : public MainMenu::CDialogFrame
{
    Q_OBJECT

public:
    /****************************************************************************/
    /**
     * \brief Get reference to instance.
     * \iparam p_Parent = parent of widget
     * \return  Reference to instance.
     */
    /****************************************************************************/
    static CViewHistoryDiffDlg& Instance(QWidget *p_Parent) {
        static CViewHistoryDiffDlg m_ViewHistoryDiffDlg(p_Parent);
        return m_ViewHistoryDiffDlg;
    }

    /****************************************************************************/
    /**
     * \brief To Show dialog diff two modulelists.
     * \param ModuleListOne = first select Modulelist
     * \param ModuleListTwo = second select ModuleList
     */
    /****************************************************************************/
    void Show(ServiceDataManager::CModuleDataList& ModuleListOne, ServiceDataManager::CModuleDataList& ModuleListTwo);

protected:
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function changeEvent
     *  \param p_Event = QEvent type parameter
     *  \return from changeEvent
     */
    /****************************************************************************/
    void changeEvent(QEvent *p_Event);

private:
    /****************************************************************************/
    /**
     * \brief  Constructor.
     * \iparam p_Parent = parent of widget
     */
    /****************************************************************************/
    CViewHistoryDiffDlg(QWidget *p_Parent = NULL);

    /****************************************************************************/
    /**
     * \brief Destructor.
     */
    /****************************************************************************/
    virtual ~CViewHistoryDiffDlg();

    Q_DISABLE_COPY(CViewHistoryDiffDlg)

    /****************************************************************************/
    /*!
     *  \brief  To add data item to the tree
     *  \iparam ModuleList = Module List object
     *  \return Returns tree item
     */
    /****************************************************************************/
    QStandardItem* AddModuleInfo(ServiceDataManager::CModuleDataList& ModuleList);

    /****************************************************************************/
    /*!
     *  \brief  To add data item to the table
     *  \iparam ModuleList = Module List object
     */
    /****************************************************************************/
    void AddItem(ServiceDataManager::CModuleDataList &ModuleList);

    /****************************************************************************/
    /*!
      *  \brief  To add data item to the table
      *  \iparam ModuleList = Module List object
      */
    /****************************************************************************/
    void AddItemToTree(ServiceDataManager::CModuleDataList &ModuleList);

    /****************************************************************************/
    /*!
      *  \brief  To set Tree view properties
      */
    /****************************************************************************/
    void SetTreeProperties();

    Ui::CViewHistoryDiffDlg *mp_Ui;                                      //!< User Interface    
    QTreeView *mp_TreeView;                                              //!< Tree view for module list
    QStandardItemModel m_Model;                                          //!< Model for tree
    QTreeView *mp_TreeViewRight;                                         //!< Tree view for module list
    QStandardItemModel m_ModelRight;                                     //!< Model for tree
    MainMenu::CMessageDlg *mp_MessageDlg;                                //!< Information dialog

private slots:
    void RetranslateUI();
    void CloseDialog();

};

} // end of namespace SystemTracking

#endif // SYSTEMTRACKING_VIEWHISTORYDIFFDLG_H
