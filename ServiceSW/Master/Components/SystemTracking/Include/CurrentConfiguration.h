/****************************************************************************/
/*! \file CurrentConfiguration.h
 *
 *  \brief CurrentConfiguration definition.
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

#ifndef SYSTEMTRACKING_CURRENTCONFIGURATION_H
#define SYSTEMTRACKING_CURRENTCONFIGURATION_H

#include <QWidget>
#include "MainMenu/Include/BaseTable.h"
#include <QStandardItemModel>
#include "Core/Include/ServiceGUIConnector.h"

#include "ServiceDataManager/Include/ModuleDataList.h"
//#include "ServiceDataManager/Include/Module.h"
#include "SystemTracking/Include/CurrentConfigurationDlg.h"
#include "MainMenu/Include/MessageDlg.h"

namespace SystemTracking {

namespace Ui {
class CCurrentConfiguration;
}

/****************************************************************************/
/**
 * \brief This is the widget displaying Modules.
 */
/****************************************************************************/
class CCurrentConfiguration : public QWidget
{
    Q_OBJECT

public:
    /****************************************************************************/
    /**
     * \brief  Default constructor.
     * \iparam p_DataConnector = service Gui connector, p_Parent = parent of widget
     */
    /****************************************************************************/
    explicit CCurrentConfiguration(Core::CServiceGUIConnector *p_DataConnector, QWidget *p_Parent = 0);

    /****************************************************************************/
    /**
     * \brief Destructor.
     */
    /****************************************************************************/
    ~CCurrentConfiguration();

    /****************************************************************************/
    /*!
     *  \brief  To add data item to the table
     *  \iparam ModuleName = Name of the module
     */
    /****************************************************************************/
    void AddItem(const QString &ModuleName);

public slots:
    /****************************************************************************/
    /*!
     *  \brief  Pop's up a dialog for SubModuleList.
     */
    /****************************************************************************/
    void ExecDialog(void);

    /****************************************************************************/
    /*!
     *  \brief Slot for updating the GUI
     */
    /****************************************************************************/
    void UpdateGUI(void);

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

private:
    Ui::CCurrentConfiguration *mp_Ui;                           //!< User Interface
    MainMenu::CBaseTable *mp_TableWidget;                       //!< Base table for current configuration
    QStandardItemModel m_Model;                                 //!< Model for the table
    SystemTracking::CCurrentConfigurationDlg *mp_ConfigDialog;  //!< Current configuration dialog
    Core::CServiceGUIConnector *mp_DataConnector;               //!< Global data connector
    ServiceDataManager::CModuleDataList *mp_ModuleList;         //!< Current list of modules
    ServiceDataManager::CModule *mp_Module;                     //!< Selected Module
    QVariant m_ModuleName;                                      //!< Stores Module name
    MainMenu::CMessageDlg *mp_MessageDialog;                    //!< Information message dialog

    QString m_strOk;
    QString m_strSelectModuleTitle;
    QString m_strSelectModuleText;

private slots:
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

} // end namespace SystemTracking

#endif // SYSTEMTRACKING_CURRENTCONFIGURATION_H
