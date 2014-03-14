/****************************************************************************/
/*! \file System.h
 *
 *  \brief Declaration of System test.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2013-06-03
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

#ifndef DIAGNOSTICS_SYSTEM_H
#define DIAGNOSTICS_SYSTEM_H

#include <QWidget>

namespace Diagnostics {

namespace Ui {
class CSystem;
}

class CSystem : public QWidget
{
    Q_OBJECT
    
public:
    explicit CSystem(QWidget *parent = 0);
    ~CSystem();

private Q_SLOTS:
    void StartBottleCheckingTest(void);

    void StartFillingNDrainingTest(void);

    void StartOverflowTest(void);

    void StartSpeakerTest(void);

    void StartAlarmTest(void);

    void StartEthernetTest(void);

    void StartUsbTest(void);

    void StartMainsRelayTest(void);

    void StartVoltageTest(void);
    
private:
    Ui::CSystem *ui;
};


} // namespace Diagnostics
#endif // DIAGNOSTICS_SYSTEM_H
