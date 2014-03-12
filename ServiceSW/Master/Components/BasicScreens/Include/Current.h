/****************************************************************************/
/*! \file Current.h
 *
 *  \brief Current definition.
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

#ifndef BASICSCREENS_CURRENT_H
#define BASICSCREENS_CURRENT_H

#include "BasicScreens/Include/AnalogPanel.h"
#include <QLabel>

namespace BasicScreens {

/****************************************************************************/
/**
 * \brief This class displays the electric currents measured in the device.
 */
/****************************************************************************/
class CCurrent : public BasicScreens::CAnalogPanel
{
    Q_OBJECT

public:
    explicit CCurrent(QWidget *p_Parent = 0);

private:
    void AddModule(QString Caption, QString Identifier);
};

} // end namespace BasicScreens

#endif // BASICSCREENS_CURRENT_H
