/****************************************************************************/
/*! \file TestDataReagentList.cpp
 *
 *  \brief Implementation file for class TestDataCReagent.
 *
 *  $Version:   $ 0.1
 *  $Date:      $ 2012-04-23
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
#include <HimalayaDataManager/Containers/Reagents/Include/DataReagentList.h>
#include <HimalayaDataManager/Containers/Reagents/Include/DataReagentListVerifier.h>

namespace DataManager {

/****************************************************************************/
/**
 * \brief Test class for CReagent class.
 */
/****************************************************************************/
class TestDataReagentList : public QObject {
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
     * \brief Test data CReagent.
     */
    /****************************************************************************/
    void utTestDataReagent();
    /****************************************************************************/
    /**
     * \brief Test data CDataReagentList.
     */
    /****************************************************************************/
    void utTestDataReagentList();
    /****************************************************************************/
    /**
     * \brief Test data CDataReagentListVerifier.
     */
    /****************************************************************************/
    void utTestDataReagentListVerifier();
}; // end class TestDataReagentList

/****************************************************************************/
void TestDataReagentList::initTestCase() {
}

/****************************************************************************/
void TestDataReagentList::init() {
}

/****************************************************************************/
void TestDataReagentList::cleanup() {
}

/****************************************************************************/
void TestDataReagentList::cleanupTestCase() {
}

/****************************************************************************/
void TestDataReagentList::utTestDataReagent() {

    CReagent *p_Reagent1 = new CReagent();
    CReagent *p_Reagent2 = new CReagent();

    p_Reagent1->SetID("S9"); // Special Reagent
    p_Reagent1->SetShortName("UNL");
    p_Reagent1->SetLongName("UNLOADER");
    p_Reagent1->SetVisibleState(false);

    QCOMPARE(p_Reagent1->GetID(), QString("S9"));
    QCOMPARE(p_Reagent1->GetShortName(), QString("UNL"));
    QCOMPARE(p_Reagent1->GetLongName(), QString("UNLOADER"));
    QCOMPARE(p_Reagent1->GetVisibleState(), false);

    p_Reagent2->SetID("L3"); // LEICA Reagent
    p_Reagent2->SetShortName("Eosin10%");
    p_Reagent2->SetLongName("EosinParameter10%");
    p_Reagent2->SetClass("5");
    p_Reagent2->Set5RackMode(true);
    p_Reagent2->SetLockState(true);
    p_Reagent2->SetMaxTime("1d");
    p_Reagent2->SetMaxSlides(2500);
    p_Reagent2->SetReagentTemp(40);
    p_Reagent2->SetHeatingStartMode(AT_PROGRAM_START);

    CReagent Reagent3(*p_Reagent2); // copy constructor of CReagent

    QCOMPARE(Reagent3.GetLockState(), true);
    QCOMPARE(Reagent3.Get5RackMode(), true);
    QCOMPARE(Reagent3.GetClass(), QString("5"));
    QCOMPARE(Reagent3.GetMaxTimeInSeconds(), 86400);
    QCOMPARE(Reagent3.GetMaxSlides(), 2500);
    QCOMPARE(Reagent3.GetReagentTemp(), 40);
    QCOMPARE(Reagent3.GetHeatingStartMode(), AT_PROGRAM_START);

}
/****************************************************************************/
void TestDataReagentList::utTestDataReagentList() {

    CReagent *p_Reagent4 = new CReagent();
    CReagent *p_Reagent5 = new CReagent();
    CDataReagentList *p_ReagentList = new CDataReagentList();
    IVerifierInterface *p_DataReagentListVerifier;
    p_DataReagentListVerifier = new CDataReagentListVerifier();
    p_ReagentList->AddVerifier(p_DataReagentListVerifier);

    p_ReagentList->SetDataVerificationMode(false);
    // Checking the Read of CDataReagentList
    QCOMPARE(p_ReagentList->Read(":/Xml/Reagents.xml"), true);
    p_ReagentList->SetDataVerificationMode(true);

    // Checking the basic interfaces of CDataReagentList
    QCOMPARE(p_ReagentList->GetNumberOfReagents(), 34);
    QCOMPARE(p_ReagentList->GetVersion(), 3);
    QCOMPARE(p_ReagentList->GetReagentType("U100"), USER_REAGENT);
    QCOMPARE(p_ReagentList->GetReagentType(12), LEICA_REAGENT);
    QCOMPARE(p_ReagentList->ReagentExists("S7"), true);
    QCOMPARE(p_ReagentList->ReagentExists("U78"), false);

    p_Reagent4->SetID("L3"); // LEICA Reagent
    p_Reagent4->SetShortName("Eosin10%");
    p_Reagent4->SetLongName("EosinParameter10%");
    p_Reagent4->SetClass("5");
    p_Reagent4->Set5RackMode(true);
    p_Reagent4->SetLockState(true);
    p_Reagent4->SetMaxTime("1d");
    p_Reagent4->SetMaxSlides(2500);
    p_Reagent4->SetReagentTemp(40);
    p_Reagent4->SetHeatingStartMode(AT_PROGRAM_START);

    // Checking the AddReagent
    QVERIFY(p_ReagentList->AddReagent(p_Reagent4)); // added the Leica reagent

    p_Reagent5->SetID("S12"); // Special Reagent
    p_Reagent5->SetShortName("UNL1");
    p_Reagent5->SetLongName("UNLOADER1");
    p_Reagent5->SetVisibleState(false);
    QVERIFY(p_ReagentList->AddReagent(p_Reagent5)); // added the Special reagent
    QCOMPARE(p_ReagentList->GetNumberOfReagents(), 36);

    // Create a User Reagent
    CReagent *p_Reagent3 = p_ReagentList->CreateReagent();
    p_Reagent3->SetShortName("USER");
    p_Reagent3->SetLongName("NEWLYADDED");
    QVERIFY(p_ReagentList->AddReagent(p_Reagent3)); // added the User reagent
    QCOMPARE(p_ReagentList->GetNumberOfReagents(), 37);

    CReagent *p_Reagent6 = new CReagent();
    CReagent *p_Reagent7 = new CReagent();
    CReagent *p_Reagent8 = new CReagent();

    QCOMPARE(p_ReagentList->GetReagent(12, *p_Reagent6) , true);
    QCOMPARE(p_ReagentList->GetReagent("L3", *p_Reagent7) , true);
    QCOMPARE(p_ReagentList->GetReagent("U126", *p_Reagent8) , true);

    p_Reagent8->SetClass("9");
    p_Reagent8->Set5RackMode(false);
    p_Reagent8->SetLockState(true);
    p_Reagent8->SetMaxTime("5d");
    p_Reagent8->SetMaxSlides(2700);
    p_Reagent8->SetReagentTemp(50);
    p_Reagent8->SetHeatingStartMode(AT_DEVICE_START);

    // Checking the Update Reagent
    QCOMPARE(p_ReagentList->UpdateReagent(p_Reagent8), true);
    QCOMPARE(p_ReagentList->ReagentExists(p_Reagent8->GetID()), true);
    // checking to update a non existing reagent
    CReagent *p_Reagent9 = new CReagent();
    p_Reagent9->SetID("S13");
    QCOMPARE(p_ReagentList->UpdateReagent(p_Reagent9), false);

    //Checking the Delete Reagent
    QVERIFY(p_ReagentList->DeleteReagent(p_Reagent6->GetID())); // deleted special reagent
    QCOMPARE(p_ReagentList->ReagentExists("S12"), false);
    QCOMPARE(p_ReagentList->GetNumberOfReagents(), 36);

    // Check the copy constructor of CDataReagentList
    CDataReagentList ReagentList1(*p_ReagentList);

    // This file would be created in build folder particular to the Target (bin_dlg)
    QCOMPARE(ReagentList1.Write("Reagents_Test.xml"), true);

    //check the Write Method
    CDataReagentList *p_ReagentList2 = new CDataReagentList();
    IVerifierInterface *p_DataReagentListVerifier1;
    p_DataReagentListVerifier1 = new CDataReagentListVerifier();
    p_ReagentList2->AddVerifier(p_DataReagentListVerifier1);

    p_ReagentList2->SetDataVerificationMode(false);
    // Checking the Read of CDataReagentList
    QCOMPARE(p_ReagentList2->Read("Reagents_Test.xml"), true);
    p_ReagentList2->SetDataVerificationMode(true);

    //delete a Reagent from the list
    QCOMPARE(p_ReagentList2->DeleteReagent("S6"), true);
    QCOMPARE(static_cast<CDataContainerBase*>(p_ReagentList2)->Write(), true);

}
/****************************************************************************/
void TestDataReagentList::utTestDataReagentListVerifier() {

    CDataReagentList ReagentList2;
    IVerifierInterface *p_DataReagentListVerifier1;
    p_DataReagentListVerifier1 = new CDataReagentListVerifier();
    ReagentList2.AddVerifier(p_DataReagentListVerifier1);

    ReagentList2.SetDataVerificationMode(false);
    QCOMPARE(ReagentList2.Read("Reagents_Test.xml"), true);
    ReagentList2.SetDataVerificationMode(true);

    CReagent *p_Reagent10 = ReagentList2.CreateReagent();

    p_Reagent10->SetShortName("USER2");

    p_Reagent10->SetClass("5");
    p_Reagent10->Set5RackMode(false);
    p_Reagent10->SetLockState(true);

    // added the User reagent but with more number of Slides
    p_Reagent10->SetMaxSlides(3500);
    QCOMPARE(ReagentList2.AddReagent(p_Reagent10), false);
    QCOMPARE(ReagentList2.GetNumberOfReagents(), 35);

    // added the User reagent but with non temperature range
    p_Reagent10->SetMaxSlides(3000);
    p_Reagent10->SetReagentTemp(20);
    QCOMPARE(ReagentList2.AddReagent(p_Reagent10), false);
    QCOMPARE(ReagentList2.GetNumberOfReagents(), 35);

    // added the User reagent but with non heating start mode
    p_Reagent10->SetReagentTemp(40);
//    p_Reagent8->SetHeatingStartMode(static_cast<HeatingStartMode_t>(9));
//    QCOMPARE(ReagentList1.AddReagent(p_Reagent8), false);
//    QCOMPARE(ReagentList1.GetNumberOfReagents(), 32);

    // added the User reagent but with max time in seconds failure
    p_Reagent10->SetHeatingStartMode(AT_DEVICE_START);
    p_Reagent10->SetMaxTimeInSeconds(76500); // less than 1 day
    QCOMPARE(ReagentList2.AddReagent(p_Reagent10), false);
    QCOMPARE(ReagentList2.GetNumberOfReagents(), 35);

    // added the User reagent but with short name out of range
    p_Reagent10->SetMaxTimeInSeconds(86400);
    p_Reagent10->SetShortName("LEICATESTREAGENT");
    QCOMPARE(ReagentList2.AddReagent(p_Reagent10), false);
    QCOMPARE(ReagentList2.GetNumberOfReagents(), 35);

    // added the User reagent but with long name out of range
    p_Reagent10->SetShortName("USER2");
    p_Reagent10->SetLongName("NEWREAGENTNEWREAGENTNEWREAGENTNEWREAGENT");
    QCOMPARE(ReagentList2.AddReagent(p_Reagent10), false);
    QCOMPARE(ReagentList2.GetNumberOfReagents(), 35);

    p_Reagent10->SetLongName("NEWREAGENT");

    QCOMPARE(ReagentList2.AddReagent(p_Reagent10), true);
    QCOMPARE(ReagentList2.GetNumberOfReagents(), 36);

    // write it in the file
    QCOMPARE(ReagentList2.Write("Reagents_Test2.xml"), true);

    // delete the temporary Files created
    QVERIFY(QFile::exists("Reagents_Test.xml"));
    QFile File("Reagents_Test.xml");
    QVERIFY(File.remove("Reagents_Test.xml"));

    QVERIFY(QFile::exists("Reagents_Test2.xml"));
    QFile File1("Reagents_Test2.xml");
    QVERIFY(File1.remove("Reagents_Test2.xml"));
}


} // end namespace DataManager

QTEST_MAIN(DataManager::TestDataReagentList)

#include "TestDataReagentList.moc"

