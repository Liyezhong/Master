/****************************************************************************/
/*! \file TestServiceUpdates.cpp
 *
 *  \brief Unit test for ServiceUpdates menu
 *
 *
 *  $Version:   $ 0.1
 *  $Date:      $ 2013-08-06
 *  $Author:    $ Soumya. D
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

#include <QtTest/QTest>
#include <QDebug>
#include <ServiceUpdates/Include/Settings.h>
#include <Settings/Include/FirmwareInformation.h>
#include <Settings/Include/DataManagement.h>
//#include <ServiceUpdates/Include/RFIDValues.h>
#include <QObject>

namespace ServiceUpdates {

/****************************************************************************/
/**
 * \brief Test class for ServiceUpdates class.
 */
/****************************************************************************/
class CTestServiceUpdates : public QObject {
    Q_OBJECT
private slots:
    /****************************************************************************/
    /**
     * \brief Called before the first testfunction is executed.
     */
    /****************************************************************************/
    void initTestCase();
    /****************************************************************************/
    /**
     * \brief Called before each testfunction is executed.
     */
    /****************************************************************************/
    void init();
    /****************************************************************************/
    /**
     * \brief Called after each testfunction was executed.
     */
    /****************************************************************************/
    void cleanup();
    /****************************************************************************/
    /**
     * \brief Called after last testfunction was executed.
     */
    /****************************************************************************/
    void cleanupTestCase();

    /****************************************************************************/
    /**
     * \brief Test ServiceUpdates Menu object
     */
    /****************************************************************************/
    void utTestServiceUpdates();

}; // end class CTestServiceUpdates

/****************************************************************************/
void CTestServiceUpdates::initTestCase() {
}

/****************************************************************************/
void CTestServiceUpdates::init() {
}

/****************************************************************************/
void CTestServiceUpdates::cleanup() {
}

/****************************************************************************/
void CTestServiceUpdates::cleanupTestCase() {
}

/****************************************************************************/
void CTestServiceUpdates::utTestServiceUpdates() {
//    MainMenu::CMainWindow *p_MainWindow = new MainMenu::CMainWindow();
//    Core::CServiceGUIConnector *p_ServiceGUIConnector = new
//            Core::CServiceGUIConnector(p_MainWindow);

//    ServiceUpdates::CSettings *p_Settings = new
//            ServiceUpdates::CSettings(p_ServiceGUIConnector, p_MainWindow);
//    p_Settings->UpdateGUIConnector(p_ServiceGUIConnector, p_MainWindow);
//    p_Settings->SaveProxyIPAddress("221.114.112.156");

}

} // end namespace ServiceUpdates

QTEST_MAIN(ServiceUpdates::CTestServiceUpdates)
#include "TestServiceUpdates.moc"

