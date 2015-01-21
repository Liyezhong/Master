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
    /****************************************************************************/
    /*!
     *  \brief Constructor
     *  \iparam p_Parent = parent windows
     */
    /****************************************************************************/
    explicit CPressureSensor(QWidget *parent = 0);

    /****************************************************************************/
    /*!
     *  \brief Destructor
     */
    /****************************************************************************/
    ~CPressureSensor();

protected:
    /****************************************************************************/
    /*!
     *  \brief Event handler for change events
     *
     *  \iparam p_Event = Change event
     */
    /****************************************************************************/
    void changeEvent(QEvent *p_Event);

private Q_SLOTS:

    /****************************************************************************/
    /*!
     *  \brief Slot for start calibration
     */
    /****************************************************************************/
    void StartCalibration(void);
    
private:
    Ui::CPressureSensor *mp_Ui;     //!< user interface

signals:
    /****************************************************************************/
    /**
     * \brief Signal is emitted for pressure sensor calibration
     */
    /****************************************************************************/
    void CalibrationPressureSensor();

private slots:

    /****************************************************************************/
    /*!
     *  \brief Translate string
     */
    /****************************************************************************/
    void RetranslateUI();
public:

    /****************************************************************************/
    /*!
     *  \brief To set button status
     *  \iparam enable = flag for enable button
     */
    /****************************************************************************/
    void SetButtonStatus(bool enable);

};


} // namespace Calibration
#endif // CALIBRATION_PRESSURESENSOR_H
