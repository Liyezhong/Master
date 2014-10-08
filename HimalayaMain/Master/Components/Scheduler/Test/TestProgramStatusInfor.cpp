/****************************************************************************/
/*! \file TestUserSettings.cpp
 *
 *  \brief Implementation file for class TestProgramStatusInfor.
 *
 *  $Version:   $ 0.1
 *  $Date:      $ 2014-09-23
 *  $Author:    $ Logan Yang
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
#include "Scheduler/Include/ProgramStatusInfor.h"


namespace Scheduler {


/****************************************************************************/
/**
 * \brief Test class for TestProgramStatusInfor class.
 */
/****************************************************************************/
class TestProgramStatusInfor : public QObject {
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
     * \brief Test the ProgramStatusInfor.
     */
    /****************************************************************************/
    void utTestProgramStatusInfor();
}; // end class TestUserSettings

/****************************************************************************/
void TestProgramStatusInfor::initTestCase() {
    // init languages
    //Global::InitSupportedLanguages();
}

/****************************************************************************/
void TestProgramStatusInfor::init() {
}

/****************************************************************************/
void TestProgramStatusInfor::cleanup() {
}

/****************************************************************************/
void TestProgramStatusInfor::cleanupTestCase() {
}

/****************************************************************************/
void TestProgramStatusInfor::utTestProgramStatusInfor() {
    CProgramStatusInfor ProStatus;
    bool result = false;

    result = ProStatus.ReadProgramStatusFile();
    QVERIFY(result);
    ProStatus.SetLastRVPosition(DeviceControl::RV_SEAL_10);
    ProStatus.SetProgramID("L01");
    ProStatus.SetStepID(3);
    ProStatus.SetScenario(4);
    QCOMPARE(ProStatus.GetLastRVPosition(),DeviceControl::RV_SEAL_10);
    QCOMPARE(ProStatus.GetProgramId(),QString("L01"));
    QVERIFY(ProStatus.GetStepID()==3);
    QVERIFY(ProStatus.GetScenario() == 4);

    ProStatus.SetProgramID("");
    QVERIFY(ProStatus.IsProgramFinished());

    quint64 OneHour = 60 * 60 * 1000;
    quint64 TimeLimit = 12 * OneHour;
    quint64 HeatingTime = ProStatus.GetOvenHeatingTime(54);
    QVERIFY(HeatingTime == TimeLimit/1000);

    quint64 time = QDateTime::currentMSecsSinceEpoch();
    ProStatus.UpdateOvenHeatingTime(time - 15 * OneHour,true,true);
    sleep(70);
    ProStatus.UpdateOvenHeatingTime(time - 13 * OneHour,true);
    HeatingTime = ProStatus.GetOvenHeatingTime(54);
    QVERIFY(HeatingTime == TimeLimit/1000);

    ProStatus.UpdateOvenHeatingTime(time - 1 * OneHour,true,true);
    sleep(70);
    ProStatus.UpdateOvenHeatingTime(time,true);
    HeatingTime = ProStatus.GetOvenHeatingTime(54);
    QVERIFY(HeatingTime == TimeLimit/1000);
}



} // end namespace DataManagement

QTEST_MAIN(Scheduler::TestProgramStatusInfor)

#include "TestProgramStatusInfor.moc"
