/****************************************************************************/
/*! \file TestDataProgramSequenceList.cpp
 *
 *  \brief Unit test for program sequence container.
 *
 *  $Version:   $ 0.1
 *  $Date:      $ 2012-05-08
 *  $Author:    $ N.Kamath
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
#include <QDebug>
#include <QFile>
#include <ColoradoDataContainer/Containers/ProgramSequence/Include/ProgramSequenceList.h>
#include <ColoradoDataContainer/Containers/ProgramSequence/Include/ProgramSequenceVerifier.h>


namespace DataManager {

/****************************************************************************/
/**
 * \brief Test class for Program sequence class.
 */
/****************************************************************************/
class TestDataProgramSequenceList : public QObject {
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
     * \brief Test  Program sequence object
     */
    /****************************************************************************/
    void utTestDataProgramSequence();

    /****************************************************************************/
    /**
     * \brief Test data CProgramSequenceList
     */
    /****************************************************************************/
    void utTestDataProgramSequenceList();

    /****************************************************************************/
    /**
     * \brief Test Program sequence verifier
     */
    /****************************************************************************/
    void utTestDPSLListVerifier();
}; // end class TestDataReagentList

/****************************************************************************/
void TestDataProgramSequenceList::initTestCase() {
}

/****************************************************************************/
void TestDataProgramSequenceList::init() {
}

/****************************************************************************/
void TestDataProgramSequenceList::cleanup() {
}

/****************************************************************************/
void TestDataProgramSequenceList::cleanupTestCase() {
}

/****************************************************************************/
void TestDataProgramSequenceList::utTestDataProgramSequence() {
    CProgramSequence *p_ProgSequence = new CProgramSequence();
    p_ProgSequence->SetCounterValue(3);
    p_ProgSequence->SetID("U10");
    p_ProgSequence->SetStartCapableStatus(true);
    p_ProgSequence->SetUsedStatus(true);
    p_ProgSequence->SetStartCapableResult("1,2");
    //Test getters
    QCOMPARE((int)p_ProgSequence->GetCounterValue(), 3);
    QCOMPARE(p_ProgSequence->GetID(), QString("U10"));
    QCOMPARE(p_ProgSequence->GetStartCapableStatus(), true);
    QCOMPARE(p_ProgSequence->GetUsedStatus(), true);
    QCOMPARE(p_ProgSequence->GetStartCapableResult(), QString("1,2"));

    // Test copy constructor , also test assignment operator
    CProgramSequence ProgramSequence(*p_ProgSequence);
    //Test getters
    QCOMPARE((int)ProgramSequence.GetCounterValue(), 3);
    QCOMPARE(ProgramSequence.GetID(), QString("U10"));
    QCOMPARE(ProgramSequence.GetStartCapableStatus(), true);
    QCOMPARE(ProgramSequence.GetUsedStatus(), true);
    QCOMPARE(ProgramSequence.GetStartCapableResult(), QString("1,2"));

    ErrorHash_t Dummy;
    ProgramSequence.SetErrorList(&Dummy);

    // check constructor with parameters
    CProgramSequence ProgramSeq("U12");
    QCOMPARE(ProgramSeq.GetID(), QString("U12"));
    ProgramSeq.SetStartCapableDynamicStatus(false);
    CProgramSequence ProgramSeq1(ProgramSeq);
    QCOMPARE(ProgramSeq1.GetStartCapableDynamicStatus(), false);

}


/****************************************************************************/
void TestDataProgramSequenceList::utTestDataProgramSequenceList() {
    //Test program list
    CProgramSequenceList *p_ProgramSequenceList = new CProgramSequenceList();
    CProgramSequenceListVerifier *p_ProgramSeqListVerifier = new CProgramSequenceListVerifier();
    p_ProgramSequenceList->AddVerifier(p_ProgramSeqListVerifier);
    p_ProgramSequenceList->SetDataVerificationMode(false);
    QCOMPARE(p_ProgramSequenceList->Read(":/Xml/ProgramSequence.xml"), true);

    p_ProgramSequenceList->SetDataVerificationMode(true);
    QCOMPARE(p_ProgramSequenceList->GetDataContainerType(), PROGRAMSEQUENCE);
    QCOMPARE(p_ProgramSequenceList->GetDataVerificationMode(), true);
    QCOMPARE(p_ProgramSequenceList->GetFilename(),QString(":/Xml/ProgramSequence.xml"));
    QCOMPARE(p_ProgramSequenceList->GetNumberOfPrograms(), 7);
    QCOMPARE(p_ProgramSequenceList->GetVersion(), 1);
    QStringList StartCapableList;
    StartCapableList << "L1" << "U1" << "U2" << "U3" << "U4";
    qDebug()<<"\n StartableProgramIDList:"<<p_ProgramSequenceList->GetStartableProgramIDList();
    QCOMPARE(p_ProgramSequenceList->GetStartableProgramIDList(), StartCapableList);

    QVERIFY(!p_ProgramSequenceList->GetProgramSequenceStep("Dummy"));

    //Test Copy constructor
    CProgramSequenceList *p_ProgramSequenceList2 = new CProgramSequenceList(*p_ProgramSequenceList);
    QCOMPARE(p_ProgramSequenceList2->GetFilename(),QString(":/Xml/ProgramSequence.xml"));
    QCOMPARE(p_ProgramSequenceList2->GetDataContainerType(), PROGRAMSEQUENCE);
    QCOMPARE(p_ProgramSequenceList2->GetDataVerificationMode(), true);
    QCOMPARE(p_ProgramSequenceList2->GetNumberOfPrograms(), p_ProgramSequenceList->GetNumberOfPrograms());
    QCOMPARE(p_ProgramSequenceList2->GetVersion(), 1);

    CProgramSequence *p_ProgSequence = new CProgramSequence("U11", false, 3, true, "0");

    p_ProgramSequenceList->AddProgramSequenceStep(p_ProgSequence);

    QCOMPARE(p_ProgramSequenceList->GetNumberOfPrograms(), 8);

    //Check contents of program sequence added
    CProgramSequence ProgramSequence2;
    QCOMPARE(p_ProgramSequenceList->GetProgramSequenceStep("U11", ProgramSequence2), true);
    QCOMPARE(ProgramSequence2.GetID(), p_ProgSequence->GetID() );
    QCOMPARE(ProgramSequence2.GetStartCapableResult(), p_ProgSequence->GetStartCapableResult());
    QCOMPARE(ProgramSequence2.GetCounterValue(), p_ProgSequence->GetCounterValue());
    QCOMPARE(ProgramSequence2.GetStartCapableStatus(), p_ProgSequence->GetStartCapableStatus());
    QCOMPARE(ProgramSequence2.GetUsedStatus(), p_ProgSequence->GetUsedStatus());

    CProgramSequence ProgramSequence3;
    QCOMPARE(p_ProgramSequenceList->GetProgramSequenceStep(0, ProgramSequence3), true);
    QCOMPARE(ProgramSequence3.GetID(), QString("L1"));
    QCOMPARE(ProgramSequence3.GetStartCapableResult(), QString("0"));
    QCOMPARE((int)ProgramSequence3.GetCounterValue(), 35);
    QCOMPARE(ProgramSequence3.GetStartCapableStatus(), true);
    QCOMPARE(ProgramSequence3.GetUsedStatus(), true);

    //Check program sequence update functionality
    CProgramSequence *p_ProgramSequence4 = new CProgramSequence();
    QCOMPARE(p_ProgramSequenceList->GetProgramSequenceStep(0, *p_ProgramSequence4), true);
    p_ProgramSequence4->SetID("U37");
    p_ProgramSequence4->SetStartCapableResult("2");
    p_ProgramSequence4->SetCounterValue(48);
    p_ProgramSequence4->SetStartCapableStatus(true);
    p_ProgramSequence4->SetUsedStatus(true);
    StartCapableList.append(p_ProgramSequence4->GetID());
    QCOMPARE(p_ProgramSequenceList->AddProgramSequenceStep(p_ProgramSequence4), true);
    QCOMPARE(p_ProgramSequenceList->GetStartableProgramIDList(), StartCapableList);

    CProgramSequence ProgramSequence5;
    QCOMPARE(p_ProgramSequenceList->GetProgramSequenceStep(8, ProgramSequence5), true);
    QCOMPARE(ProgramSequence5.GetID(), p_ProgramSequence4->GetID());
    QCOMPARE(ProgramSequence5.GetStartCapableResult(), p_ProgramSequence4->GetStartCapableResult());
    QCOMPARE(ProgramSequence5.GetCounterValue(), p_ProgramSequence4->GetCounterValue());
    QCOMPARE(ProgramSequence5.GetStartCapableStatus(), p_ProgramSequence4->GetStartCapableStatus());
    QCOMPARE(ProgramSequence5.GetUsedStatus(), p_ProgramSequence4->GetUsedStatus());

    //Check Update Function
    ProgramSequence5.SetUsedStatus(false);
    QCOMPARE(p_ProgramSequenceList->UpdateProgramSequenceStep(&ProgramSequence5), true);



    QCOMPARE(p_ProgramSequenceList->UpdateProgramSequenceStep(NULL), false);

    delete p_ProgSequence;
    //This will check the Delete methods
    delete p_ProgramSequenceList2;
    delete p_ProgramSequenceList;
}

void TestDataProgramSequenceList::utTestDPSLListVerifier() {
    CProgramSequenceList *p_ProgramSeqList = new CProgramSequenceList();
    CProgramSequenceListVerifier *p_ProgramSeqListVerifier = new CProgramSequenceListVerifier();
    p_ProgramSeqList->AddVerifier(p_ProgramSeqListVerifier);
    p_ProgramSeqList->SetDataVerificationMode(false);
    QCOMPARE(p_ProgramSeqList->Read(":/Xml/ProgramSequenceNOK.xml"), false);
    p_ProgramSeqList->SetDataVerificationMode(true);
    CProgramSequence *p_ProgSequence = new CProgramSequence("UU11", false, 3, true, "1");
    QCOMPARE(p_ProgramSeqList->AddProgramSequenceStep(p_ProgSequence), false);
}

} // end namespace DataManager

QTEST_MAIN(DataManager::TestDataProgramSequenceList)

#include "TestDataProgramSequenceList.moc"

