/****************************************************************************/
/*! \file Himalaya/HimalayaMain/Gui/Components/Dashboard/Include/PausePushButton.h
 *
 *  \brief Header file for class CPausePushButton.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2014-03-12
 *   $Author:  $ A.Yang
 *
 *  \b Company:
 *
 *       Leica Microsystems Ltd. Shanghai.
 *
 *  (C) Copyright 2014 by Leica Microsystems Shanghai. All rights reserved.
 *  This is unpublished proprietary source code of Leica. The copyright notice
 *  does not evidence any actual or intended publication.
 *
 */
/****************************************************************************/

#ifndef DASHBOARD_PAUSEPUSHBUTTON_H
#define DASHBOARD_PAUSEPUSHBUTTON_H

#include <QPushButton>

namespace Dashboard {
  

/****************************************************************************/
/**
 * \brief This is a customized push button for Himalaya Project.
 */
/****************************************************************************/
class CPausePushButton : public QPushButton
{
    Q_OBJECT

public:
    explicit CPausePushButton(QWidget *p_Parent = 0);
    virtual ~ CPausePushButton();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function setText
     *
     *  \return from setText
     */
    /****************************************************************************/
    void setText(const QString &text) { Q_UNUSED(text); };
private:
    void paintEvent(QPaintEvent *);   
};

} // end namespace Dashboard

#endif // DASHBOARD_PAUSEPUSHBUTTON_H

