/****************************************************************************/
/*! \file PressureSensor.h
 *
 *  \brief Declaration of Pressure Sensor menu.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2013-05-23
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

#ifndef CALIBRATION_PRESSURESENSOR_H
#define CALIBRATION_PRESSURESENSOR_H

#include <QWidget>

namespace Calibration {

namespace Ui {
    class CPressureSensor;
}

/****************************************************************************/
/**
 * \brief This panel provides a button that starts the pressure sensor
 *        calibration.
 */
/****************************************************************************/
class CPressureSensor : public QWidget
{
    Q_OBJECT
    
public:
    explicit CPressureSensor(QWidget *parent = 0);
    ~CPressureSensor();

protected:
    void changeEvent(QEvent *p_Event);

private Q_SLOTS:
    void StartCalibration(void);
    
private:
    Ui::CPressureSensor *mp_Ui;

signals:
    /****************************************************************************/
    /**
     * \brief Signal is emitted for pressure sensor calibration
     */
    /****************************************************************************/
    void CalibrationPressureSensor();

private slots:
    void RetranslateUI();
public:
    void SetButtonStatus(bool enable);

};


} // namespace Calibration
#endif // CALIBRATION_PRESSURESENSOR_H
