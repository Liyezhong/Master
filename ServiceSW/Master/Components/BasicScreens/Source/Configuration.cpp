/****************************************************************************/
/*! \file Configuration.cpp
 *
 *  \brief Configuration implementation.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2011-06-28
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

#include "BasicScreens/Include/Configuration.h"

namespace BasicScreens {

/****************************************************************************/
/*!
 *  \brief Constructor
 *
 *  \iparam p_Parent = Parent widget
 */
/****************************************************************************/
CConfiguration::CConfiguration(QWidget *p_Parent) : MainMenu::CTextPanel(p_Parent)
{
    AddTextItem("TYPE", "Type:", "n/a");
    AddNewLine();
    AddTextItem("SERIAL", "Serial number:", "n/a");
    AddTextItem("HARDWARE", "Hardware version:", "n/a");
    AddTextItem("SOFTWARE", "Software version:", "n/a");
    AddNewLine();
    AddNewLine();
    AddTextItem("DATE", "Date last service:", "n/a");
}

} // end namespace BasicScreens
