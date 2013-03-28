/****************************************************************************/
/*! \file TestDataStationList.cpp
 *
 *  \brief Implementation file for class TestDataCReagent.
 *
 *  $Version:   $ 0.1
 *  $Date:      $ 2012-05-02
 *  $Author:    $ Vikram MK
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
#include <ColoradoDataContainer/Containers/Stations/Include/DataStationList.h>
#include <ColoradoDataContainer/Containers/Stations/Include/DataStationListVerifier.h>

namespace DataManager {

/****************************************************************************/
/**
 * \brief Test class for CStation class.
 */
/****************************************************************************/
class TestDataStationList : public QObject {
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
     * \brief Test data CStation.
     */
    /****************************************************************************/
    void utTestDataStation();
    /****************************************************************************/
    /**
     * \brief Test data CDataStationList.
     */
    /****************************************************************************/
    void utTestDataStationList();
    /****************************************************************************/
    /**
     * \brief Test data CDataStationListVerifier.
     */
    /****************************************************************************/
    void utTestDataStationListVerifier();
}; // end class TestDataStationList

/****************************************************************************/
void TestDataStationList::initTestCase() {
}

/****************************************************************************/
void TestDataStationList::init() {
}

/****************************************************************************/
void TestDataStationList::cleanup() {
}

/****************************************************************************/
void TestDataStationList::cleanupTestCase() {
}

/****************************************************************************/
void TestDataStationList::utTestDataStation() {

    CStation *p_Station1 = new CStation();
    CStation *p_Station2 = new CStation();

    p_Station1->SetStationID("R01"); // Reagent
    p_Station1->SetReagentID("L2");
    p_Station1->SetExpiryDate("2012-05-05T23:59:03");
    p_Station1->SetLotNumber("45");
    p_Station1->SetNumberOfActualSlides(2500);
    p_Station1->SetProgramID("P36");
    p_Station1->SetStepID("5");

    QCOMPARE(p_Station1->GetStationID(), QString("R01"));
    QCOMPARE(p_Station1->GetReagentID(), QString("L2"));
    qDebug() << "the expiry date:" << p_Station1->GetExpiryDateInString();
    QCOMPARE(p_Station1->GetExpiryDateInString(), QString("2012-05-05T23:59:03"));
    QCOMPARE(p_Station1->GetLotNumber(), QString("45"));
    QCOMPARE(p_Station1->GetNumberOfActualSlides(), 2500);
    QCOMPARE(p_Station1->GetProgramID(), QString("P36"));
    QCOMPARE(p_Station1->GetStepID(), QString("5"));
    // check station expiry date method
    p_Station1->SetExpiryDate("2012-05-08T23:56:08");
    QCOMPARE(p_Station1->IsStationExpired(), true);

    p_Station1->SetExpiryDate("2012-12-31T23:56:08");
    QCOMPARE(p_Station1->IsStationExpired(), false);

    // check station actual slides expiry
    p_Station1->SetNumberOfActualSlides(3500);
    QCOMPARE(p_Station1->IsStationExpired(), true);

    p_Station2->SetStationID("H01"); // Heated Cuvette
    p_Station2->SetReagentID("U10");
    p_Station2->SetNumberOfActualSlides(1500);
    p_Station2->SetExpiryDate("2012-12-31T23:56:08");
    p_Station2->SetProgramID("-1");
    p_Station2->SetStepID("-1");
    QCOMPARE(p_Station2->GetStationType(), HEATED);

    CStation Station3(*p_Station2);

    QCOMPARE(Station3.GetStationID(), QString("H01"));
    QCOMPARE(Station3.GetStationType(), HEATED);
    QCOMPARE(Station3.GetReagentID(), QString("U10"));
    QCOMPARE(Station3.GetNumberOfActualSlides(), 1500);
    QCOMPARE(Station3.GetProgramID(), QString("-1"));
    QCOMPARE(Station3.GetStepID(), QString("-1"));

    Station3.SetStationID("");
    QCOMPARE(Station3.GetStationType(), INVALID_TYPE);

    CStation Station4;
    Station4 = *p_Station2;
    QCOMPARE(Station4.GetStationID(), QString("H01"));
    QCOMPARE(Station4.GetStationType(), HEATED);
    QCOMPARE(Station4.GetReagentID(), QString("U10"));
    QCOMPARE(Station4.GetNumberOfActualSlides(), 1500);
    QCOMPARE(Station4.GetProgramID(), QString("-1"));
    QCOMPARE(Station4.GetStepID(), QString("-1"));

    Station4.SetStationID("");
    QCOMPARE(Station4.GetStationType(), INVALID_TYPE);


}
/****************************************************************************/
void TestDataStationList::utTestDataStationList() {

    CStation *p_Station3 = new CStation();
    CStation *p_Station4 = new CStation();
    CDataStationList *p_StationList = new CDataStationList();
    IVerifierInterface *p_DataStationListVerifier;
    p_DataStationListVerifier = new CDataStationListVerifier();
    p_StationList->AddVerifier(p_DataStationListVerifier);

    p_StationList->SetDataVerificationMode(false);
    // Checking the Read of CDataStationList
    QCOMPARE(p_StationList->Read(":/Xml/Stations.xml"), true);
    p_StationList->SetDataVerificationMode(true);

    // Checking the basic interfaces of CDataStationList
    QCOMPARE(p_StationList->GetNumberOfStations(), 18);
    QCOMPARE(p_StationList->GetVersion(), 1);
    QCOMPARE(p_StationList->StationExists("H01"), true);
    QCOMPARE(p_StationList->StationExists("R31"), true);
    QCOMPARE(p_StationList->StationExists("L09"), false);

    // Checking for parallel Station method
    QCOMPARE(p_StationList->IsParallelStation("R24"), true);
    QCOMPARE(p_StationList->IsParallelStation("R26"), true);
    QCOMPARE(p_StationList->IsParallelStation("R31"), false);
    QCOMPARE(p_StationList->IsParallelStation("R20"), false);

    //Checking the get Station
    QCOMPARE(p_StationList->GetStation(3, *p_Station3) , true);
    QCOMPARE(p_StationList->GetStation("R31", *p_Station4) , true);

    p_Station4->SetProgramID("34");
    p_Station4->SetStepID("2");

    // check for update
    QVERIFY(p_StationList->UpdateStation(p_Station4));
    QCOMPARE(p_StationList->StationExists(p_Station4->GetStationID()), true);
    QCOMPARE(p_StationList->IsParallelStation("R31"), true);

    //check for GetPossibleStations method
    qDebug() << "the list of stations" << p_StationList->GetPossibleStations("L1","37","1");

    // This file would be created in build folder particular to the Target (bin_dlg)
    QCOMPARE(p_StationList->Write("Stations_Test.xml"), true);

    // Check the copy constructor of CDataStationList
    CDataStationList StationList1(*p_StationList);

    CStation *p_Station5 = new CStation();
    QCOMPARE(StationList1.GetStation(5, *p_Station5) , true);

    // check the write method
    StationList1.SetDataVerificationMode(false);
    QCOMPARE(StationList1.Read("Stations_Test.xml"), true);
    StationList1.SetDataVerificationMode(true);

    p_Station5->SetExpiryDate("2080-12-31T23:56:08");
    p_Station5->SetNumberOfActualSlides(2500);
    QVERIFY(StationList1.UpdateStation(p_Station5));

    // check for Delete Station method
    QCOMPARE(StationList1.DeleteStation(17), true);
    QCOMPARE(StationList1.StationExists("R20"), false);
    QCOMPARE(StationList1.GetNumberOfStations(), 17);

    QCOMPARE(StationList1.DeleteStation("R03"), true);
    QCOMPARE(StationList1.StationExists("R03"), false);
    QCOMPARE(StationList1.GetNumberOfStations(), 16);

    //Check Assignment operator
    CDataStationList AssignmentTestStationList;
    AssignmentTestStationList = *p_StationList;
    CStation *p_Station6 = new CStation();
    QCOMPARE(AssignmentTestStationList.GetStation(5, *p_Station6) , true);

    // check the write method
    AssignmentTestStationList.SetDataVerificationMode(false);
    QCOMPARE(AssignmentTestStationList.Read("Stations_Test.xml"), true);
    AssignmentTestStationList.SetDataVerificationMode(true);

    p_Station6->SetExpiryDate("2080-12-31T23:56:08");
    p_Station6->SetNumberOfActualSlides(2500);
    QVERIFY(AssignmentTestStationList.UpdateStation(p_Station6));

    // check for Delete Station method
    QCOMPARE(AssignmentTestStationList.DeleteStation(17), true);
    QCOMPARE(AssignmentTestStationList.StationExists("R20"), false);
    QCOMPARE(AssignmentTestStationList.GetNumberOfStations(), 17);

    QCOMPARE(AssignmentTestStationList.DeleteStation("R03"), true);
    QCOMPARE(AssignmentTestStationList.StationExists("R03"), false);
    QCOMPARE(AssignmentTestStationList.GetNumberOfStations(), 16);

    // check the write method without file name
    QCOMPARE(AssignmentTestStationList.Write(), true);
}
/****************************************************************************/
void TestDataStationList::utTestDataStationListVerifier() {

    CDataStationList StationList2;
    IVerifierInterface *p_DataStationListVerifier1;
    p_DataStationListVerifier1 = new CDataStationListVerifier();
    StationList2.AddVerifier(p_DataStationListVerifier1);

    StationList2.SetDataVerificationMode(false);
    QCOMPARE(StationList2.Read("Stations_Test.xml"), true);
    StationList2.SetDataVerificationMode(true);

    CStation *p_Station7 = new CStation();
    CStation *p_Station8 = new CStation();

    QCOMPARE(StationList2.GetStation("R18", *p_Station7) , true);
    QCOMPARE(StationList2.GetStation("R33", *p_Station8) , true);

    // check for maximum number of slides
//    p_Station8->SetNumberOfActualSlides(4000);
//    QCOMPARE(StationList2.UpdateStation(p_Station8), false);
//    CStation *p_Station9 = new CStation();
//    QCOMPARE(StationList2.GetStation("R33", *p_Station9) , true);
//    QCOMPARE(p_Station9->GetNumberOfActualSlides(), 1000);

    //Check for expiry date
    p_Station7->SetExpiryDate("2009-14-35");
    QCOMPARE(StationList2.UpdateStation(p_Station7), false);
    QCOMPARE(StationList2.StationExists("R18"), true);
    p_Station7->SetExpiryDate("2090-1-20");
    QCOMPARE(StationList2.UpdateStation(p_Station7), true);

    //check for station ID
    p_Station7->SetStationID("RR56");
    QCOMPARE(StationList2.UpdateStation(p_Station7), false);
    QCOMPARE(StationList2.StationExists("RR56"), false);

    // delete the temporary Files created
    QVERIFY(QFile::exists("Stations_Test.xml"));
    QFile File("Stations_Test.xml");
    QVERIFY(File.remove("Stations_Test.xml"));
}


} // end namespace DataManager

QTEST_MAIN(DataManager::TestDataStationList)

#include "TestDataStationList.moc"

