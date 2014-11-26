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
#include "SVCDiagnostics/Include/GraphicsItemPart.h"
#include "SVCDiagnostics/Include/SVCDashboardWidget.h"
#include "SVCDiagnostics/Include/SVCRotaryValveDlg.h"
#include "SVCDiagnostics/Include/SVCButton.h"
#include "SVCDiagnostics/Include/SVCLabel.h"

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

    /****************************************************************************/
    /**
     * \brief Test Diagnostics Menu object
     */
    /****************************************************************************/
    void utTestRotaryValveDlg();

    /****************************************************************************/
    /**
     * \brief Test Diagnostics SVCLabel & SVCButton
     */
    /****************************************************************************/
    void utTestLabelButton();

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
    CGraphicsItemPart* ItemPart = new CGraphicsItemPart(QPixmap(":/Images/Retort.png"));
    ItemPart->SetStatus(CGraphicsItemPart::Working);

    QVERIFY(CGraphicsItemPart::Working == ItemPart->Status());

    CSVCDashboardWidget* TestWidget = new CSVCDashboardWidget();
    TestWidget->UpdateAirHeatingTubeLabel(11, 12);
    TestWidget->UpdateLiquidHeatingTubeLabel(11, 12);
    TestWidget->UpdateOvenLabel(11, 11, 11, 12);
    TestWidget->UpdatePressureLabel(11.2);
    TestWidget->UpdateRetortLabel(11, 11, 11, 12);
    TestWidget->UpdateRotaryValveLabel(true,10, 11, 11, 12);

    TestWidget->UpdatePartStatus();

    TestWidget->TimerStart(true);
    TestWidget->TimerStart(false);
}

/****************************************************************************/
void CTestSVCDiagnostics::utTestRotaryValveDlg()
{
    CSVCRotaryValveDlg rotaryValveDlg(NULL);
    bool flag1, flag2;
    qint32 pos1, pos2;

    flag1 = true;
    pos1 = 10;
    rotaryValveDlg.SetPos(flag1, flag1);
    rotaryValveDlg.GetPos(flag2, pos2);

    QCOMPARE(flag1, flag2);
    QCOMPARE(pos1, pos2);
}

/****************************************************************************/
void CTestSVCDiagnostics::utTestLabelButton()
{
    SVCLabel  label;
    label.setText(tr("test"));
    label.setPos(10, 10);
    label.setPos(QPoint(10, 10));
    label.setSize(10, 10);

    SVCLabel  label1("test");
    label1.setText(tr("hello"));

    SVCButton button;
    button.setText(tr("test"));
    button.setPos(QPoint(10, 10));
    button.setPos(10, 10);
    button.setSize(10, 10);

    SVCButton button1(tr("hello"));
    button1.setText(tr("test"));
}

} // end namespace SVCDiagnostics

QTEST_MAIN(SVCDiagnostics::CTestSVCDiagnostics)
#include "TestSVCDiagnostics.moc"

