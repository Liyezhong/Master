/****************************************************************************/
/*! \file TestSchedulerCommand.cpp
 *
 *  \brief Implementation file for class TestSchedulerCommand.
 *
 *  $Version:   $ 0.1
 *  $Date:      $ 2014-01-27
 *  $Author:    $ Songtao Yu
 *
 *  \b Company:
 *
 *       Leica Biosystems Nussloch GmbH.
 *
 *  (C) Copyright 2013 by Leica Biosystems Nussloch GmbH. All rights reserved.
 *  This is unpublished proprietary source code of Leica. The copyright notice
 *  does not evidence any actual or intended publication.
 *
 */
/****************************************************************************/

#include <QTest> 

#include <HimalayaMasterThread/Include/ThreadIDs.h>
#include <Scheduler/Include/SchedulerMainThreadController.h>
#include <Scheduler/Include/SchedulerCommandProcessor.h>
#include <DeviceControl/Include/Interface/IDeviceProcessing.h>

namespace Scheduler {

/****************************************************************************/
/**
 * \brief Test class for TestSchedulerCommand class.
 */
/****************************************************************************/
class TestSchedulerCommand : public QObject {
    Q_OBJECT

private:
    SchedulerMainThreadController* m_pSchedulerMainController;
	DeviceControl::IDeviceProcessing* mp_IDeviceProcessing;
	
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

    void UTAll();

}; // end class TestEventCSVInfo


void TestSchedulerCommand::UTAll()
{

}

/******************************************************************ls**********/
void TestSchedulerCommand::initTestCase()
{

}

/****************************************************************************/
void TestSchedulerCommand::init()
{
	mp_IDeviceProcessing = new IDeviceProcessing();	
    m_pSchedulerMainController =new SchedulerMainThreadController(THREAD_ID_SCHEDULER, mp_IDeviceProcessing);
	m_pSchedulerMainController->CreateAndInitializeObjects();
}

/****************************************************************************/
void TestSchedulerCommand::cleanup()
{
    delete m_pSchedulerMainController;
    m_pSchedulerMainController = NULL;
	delete mp_IDeviceProcessing;
	mp_IDeviceProcessing = NULL;
}

/****************************************************************************/
void TestSchedulerCommand::cleanupTestCase()
{

}

} // end namespace EventHandler

QTEST_MAIN(Scheduler::TestSchedulerCommand)

#include "TestSchedulerCommand.moc"
