/****************************************************************************/
/*! \file MainControlManufacturing.cpp
 *
 *  \brief CMainControl class definition for Manufacturing SW
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2014-05-29
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

#ifndef MAINCONTROL_DIAGNOSTICSMANUFACTURING_H
#define MAINCONTROL_DIAGNOSTICSMANUFACTURING_H

#include "DiagnosticsManufacturing/Include/TestReportGeneration.h"
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
#include "Core/Include/ServiceDefines.h"

namespace DiagnosticsManufacturing {

namespace Ui {
    class CMainControlManufacturing;
}

/****************************************************************************/
/**
 * \brief This widget lists all the tests to be performed for Main Control
 */
/****************************************************************************/
class CMainControl : public QWidget
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
    explicit CMainControl(Core::CServiceGUIConnector *p_DataConnector = NULL, MainMenu::CMainWindow *p_Parent = NULL);

    /****************************************************************************/
    /*!
     *  \brief Destructor
     */
    /****************************************************************************/
    ~CMainControl();

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
    MainMenu::CMainWindow *mp_MainWindow;       //!< Main window object
    Ui::CMainControlManufacturing *mp_Ui;         //!< User Interface
    MainMenu::CBaseTable *mp_TableWidget;        //!< Base Table widget
    QStandardItemModel m_Model;                  //!< Model for the table
    ServiceDataManager::CModule* mp_Module;     //!< Module information
    QString m_LineEditString;                    //!< Stores serial number string
    QString m_MainControlSNString;               //!< Stores serial number of Main Control Moudule
    QString m_EBoxSNString;                      //!< Stores serial number of E Box sub-module
    KeyBoard::CKeyBoard *mp_KeyBoardWidget;      //!< Keyboard widget
    QHash<QString, QString> m_TestReport;        //!< Test report
    QList<QString> m_TestNames;                  //!< List of test names
    DiagnosticsManufacturing::CTestReportGeneration *mp_TestReport;     //!< Test report generation object
    QStringList m_TestResult;                                           //!< StringList stores test result
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

#endif // MAINCONTROL_DIAGNOSTICSMANUFACTURING_H
