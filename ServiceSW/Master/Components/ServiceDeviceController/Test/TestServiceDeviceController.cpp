    /****************************************************************************/
/*! \file TestServiceDeviceController.cpp
 *
 *  \brief Implementation file for class TestServiceDeviceController.
 *
 *  \b Description:
 *         Checks the CTestServiceDeviceController class implementation
 *
 *  $Version:   $ 0.1
 *  $Date:      $ 2014-01-22
 *  $Author:    $ Srivathsa HH
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
#include "ServiceDeviceController/Include/DeviceProcessor/DeviceProcessor.h"
#include "DeviceControl/Include/Interface/IDeviceProcessing.h"

namespace DeviceControl {

/****************************************************************************/
/**
 * \brief Test class for CTestServiceDeviceController class.
 */
/****************************************************************************/
class CTestServiceDeviceController : public QObject {
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
     * \brief Test DeviceProcessor.
     */
    /****************************************************************************/
    void TestDeviceProcessor();

}; // end class CTestServiceDeviceController

/****************************************************************************/
void CTestServiceDeviceController::initTestCase() {

}

/****************************************************************************/
void CTestServiceDeviceController::init() {
}

/****************************************************************************/
void CTestServiceDeviceController::cleanup() {
}

/****************************************************************************/
void CTestServiceDeviceController::cleanupTestCase() {
}

/****************************************************************************/
void CTestServiceDeviceController::TestDeviceProcessor() {

    IDeviceProcessing iDevProc;
    DeviceProcessor* pDeviceControl = new DeviceProcessor(iDevProc);
    pDeviceControl->OnCalibrateDevice(Service::DEVICE_PRESSURE_SENSOR);
    delete pDeviceControl;
}



} // end namespace ImportExport

QTEST_MAIN(DeviceControl::CTestServiceDeviceController)

#include "TestServiceDeviceController.moc"
