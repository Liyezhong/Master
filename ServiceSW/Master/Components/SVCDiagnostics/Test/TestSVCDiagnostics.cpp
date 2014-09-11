/****************************************************************************/
/*! \file TestSVCDiagnostics.cpp
 *
 *  \brief Unit test for SVCDiagnostics menu
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
#include <MainMenu/Include/MainWindow.h>
#include <QObject>
#include <QMessageBox>
#include <QMainWindow>

namespace SVCDiagnostics {

/****************************************************************************/
/**
 * \brief Test class for Diagnostics class.
 */
/****************************************************************************/
class CTestSVCDiagnostics : public QObject {
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
    void utTestSVCDiagnostics();


}; // end class CTestSVCDiagnostics

/****************************************************************************/
void CTestSVCDiagnostics::initTestCase() {
}

/****************************************************************************/
void CTestSVCDiagnostics::init() {
}

/****************************************************************************/
void CTestSVCDiagnostics::cleanup() {
}

/****************************************************************************/
void CTestSVCDiagnostics::cleanupTestCase() {
}

/****************************************************************************/
void CTestSVCDiagnostics::utTestSVCDiagnostics() {
}

} // end namespace SVCDiagnostics

QTEST_MAIN(SVCDiagnostics::CTestSVCDiagnostics)
#include "TestSVCDiagnostics.moc"

