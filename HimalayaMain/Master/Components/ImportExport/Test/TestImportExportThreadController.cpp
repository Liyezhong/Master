/****************************************************************************/
/*! \file TestImportExportThreadController.cpp
 *
 *  \brief Implementation file for class TestImportExportThreadController.
 *
 *  \b Description:
 *         Checks the CImportExportThreadController class implementation
 *
 *  $Version:   $ 0.1
 *  $Date:      $ 2012-11-15
 *  $Author:    $ Raju
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

#include <QTest>
#include <QFile>
#include "Global/Include/Exception.h"
#include "Global/Include/Utils.h"
#include "ImportExport/Include/ImportExportThreadController.h"


namespace ImportExport {

/****************************************************************************/
/**
 * \brief Test class for XmlConfigFileUserSettings class.
 */
/****************************************************************************/
class TestImportExportThreadController : public QObject {
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
     * \brief Test write and read Export configuration.
     */
    /****************************************************************************/
    void utTestImportExportThreadController();

}; // end class TestExportController

/****************************************************************************/
void TestImportExportThreadController::initTestCase() {

}

/****************************************************************************/
void TestImportExportThreadController::init() {
}

/****************************************************************************/
void TestImportExportThreadController::cleanup() {
}

/****************************************************************************/
void TestImportExportThreadController::cleanupTestCase() {
}

/****************************************************************************/
void TestImportExportThreadController::utTestImportExportThreadController() {

}

} // end namespace ImportExport

QTEST_MAIN(ImportExport::TestImportExportThreadController)

#include "TestImportExportThreadController.moc"
