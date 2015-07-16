#include <QString>
#include <QtTest>

#include "HimalayaMasterThread/Include/HimalayaMasterThreadController.h"
#include <SWUpdateManager/Include/SWUpdateManager.h>

//namespace Himalaya {

class TestSWUpate : public QObject
{
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
     * \brief Test Himalaya Sepia controller.
     */
    /****************************************************************************/
    void utTestSWUpate();
}; // end class TestHimalayaSepiaController

/****************************************************************************/
void TestSWUpate::initTestCase() {

}

/****************************************************************************/
void TestSWUpate::init() {
}

/****************************************************************************/
void TestSWUpate::cleanup() {
}

/****************************************************************************/
void TestSWUpate::cleanupTestCase() {
}

void TestSWUpate::utTestSWUpate()
{
        Himalaya::HimalayaMasterThreadController TheMasterThreadController;
        SWUpdate::SWUpdateManager *swup = new SWUpdate::SWUpdateManager(TheMasterThreadController);

        QString opt = "-check";
        QString from = "USB";
        swup->UpdateSoftware(opt, from);
        swup->PowerFailed();

//        swup->SWUpdateCheckComplete();
//        swup->RollBackComplete();
//        swup->WaitDialog(false, Global::DEFAULT_TEXT);
//        swup->SWUpdateStatus(false);

        delete swup;
}

//} //end of namespace
//QTEST_MAIN(Himalaya::TestSWUpate)
QTEST_MAIN(TestSWUpate)

#include "TestSWUpate.moc"
