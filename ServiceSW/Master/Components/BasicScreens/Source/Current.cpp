/****************************************************************************/
/*! \file Current.cpp
 *
 *  \brief Current implementation.
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

#include "BasicScreens/Include/Current.h"

namespace BasicScreens {

/****************************************************************************/
/*!
 *  \brief Constructor
 *
 *  \iparam p_Parent = Parent widget
 */
/****************************************************************************/
CCurrent::CCurrent(QWidget *p_Parent) : BasicScreens::CAnalogPanel(p_Parent)
{
    AddModule("Main Board", "MAIN");
    AddNewLine();
    AddModule("Y-Axis", "YAXIS");
    AddNewLine();
    AddModule("Z-Axis", "ZAXIS");
    AddNewLine();
    AddModule("Loading Drawer", "LOADING");
    AddNewLine();
    AddModule("Unloading Drawer", "UNLOADING");
    AddNewLine();
    AddModule("Oven", "OVEN");
    AddAnalogItem("OVEN_HEATER", "Heater current:", "n/a");
    AddNewLine();
    AddModule("Heated Cuvettes", "HEATED");
    AddAnalogItem("HEATED_HEATER", "Heater current:", "n/a");
    AddNewLine();
    AddModule("Rack Transfer", "TRANSFER");
    AddNewLine();
    AddModule("Slide ID", "SLIDEID");
}

/****************************************************************************/
/*!
 *  \brief Adds an assembly group to the screen
 *
 *  \iparam Caption = Caption of the new section
 *  \iparam Identifier = Identifier prefix of the new section
 */
/****************************************************************************/
void CCurrent::AddModule(QString Caption, QString Identifier)
{
    AddCaption(Caption);
    AddAnalogItem(Identifier + "_VOLTAGE", "Board voltage:", "n/a");
    AddAnalogItem(Identifier + "_CURRENT", "Board current:", "n/a");
}

} // end namespace BasicScreens
