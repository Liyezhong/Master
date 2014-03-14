/****************************************************************************/
/*! \file LevelSensor.h
 *
 *  \brief LevelSensor definition.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2011-07-11
 *   $Author:  $ M.Scherer
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

#ifndef CALIBRATION_LEVELSENSOR_H
#define CALIBRATION_LEVELSENSOR_H

#include <QWidget>

namespace Calibration {

namespace Ui {
    class CLevelSensor;
}

/****************************************************************************/
/**
 * \brief This class handles the calibration of the liquid level sensor.
 */
/****************************************************************************/
class CLevelSensor : public QWidget
{
    Q_OBJECT

public:
    explicit CLevelSensor(QWidget *p_Parent = 0);
    virtual ~CLevelSensor();

private:
    Ui::CLevelSensor *mp_Ui;    //!< User interface
    QStringList m_Instructions; //!< List of calibration instructions
    qint32 m_Step;              //!< Current step in the calibration process


public:
signals:
    void deviceConfigure();

private slots:
    void NextStep();
    void TestForDeviceInit();
};

} // end namespace Calibration

#endif // CALIBRATION_LEVELSENSOR_H
