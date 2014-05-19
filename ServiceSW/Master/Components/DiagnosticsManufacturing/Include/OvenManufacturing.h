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

#ifndef DIAGNOSTICSMANUFACTURING_OVEN_H
#define DIAGNOSTICSMANUFACTURING_OVEN_H

#include "DiagnosticsManufacturing/Include/TestReportGeneration.h"
#include "DiagnosticsManufacturing/Include/OvenManufacturing.h"
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
    explicit COven(Core::CServiceGUIConnector *p_DataConnector = NULL, MainMenu::CMainWindow *p_Parent = NULL);
    ~COven();

    /****************************************************************************/
    /*!
     *  \brief  To add data item to the table
     *  \iparam SerialNumber = Serial number of the module
     *  \iparam Test = Test name
     */
    /****************************************************************************/
    void AddItem(QString SerialNumber, QString Test);

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
    bool eventFilter(QObject *p_Object, QEvent *p_Event);
    void changeEvent(QEvent *p_Event);

private:
    Core::CServiceGUIConnector *mp_DataConnector;   //!< Data Connector object
    MainMenu::CMainWindow *mp_MainWindow;       //!< Main window object
    Ui::COvenManufacturing *mp_Ui;         //!< User Interface
    MainMenu::CBaseTable *mp_TableWidget;       //!< Base Table widget
    QStandardItemModel m_Model;                 //!< Model for the table
    QString m_LineEditString;                   //!< Stores serial number string
    QString m_OvenSNString;                     //!< Stores serial number of Oven module
    QString m_HeaterSNString;                   //!< Stores serial number of Heater sub-module
    QString m_CoverSensorSNString;              //!< Stores serial number of Cover sensor sub-module
    KeyBoard::CKeyBoard *mp_KeyBoardWidget;     //!< Keyboard widget
    QHash<QString, QString> m_TestReport;       //!< Test report
    QList<QString> m_TestNames;                 //!< List of test names   
    DiagnosticsManufacturing::CTestReportGeneration *mp_TestReport;     //!< Test report generation object
    QStringList m_TestResult;                                           //!< StringList stores test result
    MainMenu::CMessageDlg *mp_MessageDlg;                               //!< Information dialog
    QString m_FinalTestResult;                                          //!< Stores Final test result


    void ConnectKeyBoardSignalSlots();
    void DisconnectKeyBoardSignalSlots();

signals:
    /****************************************************************************/
    /*!
     *  \brief Signal emitted for module test
     */
    /****************************************************************************/
    void BeginModuleTest(Service::ModuleNames_t);

private slots:
    void OnOkClicked(QString EnteredString);
    void OnESCClicked();
    void RetranslateUI();

public slots:
    void BeginTest();
    void SetTestResult(Service::ModuleTestNames TestName, bool Result);
    void SendTestReport();
    void ResetTestStatus();

};

} // end namespace DiagnosticsManufacturing


#endif // DIAGNOSTICSMANUFACTURING_OVEN_H
