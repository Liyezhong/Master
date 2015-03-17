/****************************************************************************/
/*! \file FirmwareUpdate.h
 *
 *  \brief CFirmwareUpdate class definition file
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2014-07-10
 *   $Author:  $ Dixiong Li
 *
 *  \b Company:
 *
 *       Leica Biosystems Shanghai.
 *
 *  (C) Copyright 2010 by Leica Biosystems Shanghai. All rights reserved.
 *  This is unpublished proprietary source code of Leica. The copyright notice
 *  does not evidence any actual or intended publication.
 *
 */
/****************************************************************************/

#ifndef SERVICEUPDATES_FIRMWAREUPDATE_H
#define SERVICEUPDATES_FIRMWAREUPDATE_H

#include <QWidget>
#include "MainMenu/Include/BaseTable.h"
#include <QStandardItemModel>
#include "Core/Include/ServiceDefines.h"
#include "Core/Include/ServiceGUIConnector.h"

namespace ServiceUpdates {

namespace Ui {
    class CFirmwareUpdate;
}

/****************************************************************************/
/**
 * \brief This widget displays history of ModuleList
 */
/****************************************************************************/
class CFirmwareUpdate : public QWidget
{
    Q_OBJECT

public:
    /****************************************************************************/
    /**
     * \brief  Default constructor.
     * \iparam p_DataConnector = service Gui connector
     * \iparam p_Parent = parent widget
     */
    /****************************************************************************/
    explicit CFirmwareUpdate(Core::CServiceGUIConnector *p_DataConnector = NULL, QWidget *p_Parent = 0);

    /****************************************************************************/
    /**
     * \brief Destructor.
     */
    /****************************************************************************/
    ~CFirmwareUpdate(void);

    /****************************************************************************/
    /*!
     *  \brief  To add data item to the table
     *  \iparam Slave = ASB name
     *  \iparam CurrentVersion = current version number
     *  \iparam LatestVersion = latest version number
     */
    /****************************************************************************/
    void AddItem(QString& Slave, QString& CurrentVersion, QString& LatestVersion);

    /****************************************************************************/
    /*!
     *  \brief  To set firmware update result on gui.
     *  \param Index = row index
     *  \param Result = update result
     */
    /****************************************************************************/
    void SetUpdateResult(int Index, bool Result);

signals:

    /****************************************************************************/
    /*!
     *  \brief Signal emitted for module test
     *  \iparam Names = string list of module names
     *  \iparam TestCaseList = list of test case id
     */
    /****************************************************************************/
    void BeginModuleTest(Service::ModuleNames_t Names, const QList<Service::ModuleTestCaseID> &TestCaseList);

    /****************************************************************************/
    /**
     * \brief Signal emitted to perform manufacturing tests
     * \iparam Test = Test name
     * \iparam AbortId = Test case abort id
     */
    /****************************************************************************/
    void PerformManufacturingTest(Service::ModuleTestCaseID Test, Service::ModuleTestCaseID AbortId=Service::TEST_CASE_ID_UNUSED);

    /****************************************************************************/
    /*!
     *  \brief Signal emitted for update module
     */
    /****************************************************************************/
    void UpdateModule(ServiceDataManager::CModule&);

protected:
    /****************************************************************************/
    /*!
     *  \brief Event handler for change events
     *
     *  \iparam p_Event = Change event
     */
    /****************************************************************************/
    void changeEvent(QEvent *p_Event);

private Q_SLOTS:
    /****************************************************************************/
    /*!
     *  \brief Slot for update firmware
     */
    /****************************************************************************/
    void UpdateFirmware(void);

    /****************************************************************************/
    /*!
     *  \brief Translates the strings in UI to the selected language
     */
    /****************************************************************************/
    void RetranslateUI();

public slots:
    /****************************************************************************/
    /*!
     *  \brief slot for Refresh latest version from firmware config file
     */
    /****************************************************************************/
    void RefreshLatestVersion();

    /****************************************************************************/
    /*!
     *  \brief Slot for updating the GUI
     */
    /****************************************************************************/
    void UpdateGUI();

    /****************************************************************************/
    /*!
     *  \brief Slot for updating slave_hw_version.xml sw_version.xml
     */
    /****************************************************************************/
    void UpdateSlaveVersion();

    /****************************************************************************/
    /*!
     *  \brief  To set firmware update enable
     *  \param Enable = flag of enable
     */
    /****************************************************************************/
    void SetEnableUpdate(bool Enable);

private:
    Ui::CFirmwareUpdate *mp_Ui;             //!< User Interface
    Core::CServiceGUIConnector *mp_DataConnector;  //!< Data Connector object
    ServiceDataManager::CModule* mp_Module;        //!< EBox Module information
    MainMenu::CBaseTable *mp_TableWidget;   //!< Base Table widget
    QStandardItemModel m_Model;             //!< Model for the table
    bool m_Result;                      //!< Firmware Update result

    /****************************************************************************/
    /*!
     *  \brief To find item by slave name
     *  \iparam Slave = slave name
     *  \result pointer of QStandardIterm
     */
    /****************************************************************************/
    QStandardItem* FindLastVersionItem(QString& Slave);

    /****************************************************************************/
    /*!
     *  \brief copy firmware files to rollback folder when update firmware success.
     */
    /****************************************************************************/
    void CopyFilesToRollBackDir();

};

} // end namespace ServiceUpdates

#endif //SERVICEUPDATES_FIRMWAREUPDATE_H
