/****************************************************************************/
/*! \file AnalogPanel.cpp
 *
 *  \brief AnalogPanel implementation.
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

#include "BasicScreens/Include/AnalogPanel.h"

namespace BasicScreens {

/****************************************************************************/
/*!
 *  \brief Constructor
 *
 *  \iparam p_Parent = Parent widget
 */
/****************************************************************************/
CAnalogPanel::CAnalogPanel(QWidget *p_Parent) : MainMenu::CTextPanel(p_Parent)
{

}

/****************************************************************************/
/*!
 *  \brief Destructor
 */
/****************************************************************************/
CAnalogPanel::~CAnalogPanel()
{
    try {
        QMap<QString, QLabel *>::const_iterator i;

        i = m_Status.constBegin();
        while (i != m_Status.constEnd()) {
            delete i.value();
            ++i;
        }
    }
    catch (...) {}
}

/****************************************************************************/
/*!
 *  \brief Adds an item at the bottom of the existing elements
 *
 *  \iparam Identifier = Identifier to access an analog value
 *  \iparam Description = Text description of the item
 *  \iparam Value = Initial analog value
 */
/****************************************************************************/
void CAnalogPanel::AddAnalogItem(QString Identifier, QString Description, QString Value)
{
    QLabel *p_Status = new QLabel();
    QPixmap Pixmap(":/Resources/result_led_green.png");
    p_Status->setPixmap(Pixmap);

    AddTextItem(Identifier, Description, Value);

    mp_Layout->addWidget(p_Status, m_LineCounter - 1, 2);

    m_Status[Identifier] = p_Status;
}

} // end namespace BasicScreens
