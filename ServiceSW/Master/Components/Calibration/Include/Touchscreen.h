/****************************************************************************/
/*! \file Touchscreen.h
 *
 *  \brief Touchscreen definition.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2011-07-13
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

#ifndef CALIBRATION_TOUCHSCREEN_H
#define CALIBRATION_TOUCHSCREEN_H

#include <QWidget>

namespace Calibration {

namespace Ui {
    class CTouchscreen;
}

/****************************************************************************/
/**
 * \brief This panel provides a button that starts the touchscreen
 *        calibration.
 */
/****************************************************************************/
class CTouchscreen : public QWidget
{
    Q_OBJECT

public:
    explicit CTouchscreen(QWidget *p_Parent = 0);
    virtual ~CTouchscreen();

private:
    Ui::CTouchscreen *mp_Ui;    //!< User interface
};

} // end namespace Calibration

#endif // CALIBRATION_TOUCHSCREEN_H
