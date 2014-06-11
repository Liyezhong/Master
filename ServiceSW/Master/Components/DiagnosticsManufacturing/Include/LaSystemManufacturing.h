/****************************************************************************/
/*! \file LaSystemManufacturing.cpp
 *
 *  \brief CLaSystem class definition for Manufacturing SW
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2014-05-30
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

#ifndef LASYSTEM_DIAGNOSTICSMANUFACTURING_H
#define LASYSTEM_DIAGNOSTICSMANUFACTURING_H

#include "Core/Include/ServiceGUIConnector.h"
#include "Core/Include/ServiceDefines.h"
#include <MainMenu/Include/BaseTable.h>
#include <KeyBoard/Include/KeyBoard.h>
#include "MainMenu/Include/MessageDlg.h"
#include "MainMenu/Include/MainWindow.h"
#include <QStandardItemModel>
#include <QWidget>
#include <QPixmap>
#include <QEvent>

namespace DiagnosticsManufacturing {

namespace Ui {
    class CLaSystemManufacturing;
}

/****************************************************************************/
/**
 * \brief This widget lists all the tests to be performed for L&A System
 */
/****************************************************************************/
class CLaSystem : public QWidget
{
    Q_OBJECT

public:
    /****************************************************************************/
    /*!
     *  \brief Constructor
     *  \iparam p_DataConnector = Service GUI connector object
     *  \iparam p_Parent = Parent widget
     */
    /****************************************************************************/
    explicit CLaSystem(Core::CServiceGUIConnector *p_DataConnector = NULL, MainMenu::CMainWindow *p_Parent = NULL);

    /****************************************************************************/
    /*!
     *  \brief Destructor
     */
    /****************************************************************************/
    ~CLaSystem();

    /****************************************************************************/
    /*!
     *  \brief  Set result to test case
     *  \iparam Id = Test Case Id
     *  \iparam Result = Test result
     */
    /****************************************************************************/
    void SetTestResult(Service::ModuleTestCaseID Id, bool Result);

    /****************************************************************************/
    /*!
     *  \brief  Enable 'begin test' button
     *  \iparam EnableFlag = Enable or Disable
     */
    /****************************************************************************/
    void EnableButton(bool EnableFlag);

private:
    /****************************************************************************/
    /*!
     *  \brief  To add data item to the table
     *  \iparam Index = Test Case No.
     *  \iparam Id = Test Case Id
     */
    /****************************************************************************/
    void AddItem(quint8 Index, Service::ModuleTestCaseID_t Id);

    /****************************************************************************/
    /*!
     *  \brief  Sets the Line edit string text
     *  \iparam Value = Serial number of the module
     */
    /****************************************************************************/
    void SetLineEditText(QString Value) {
        m_LineEditString = Value;
    }

protected:
    /****************************************************************************/
    /*!
     *  \brief Filters all mouse events
     *  \iparam p_Object = Object that is watched
     *  \iparam p_Event = Current event
     *  \return True if an event should be filtered
     */
    /****************************************************************************/
    bool eventFilter(QObject *p_Object, QEvent *p_Event);

    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function changeEvent
     *  \param p_Event = QEvent type parameter
     *  \return from changeEvent
     */
    /****************************************************************************/
    void changeEvent(QEvent *p_Event);

private:
    Core::CServiceGUIConnector *mp_DataConnector;   //!< Data Connector object
    MainMenu::CMainWindow *mp_MainWindow;       //!< Main window object
    Ui::CLaSystemManufacturing *mp_Ui;          //!< User Interface
    MainMenu::CBaseTable *mp_TableWidget;       //!< Base Table widget
    QStandardItemModel m_Model;                 //!< Model for the table
    ServiceDataManager::CModule* mp_Module;     //!< Module information
    QString m_LineEditString;                   //!< Stores serial number string
    QString m_LaSNString;                       //!< Stores serial number of L&A System module
    QString m_HeaterLiquidSNString;             //!< Stores serial number of Heater with liquid sub-module
    QString m_HeaterAirSNString;                //!< Stores serial number of Heater with air sub-module
    QString m_AirSystemSNString;                //!< Stroes serial number of vacuum test sub-module
    KeyBoard::CKeyBoard *mp_KeyBoardWidget;     //!< Keyboard widget
    QHash<QString, QString> m_TestReport;       //!< Test report
    QList<QString> m_TestNames;                 //!< List of test names   
    MainMenu::CMessageDlg *mp_MessageDlg;                               //!< Information dialog
    QString m_FinalTestResult;                                          //!< Stores Final test result

    /****************************************************************************/
    /*!
     *  \brief Connects signals and slots of keyboard.
     */
    /****************************************************************************/
    void ConnectKeyBoardSignalSlots();

    /****************************************************************************/
    /*!
     *  \brief Disconnects signals and slots of keyboard.
     */
    /****************************************************************************/
    void DisconnectKeyBoardSignalSlots();

signals:
    /****************************************************************************/
    /*!
     *  \brief Signal emitted for module test
     */
    /****************************************************************************/
    void BeginModuleTest(Service::ModuleNames_t, const QList<Service::ModuleTestCaseID> &TestCaseList);

    /****************************************************************************/
    /*!
     *  \brief Signal emitted for update module
     */
    /****************************************************************************/
    void UpdateModule(ServiceDataManager::CModule&);

private slots:
    /****************************************************************************/
    /*!
     *  \brief This function is called when OK is clicked
     *  \iparam EnteredString = Stores line edit string
     */
    /****************************************************************************/
    void OnOkClicked(QString EnteredString);

    /****************************************************************************/
    /*!
     *  \brief This function hides the keyboard when Esc is clicked
     */
    /****************************************************************************/
    void OnESCClicked();

    /****************************************************************************/
    /*!
     *  \brief Connects signals and slots of keyboard.
     */
    /****************************************************************************/
    void RetranslateUI();
public slots:
    /****************************************************************************/
    /*!
     *  \brief Slot called for agitator tests
     */
    /****************************************************************************/
    void BeginTest();

    /****************************************************************************/
    /*!
     *  \brief Slot called for sending the test report to server
     */
    /****************************************************************************/
    void SendTestReport();

    /****************************************************************************/
    /*!
     *  \brief Slot called to reset the test status
     */
    /****************************************************************************/
    void ResetTestStatus();

};

} // end namespace DiagnosticsManufacturing


#endif // DIAGNOSTICSMANUFACTURING_OVEN_H
