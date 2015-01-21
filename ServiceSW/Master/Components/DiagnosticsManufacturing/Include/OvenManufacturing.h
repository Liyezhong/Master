/****************************************************************************/
/*! \file OvenManufacturing.cpp
 *
 *  \brief COven class definition for Manufacturing SW
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2014-05-12
 *   $Author:  $ Sunny Qu
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

#ifndef OVEN_DIAGNOSTICSMANUFACTURING_H
#define OVEN_DIAGNOSTICSMANUFACTURING_H

#include "DiagnosticsManufacturing/Include/TestCaseReporter.h"
#include "DiagnosticsManufacturing/Include/OvenManufacturing.h"
#include "Core/Include/ServiceGUIConnector.h"
#include "Core/Include/ServiceDefines.h"
#include <MainMenu/Include/BaseTable.h>
#include <KeyBoard/Include/KeyBoard.h>
#include "MainMenu/Include/MessageDlg.h"
#include "MainMenu/Include/MainWindow.h"
#include "MainMenu/Include/WaitDialog.h"
#include <QStandardItemModel>
#include <QWidget>
#include <QPixmap>
#include <QEvent>
#include "DiagnosticsManufacturing/Include/HeatingTestDialog.h"
#include "Core/Include/ServiceDefines.h"

namespace DiagnosticsManufacturing {

namespace Ui {
    class COvenManufacturing;
}

/****************************************************************************/
/**
 * \brief This widget lists all the tests to be performed for X-Arm
 */
/****************************************************************************/
class COven : public QWidget
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
    explicit COven(Core::CServiceGUIConnector *p_DataConnector = NULL, MainMenu::CMainWindow *p_Parent = NULL);

    /****************************************************************************/
    /*!
     *  \brief Destructor
     */
    /****************************************************************************/
    ~COven();

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
    MainMenu::CMainWindow *mp_MainWindow;           //!< Main window object
    Ui::COvenManufacturing *mp_Ui;                  //!< User Interface
    MainMenu::CBaseTable *mp_TableWidget;           //!< Base Table widget
    QStandardItemModel m_Model;                     //!< Model for the table
    ServiceDataManager::CModule* mp_Module;         //!< Module information
    KeyBoard::CKeyBoard *mp_KeyBoardWidget;         //!< Keyboard widget
    CTestCaseReporter* mp_TestReporter;             //!< Test report generation object
    MainMenu::CMessageDlg *mp_MessageDlg;           //!< Information dialog
    MainMenu::CWaitDialog *mp_WaitDlg;              //!< Waiting dialog
    QString m_OvenSNString;                         //!< Stores serial number of Oven module
    QString m_FinalTestResult;                      //!< Stores Final test result
    bool m_TestFlag;                                //!< ture if have test case finished

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
     *  \iparam Names = name of module
     *  \iparam TestCaseList = list of test case id
     */
    /****************************************************************************/
    void BeginModuleTest(Service::ModuleNames_t Names, const QList<Service::ModuleTestCaseID> &TestCaseList);

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
    void OnOkClicked(const QString& EnteredString);

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
