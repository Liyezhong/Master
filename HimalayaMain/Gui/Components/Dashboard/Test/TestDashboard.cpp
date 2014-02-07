/****************************************************************************/
/*! \file TestDashboard.cpp
 *
 *  \brief Unit test for Dashboard menu
 *
 *
 *  $Version:   $ 0.1
 *  $Date:      $ 2014-1-27
 *  $Author:    $ Abe Yang
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

#include <QtTest>
#include <QDebug>
#include <Dashboard/Include/DashboardWidget.h>
#include <QObject>

namespace Dashboard {

/****************************************************************************/
/**
 * \brief Test class for Dashboard class.
 */
/****************************************************************************/
class CTestDashboard : public QObject {
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
     * \brief Test ProgramWidget object
     */
    /****************************************************************************/
    void utTestDashboard();

}; // end class TestDashboard

/****************************************************************************/
void CTestDashboard::initTestCase() {
}

/****************************************************************************/
void CTestDashboard::init() {
}

/****************************************************************************/
void CTestDashboard::cleanup() {
}

/****************************************************************************/
void CTestDashboard::cleanupTestCase() {
}

/****************************************************************************/
void CTestDashboard::utTestDashboard()
{

}

} // end namespace Dashboard

QTEST_MAIN(Dashboard::CTestDashboard)
#include "TestDashboard.moc"

