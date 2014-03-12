/****************************************************************************/
/*! \file Touchscreen.cpp
 *
 *  \brief Touchscreen implementation.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2011-07-13
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

#include "Calibration/Include/Touchscreen.h"
#include "ui_Touchscreen.h"

namespace Calibration {

/****************************************************************************/
/*!
 *  \brief Constructor
 *
 *  \iparam p_Parent = Parent widget
 */
/****************************************************************************/
CTouchscreen::CTouchscreen(QWidget *p_Parent) : QWidget(p_Parent), mp_Ui(new Ui::CTouchscreen)
{
    mp_Ui->setupUi(this);
}

/****************************************************************************/
/*!
 *  \brief Destructor
 */
/****************************************************************************/
CTouchscreen::~CTouchscreen()
{
    try {
        delete mp_Ui;
    }
    catch (...) {}
}

} // end namespace Calibration
