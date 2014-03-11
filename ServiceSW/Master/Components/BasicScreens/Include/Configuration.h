/****************************************************************************/
/*! \file Configuration.h
 *
 *  \brief Configuration definition.
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

#ifndef BASICSCREENS_CONFIGURATION_H
#define BASICSCREENS_CONFIGURATION_H

#include "MainMenu/Include/TextPanel.h"

namespace BasicScreens {

/****************************************************************************/
/**
 * \brief This is the text panel displaying basic device information.
 */
/****************************************************************************/
class CConfiguration : public MainMenu::CTextPanel
{
    Q_OBJECT

public:
    explicit CConfiguration(QWidget *p_Parent = 0);
};

} // end namespace BasicScreens

#endif // BASICSCREENS_CONFIGURATION_H
