/****************************************************************************/
/*! \file Temperature.cpp
 *
 *  \brief Temperature implementation.
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

#include "BasicScreens/Include/Temperature.h"

namespace BasicScreens {

/****************************************************************************/
/*!
 *  \brief Constructor
 *
 *  \iparam p_Parent = Parent widget
 */
/****************************************************************************/
CTemperature::CTemperature(QWidget *p_Parent) : BasicScreens::CAnalogPanel(p_Parent)
{
    AddCaption("Oven");
    AddAnalogItem("OVEN_TEMP1", "Temperature sensor 1:", "n/a");
    AddAnalogItem("OVEN_TEMP2", "Temperature sensor 2:", "n/a");

    for (qint32 i = 0; i < 6; i++) {
        AddNewLine();
        AddCaption("Heated Cuvette " + QString::number(i));
        AddAnalogItem("HEATED" + QString::number(i) + "_TEMP1", "Reagent temperature:", "n/a");
        AddAnalogItem("HEATED" + QString::number(i) + "_TEMP2", "Hotplate temperature:", "n/a");
    }
}

} // end namespace BasicScreens
