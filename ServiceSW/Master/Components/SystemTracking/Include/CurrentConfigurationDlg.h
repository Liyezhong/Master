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
#include <QStandardItemModel>
#include "ServiceDataManager/Include/Module.h"
#include "ServiceDataManager/Include/SubModule.h"
#include "SystemTracking/Include/CurrentConfigParameterDlg.h"
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
    /****************************************************************************/
    /**
     * \brief  Default constructor.
     * \iparam p_Parent = parent of widget
     */
    /****************************************************************************/
    explicit CCurrentConfigurationDlg(QWidget *p_Parent = NULL);

    /****************************************************************************/
    /**
     * \brief Destructor.
     */
    /****************************************************************************/
    ~CCurrentConfigurationDlg();

    /****************************************************************************/
    /*!
     *  \brief  To add data item to the table
     *  \iparam SubModuleName = Name of the module
     */
    /****************************************************************************/
    void AddItem(const QString& SubModuleName);

    /****************************************************************************/
    /*!
     *  \brief  To initialize the pop up dialog
     *  \iparam p_Module = Module object
     */
    /****************************************************************************/
    void InitDialog(ServiceDataManager::CModule *p_Module);

protected:
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function changeEvent
     *
     *  \param p_Event = QEvent type parameter
     *
     *  \return from changeEvent
     */
    /****************************************************************************/
    void changeEvent(QEvent *p_Event);

public slots:
    /****************************************************************************/
    /*!
    *  \brief  Pop's up a dialog for SubModule parameters.
    */
    /****************************************************************************/
    void ExecDialog();

    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of OnCancel
     */
    /****************************************************************************/
    void OnCancel();

private:
    Ui::CCurrentConfigurationDlg *mp_Ui;                         //!< Userinterface
    MainMenu::CBaseTable *mp_TableWidget;                        //!< Table for submodule list
    QStandardItemModel m_Model;                                  //!< Model for the table

    ServiceDataManager::CModule *mp_Module;                      //!< Selected module name
    ServiceDataManager::CSubModule *mp_SubModule;                //!< Selected Submodule name
    QVariant m_SubModuleName;                                    //!< Stores sub module name
    MainMenu::CMessageDlg *mp_MessageDialog;                     //!< Information message dialog
    SystemTracking::CCurrentConfigParameterDlg *mp_ParameterDlg; //!< Current Configuration parameter dialog

    QString m_strOk;
    QString m_strSelectSubModuleMsgTitle;
    QString m_strSelectSubModuleMsgText;

private slots:
    /****************************************************************************/
    /*!
     *  \brief To get the index of selected row from the table
     *  \iparam Index = Index of the row
     */
    /****************************************************************************/
     void SelectionChanged(QModelIndex Index);

     /****************************************************************************/
     /*!
      *  \brief  Definition/Declaration of function RetranslateUI
      *
      *  \return from RetranslateUI
      */
     /****************************************************************************/
     void RetranslateUI();
};

}   // end namespace SystemTracking

#endif // SYSTEMTRACKING_CURRENTCONFIGURATIONDLG_H
