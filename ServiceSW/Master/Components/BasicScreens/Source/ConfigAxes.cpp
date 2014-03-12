/****************************************************************************/
/*! \file ConfigAxes.cpp
 *
 *  \brief ConfigAxes implementation.
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

#include "BasicScreens/Include/ConfigAxes.h"

namespace BasicScreens {

/****************************************************************************/
/*!
 *  \brief Constructor
 *
 *  \iparam p_Parent = Parent widget
 */
/****************************************************************************/
CConfigAxes::CConfigAxes(QWidget *p_Parent) : MainMenu::CTextPanel(p_Parent)
{
    AddGripperArm("Left Gripper Arm", "LEFT");
    AddNewLine();
    AddNewLine();
    AddGripperArm("Right Gripper Arm", "RIGHT");
}

/****************************************************************************/
/*!
 *  \brief Adds a complete gripper arm to the panel
 *
 *  \iparam Caption = The main caption for the arm
 *  \iparam Identifier = The main identifier prefix
 */
/****************************************************************************/
void CConfigAxes::AddGripperArm(QString Caption, QString Identifier)
{
    QStringList Captions = QStringList() << "X-Axis" << "Y-Axis" << "Z-Axis";
    QStringList Identifiers = QStringList() << "XAXIS" << "YAXIS" << "ZAXIS";

    AddCaption(Caption);

    for (qint32 i = 0; i < 3; i++) {
        AddNewLine();
        AddCaption(Captions[i]);
        AddTextItem(Identifier + "_" + Identifiers[i] + "_SERIAL", "Serial number:", "n/a");
        AddTextItem(Identifier + "_" + Identifiers[i] + "_HARDWARE", "Hardware version:", "n/a");
        AddTextItem(Identifier + "_" + Identifiers[i] + "_SOFTWARE", "Software version:", "n/a");
        AddTextItem(Identifier + "_" + Identifiers[i] + "_RESULT", "Test result:", "n/a");
    }
}

} // end namespace BasicScreens
