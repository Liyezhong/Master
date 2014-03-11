/****************************************************************************/
/*! \file AnalogPanel.h
 *
 *  \brief AnalogPanel definition.
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

#ifndef BASICSCREENS_ANALOGPANEL_H
#define BASICSCREENS_ANALOGPANEL_H

#include "MainMenu/Include/TextPanel.h"
#include <QLabel>

namespace BasicScreens {

/****************************************************************************/
/**
 * \brief This is a base class for all widgets displaying analog input
 *        values.
 */
/****************************************************************************/
class CAnalogPanel : public MainMenu::CTextPanel
{
    Q_OBJECT

public:
    explicit CAnalogPanel(QWidget *p_Parent = 0);
    virtual ~CAnalogPanel();

protected:
    void AddAnalogItem(QString Identifier, QString Description, QString Value);

private:
    QMap<QString, QLabel *> m_Status;   //!< List of the red or green status LEDs
};

} // end namespace BasicScreens

#endif // BASICSCREENS_ANALOGPANEL_H
