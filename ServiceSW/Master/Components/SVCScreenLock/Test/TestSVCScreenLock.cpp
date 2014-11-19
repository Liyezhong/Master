/****************************************************************************/
/*! \file TestSVCScreenLock.cpp
 *
 *  \brief Unit test for SVCScreenLock menu
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
#include "SVCScreenLock/Include/SVCScreenLockWidget.h"

namespace SVCScreenLock {

/****************************************************************************/
/**
 * \brief Test class for Diagnostics class.
 */
/****************************************************************************/
class CTestSVCScreenLock : public QObject {
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
     * \brief Test Diagnostics Menu object
     */
    /****************************************************************************/
    void utTestSVCScreenLock();


}; // end class CTestSVCDiagnostics

/****************************************************************************/
void CTestSVCScreenLock::initTestCase() {
}

/****************************************************************************/
void CTestSVCScreenLock::init() {
}

/****************************************************************************/
void CTestSVCScreenLock::cleanup() {
}

/****************************************************************************/
void CTestSVCScreenLock::cleanupTestCase() {
}

/****************************************************************************/
void CTestSVCScreenLock::utTestSVCScreenLock() {
    CSVCScreenLockWidget* testWidget = new CSVCScreenLockWidget();

    testWidget->OnCurrentTabChanged(1);
    testWidget->OnCurrentTabChanged(3);
    testWidget->OnInteractStart();
}

} // end namespace SVCScreenLock

QTEST_MAIN(SVCScreenLock::CTestSVCScreenLock)
#include "TestSVCScreenLock.moc"

