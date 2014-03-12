/****************************************************************************/
/*! \file Temperature.h
 *
 *  \brief Temperature definition.
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

#ifndef BASICSCREENS_TEMPERATURE_H
#define BASICSCREENS_TEMPERATURE_H

#include "BasicScreens/Include/AnalogPanel.h"
#include <QLabel>

namespace BasicScreens {

/****************************************************************************/
/**
 * \brief This class shows all temperatures meassured by the device.
 */
/****************************************************************************/
class CTemperature : public BasicScreens::CAnalogPanel
{
    Q_OBJECT

public:
    explicit CTemperature(QWidget *p_Parent = 0);
};

} // end namespace BasicScreens

#endif // BASICSCREENS_TEMPERATURE_H
