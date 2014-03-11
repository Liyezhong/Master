/****************************************************************************/
/*! \file Counters.h
 *
 *  \brief Counters definition.
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

#ifndef BASICSCREENS_COUNTERS_H
#define BASICSCREENS_COUNTERS_H

#include "MainMenu/Include/TextPanel.h"

namespace BasicScreens {

/****************************************************************************/
/**
 * \brief This class displays the life cycle counter of the device.
 */
/****************************************************************************/
class CCounters : public MainMenu::CTextPanel
{
    Q_OBJECT

public:
    explicit CCounters(QWidget *p_Parent = 0);

private:
    void AddGripperArm(QString Caption, QString Identifier);
    void AddModule(QString Caption, QString Identifier);
    void AddModule(QString Caption, QString Identifier, QString Additional);
};

} // end namespace BasicScreens

#endif // BASICSCREENS_COUNTERS_H
