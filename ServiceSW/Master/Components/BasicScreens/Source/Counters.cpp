/****************************************************************************/
/*! \file Counters.cpp
 *
 *  \brief Counters implementation.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2011-07-08
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

#include "BasicScreens/Include/Counters.h"

namespace BasicScreens {

/****************************************************************************/
/*!
 *  \brief Constructor
 *
 *  \iparam p_Parent = Parent widget
 */
/****************************************************************************/
CCounters::CCounters(QWidget *p_Parent) : MainMenu::CTextPanel(p_Parent)
{
    AddModule("Total unit", "TOTAL");
    AddNewLine();
    AddNewLine();

    AddGripperArm("Left Gripper Arm", "LEFT");
    AddNewLine();
    AddNewLine();
    AddGripperArm("Right Gripper Arm", "RIGHT");
    AddNewLine();
    AddNewLine();

    AddCaption("Other Modules");
    AddNewLine();
    AddModule("Agitation", "AGITATION", "Stepper motor");
    AddNewLine();
    AddModule("Loading Drawer", "LOADING", "Stepper motor");
    AddNewLine();
    AddModule("Unloading Drawer", "UNLOADING", "Stepper motor");
    AddNewLine();
    AddModule("Oven", "OVEN", "Stepper motor");
    AddTextItem("OVEN_HEAT", "Heater operating hours:", "n/a");
    AddNewLine();
    AddModule("Heated Cuvettes", "HEATED");
    AddTextItem("HEATED_HEAT", "Heater operating hours:", "n/a");
    AddNewLine();
    AddModule("Rack Transfer", "TRANSFER", "Stepper motor");
    AddNewLine();
    AddModule("Slide ID", "SLIDEID", "Stepper motor");
}

/****************************************************************************/
/*!
 *  \brief Adds a gripper arm to the panel
 *
 *  \iparam Caption = Caption of the section
 *  \iparam Identifier = Identifier prefix of the section
 */
/****************************************************************************/
void CCounters::AddGripperArm(QString Caption, QString Identifier)
{
    QStringList Captions = QStringList() << "X-Axis" << "Y-Axis" << "Z-Axis";
    QStringList Identifiers = QStringList() << "XAXIS" << "YAXIS" << "ZAXIS";

    AddCaption(Caption);

    for (qint32 i = 0; i < 3; i++) {
        AddNewLine();
        AddModule(Captions[i], Identifier + "_" + Identifiers[i], "Stepper motor");
    }
}

/****************************************************************************/
/*!
 *  \brief Adds an assembly group to the panel
 *
 *  \iparam Caption = Caption of the section
 *  \iparam Identifier = Identifier prefix of the section
 */
/****************************************************************************/
void CCounters::AddModule(QString Caption, QString Identifier)
{
    AddCaption(Caption);
    AddTextItem(Identifier + "_HOURS", "Total operating hours:", "n/a");
}

/****************************************************************************/
/*!
 *  \brief Adds an assembly group to the panel
 *
 *  \iparam Caption = Caption of the section
 *  \iparam Identifier = Identifier prefix of the section
 *  \iparam Additional = Caption for an additional counter
 */
/****************************************************************************/
void CCounters::AddModule(QString Caption, QString Identifier, QString Additional)
{
    AddCaption(Caption);
    AddTextItem(Identifier + "_HOURS", "Total operating hours:", "n/a");
    AddTextItem(Identifier + "_ADD", Additional + " hours:", "n/a");
}

} // end namespace BasicScreens
