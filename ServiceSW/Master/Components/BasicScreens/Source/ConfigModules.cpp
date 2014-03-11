/****************************************************************************/
/*! \file ConfigModules.cpp
 *
 *  \brief ConfigModules implementation.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2011-06-29
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

#include "BasicScreens/Include/ConfigModules.h"

namespace BasicScreens {

/****************************************************************************/
/*!
 *  \brief Constructor
 *
 *  \iparam p_Parent = Parent widget
 */
/****************************************************************************/
CConfigModules::CConfigModules(QWidget *p_Parent) : MainMenu::CTextPanel(p_Parent)
{
    AddModule("Loading Drawer", "LOADING");
    AddNewLine();
    AddModule("Unloading Drawer", "UNLOADING");
    AddNewLine();
    AddModule("Oven", "OVEN");
    AddNewLine();
    AddModule("Heated Cuvettes", "HEATED");
    AddNewLine();
    AddModule("Rack Transfer", "TRANSFER");
    AddNewLine();
    AddModule("Slide ID", "SLIDEID");
}

/****************************************************************************/
/*!
 *  \brief Adds an assembly group to the panel
 *
 *  \iparam Caption = The name of the module
 *  \iparam Identifier = The identifier of the module
 */
/****************************************************************************/
void CConfigModules::AddModule(QString Caption, QString Identifier)
{
    AddCaption(Caption);
    AddTextItem(Identifier + "_SERIAL", "Serial number:", "n/a");
    AddTextItem(Identifier + "_HARDWARE", "Hardware version:", "n/a");
    AddTextItem(Identifier + "_SOFTWARE", "Software version:", "n/a");
    AddTextItem(Identifier + "_RESULT", "Test result:", "n/a");
}

} // end namespace BasicScreens
