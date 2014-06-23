/****************************************************************************/
/*! \file CleaningManufacturing.h
 *
 *  \brief CCleaning class definition for Manufacturing SW
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2014-06-23
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

#ifndef CLEANINGMANUFACTURING_H
#define CLEANINGMANUFACTURING_H

#include "Core/Include/ServiceGUIConnector.h"
#include "Core/Include/ServiceDefines.h"
#include <MainMenu/Include/BaseTable.h>
#include "MainMenu/Include/MessageDlg.h"
#include "MainMenu/Include/MainWindow.h"
#include <QStandardItemModel>
#include <QWidget>
#include <QPixmap>
#include <QEvent>

namespace DiagnosticsManufacturing {

namespace Ui {
    class CCleaningManufacturing;
}

/****************************************************************************/
/**
 * \brief This widget lists all the tests to be performed for Clean System
 */
/****************************************************************************/
class CCleaning : public QWidget
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
    explicit CCleaning(Core::CServiceGUIConnector *p_DataConnector = NULL, MainMenu::CMainWindow *p_Parent = NULL);

    /****************************************************************************/
    /*!
     *  \brief Destructor
     */
    /****************************************************************************/
    ~CCleaning();

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
     *  \brief  Definition/Declaration of function changeEvent
     *  \param p_Event = QEvent type parameter
     *  \return from changeEvent
     */
    /****************************************************************************/
    void changeEvent(QEvent *p_Event);

private:
    Core::CServiceGUIConnector *mp_DataConnector;   //!< Data Connector object
    MainMenu::CMainWindow *mp_MainWindow;       //!< Main window object
    Ui::CCleaningManufacturing *mp_Ui;            //!< User Interface
    MainMenu::CBaseTable *mp_TableWidget;       //!< Base Table widget
    QStandardItemModel m_Model;                 //!< Model for the table
    QHash<QString, QString> m_TestReport;       //!< Test report
    QList<QString> m_TestNames;                 //!< List of test names
    MainMenu::CMessageDlg *mp_MessageDlg;       //!< Information dialog
    QString m_FinalTestResult;                   //!< Stores Final test result


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

#endif // CLEANSYSTEMMANUFACTURING_H
