/****************************************************************************/
/*! \file TestRMSHandler.cpp
 *
 *  \brief Implementation file for class TestRMSHandler.
 *
 *  $Version:   $ 0.1
 *  $Date:      $ 2012-08-21 2013-Jan-16
 *  $Author:    $ Vikram MK,Ningu
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
#include "RMS/Include/RMSHandler.h"
#include "RMS/Include/LeicaReagentBottle.h"
#include "RMS/Include/LeicaReagentKit.h"
#include "Master/Components/HimalayaDataManager/Include/DataManager.h"

namespace HimalayaRMS {

/****************************************************************************/
/**
 * \brief Test class for XmlConfigFileUserSettings class.
 */
/****************************************************************************/
class TestRMSHandler : public QObject {
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
     * \brief Test RMS handler.
     */
    /****************************************************************************/
    void utTestRMSHandler();

}; // end class TestExportController

/****************************************************************************/
void TestRMSHandler::initTestCase() {

}

/****************************************************************************/
void TestRMSHandler::init() {
}

/****************************************************************************/
void TestRMSHandler::cleanup() {
}

/****************************************************************************/
void TestRMSHandler::cleanupTestCase() {
}


/****************************************************************************/
void TestRMSHandler::utTestRMSHandler() {

//    CLeicaReagentKit *p_LeicaReagentkit = new CLeicaReagentKit();
//    CLeicaReagentBottle *p_LeicaReagentBottle = new CLeicaReagentBottle();
//    CRMSHandler *p_RMSHandler = new CRMSHandler();

//    QByteArray rfidkitData,rfidbottleData;
//    QDataStream dskit(&rfidkitData, QIODevice::WriteOnly);
//    //Kit data
//    dskit << (qint8)1;     // TypeID (kit=1, bottle=2)
//    dskit << (qint16)1;    // KitID
//    dskit << (qint8)1;     // number of kit reagents (= number of bottles)
//    dskit << (qint8)13;    // expiry date (year)
//    dskit << (qint8)11;    // expiry date (month)
//    dskit << (qint8)1;    // expiry date (day)
//    dskit << (qint8)3;     // open shelf life time (in days)
//    dskit << (qint16)2500; // maximum number of slides
//    dskit << (qint32)175;  // lot number of bottle 1

//    QDataStream dsbottle(&rfidbottleData, QIODevice::WriteOnly);
//    //Bottle data
//    dsbottle << (qint8)2;     // TypeID (kit=1, bottle=2)
//    dsbottle << (qint16)1;    // reagentID
//    dsbottle << (qint32)175;   // lot number
//    dsbottle << (qint16)0;    // maximum number of slides (redundant, kit data will be used)
//    p_LeicaReagentkit->DeserializeKitContent(rfidkitData);
//    p_LeicaReagentBottle->DeserializeBottleContent(rfidbottleData);


//    p_RMSHandler->ValidateLeicaReagentKit();
//    p_RMSHandler->ValidateLeicaReagentBottle();

//    delete p_RMSHandler;
//    delete p_LeicaReagentkit;
//    delete p_LeicaReagentBottle;

}

} // end namespace HimalayaRMS

QTEST_MAIN(HimalayaRMS::TestRMSHandler)

#include "TestRMSHandler.moc"
