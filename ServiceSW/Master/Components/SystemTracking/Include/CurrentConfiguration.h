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

#include "DataManager/Containers/InstrumentHistory/Include/ModuleDataList.h"
#include "DataManager/Containers/InstrumentHistory/Include/Module.h"
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
    explicit CCurrentConfiguration(Core::CServiceGUIConnector *p_DataConnector, QWidget *p_Parent = 0);
    ~CCurrentConfiguration();

    /****************************************************************************/
    /*!
     *  \brief  To add data item to the table
     *  \iparam ModuleName = Name of the module
     */
    /****************************************************************************/
    void AddItem(const QString &ModuleName);

public slots:
    void ExecDialog(void);

    void UpdateGUI(void);

private:
    Ui::CCurrentConfiguration *mp_Ui;                           //!< User Interface
    MainMenu::CBaseTable *mp_TableWidget;                       //!< Base table for current configuration
    QStandardItemModel m_Model;                                 //!< Model for the table
    SystemTracking::CCurrentConfigurationDlg *mp_ConfigDialog;  //!< Current configuration dialog
    Core::CServiceGUIConnector *mp_DataConnector;               //!< Global data connector
    DataManager::CModuleDataList *mp_ModuleList;                //!< Current list of modules
    DataManager::CModule *mp_Module;                            //!< Selected Module
    QVariant m_ModuleName;                                      //!< Stores Module name
    MainMenu::CMessageDlg *mp_MessageDialog;                    //!< Information message dialog

private slots:
     void SelectionChanged(QModelIndex Index);
};

} // end namespace SystemTracking

#endif // SYSTEMTRACKING_CURRENTCONFIGURATION_H
