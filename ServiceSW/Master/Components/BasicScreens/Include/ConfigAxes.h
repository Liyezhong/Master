/****************************************************************************/
/*! \file ConfigAxes.h
 *
 *  \brief ConfigAxes definition.
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

#ifndef BASICSCREENS_CONFIGAXES_H
#define BASICSCREENS_CONFIGAXES_H

#include "MainMenu/Include/TextPanel.h"

namespace BasicScreens {

/****************************************************************************/
/**
 * \brief This widget diplays basic information about all assembly groups
 *        involved in the gripper arms.
 */
/****************************************************************************/
class CConfigAxes : public MainMenu::CTextPanel
{
    Q_OBJECT

public:
    explicit CConfigAxes(QWidget *p_Parent = 0);

private:
    void AddGripperArm(QString Caption, QString Identifier);
};

} // end namespace BasicScreens

#endif // BASICSCREENS_CONFIGAXES_H
