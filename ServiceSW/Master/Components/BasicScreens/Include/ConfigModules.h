/****************************************************************************/
/*! \file ConfigModules.h
 *
 *  \brief ConfigModules definition.
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

#ifndef BASICSCREENS_CONFIGMODULES_H
#define BASICSCREENS_CONFIGMODULES_H

#include "MainMenu/Include/TextPanel.h"

namespace BasicScreens {

/****************************************************************************/
/**
 * \brief This widget displays basic information of some Colorado assembly
 *        groups.
 */
/****************************************************************************/
class CConfigModules : public MainMenu::CTextPanel
{
    Q_OBJECT

public:
    explicit CConfigModules(QWidget *p_Parent = 0);

private:
    void AddModule(QString Caption, QString Identifier);
};

} // end namespace BasicScreens

#endif // BASICSCREENS_CONFIGMODULES_H
