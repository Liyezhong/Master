/****************************************************************************/
/*! \file AddModifyConfiguration.h
 *
 *  \brief Add/Modify Configuration definition.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2013-01-16
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

#ifndef SYSTEMTRACKING_ADDMODIFYCONFIGURATION_H
#define SYSTEMTRACKING_ADDMODIFYCONFIGURATION_H

#include "MainMenu/Include/TextPanel.h"

namespace SystemTracking {

/****************************************************************************/
/**
 * \brief This class provides facility to add/modify configuration
 */
/****************************************************************************/
class CAddModifyConfig : public MainMenu::CTextPanel
{
    Q_OBJECT

public:
    explicit CAddModifyConfig(QWidget *p_Parent = 0);

};

} // end namespace SystemTracking

#endif //SYSTEMTRACKING_ADDMODIFYCONFIGURATION_H
