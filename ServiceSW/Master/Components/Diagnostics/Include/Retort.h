/****************************************************************************/
/*! \file Retort.h
 *
 *  \brief Declaration of Retort test.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2013-05-27
 *   $Author:  $ R.Wu
 *
 *  \b Company:
 *
 *       Leica Biosystems R&D Center Shanghai.
 *
 *  (C) Copyright 2010 by LBS R&D Center Shanghai. All rights reserved.
 *  This is unpublished proprietary source code of Leica. The copyright notice
 *  does not evidence any actual or intended publication.
 *
 */
/****************************************************************************/

#ifndef DIAGNOSTICS_RETORT_H
#define DIAGNOSTICS_RETORT_H

#include <QWidget>

namespace Diagnostics {

namespace Ui {
class CRetort;
}

class CRetort : public QWidget
{
    Q_OBJECT
    
public:
    explicit CRetort(QWidget *parent = 0);
    ~CRetort();

protected:
    void changeEvent(QEvent *p_Event);

private Q_SLOTS:
    void StartLidLockTest(void);
    void StartLevelSensorHeatingTest(void);
    void StartLevelSensorDetectingTest(void);
    void StartHeatingTestEmpty(void);
    void StartHeatingTestWithLiquid(void);
    
private:
    Ui::CRetort *ui;

signals:
    void GuiLevelSensorHeatingTest();
    void GuiRetortEmptyHeatingTest();
    void GuiRetortLiquidHeatingTest();
    void GuiLevelSensorDetectingTest(qint32);

public slots:
    void OnStartLevelSensorDetectTest(qint32);

};


} // namespace Diagnostics
#endif // DIAGNOSTICS_RETORT_H
