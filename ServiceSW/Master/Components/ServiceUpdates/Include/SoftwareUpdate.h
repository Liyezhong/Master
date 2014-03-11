/****************************************************************************/
/*! \file SoftwareUpdate.h
 *
 *  \brief
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2013-02-08
 *   $Author:  $ Soumya. D
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

#ifndef SERVICEUPDATES_SOFTWAREUPDATE_H
#define SERVICEUPDATES_SOFTWAREUPDATE_H

#include "MainMenu/Include/TextPanel.h"

namespace ServiceUpdate {

/****************************************************************************/
/**
 * \brief This widget displays history of ModuleList
 */
/****************************************************************************/
class CSoftwareUpdate : public MainMenu::CTextPanel
{
    Q_OBJECT

public:
    explicit CSoftwareUpdate(QWidget *p_Parent = 0);

};

} // end namespace ServiceUpdate

#endif //SERVICEUPDATES_SOFTWAREUPDATE_H
