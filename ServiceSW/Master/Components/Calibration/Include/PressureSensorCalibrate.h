/****************************************************************************/
/*! \file PressureSensorCalibrate.h
 *
 *  \brief Declaration of Pressure Sensor calibration.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2013-05-29
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

#ifndef CALIBRATION_PRESSURESENSORCALIBRATE_H
#define CALIBRATION_PRESSURESENSORCALIBRATE_H

#include <QObject>


namespace Calibration {

class CPressureSensorCalibrate : public QObject
{
    Q_OBJECT

public:
    CPressureSensorCalibrate(void);
    ~CPressureSensorCalibrate(void);

    void Run(void);

public Q_SLOTS:
    void StartCalibration(void);

private Q_SLOTS:
    void Succeed(void);

    void Fail(void);
};

} // namespace Diagnostics

#endif // DIAGNOSTICS_PRESSURESENSORCALIBRATE_H
