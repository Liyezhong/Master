/****************************************************************************/
/*! \file Diagnostics/Include/MainControl.h
 *
 *  \brief Declaration of Main Control test.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2014-09-09
 *   $Author:  $ Dixiong Li
 *
 *  \b Company:
 *
 *       Leica Biosystems R&D Center Shanghai.
 *
 *  (C) Copyright 2010 by LBS R&D Center Shanghai. All rights reserved.
 *  This is unpublished proprietary source code of Leica. The copyright notice
 *  does not evidence any actual or intended publication.
 *
 */
/****************************************************************************/

#ifndef DIAGNOSTICS_MAINCONTROL_H
#define DIAGNOSTICS_MAINCONTROL_H

#include <QWidget>
#include "MainMenu/Include/MainWindow.h"

namespace Diagnostics {

namespace Ui {
class CMainControl;
}

/****************************************************************************/
/**
 * \brief This widget provides options to perform Main Control module tests
 */
/****************************************************************************/
class CMainControl : public QWidget
{
    Q_OBJECT

public:
    /****************************************************************************/
    /*!
     *  \brief Constructor
     *  \iparam p_Parent = Parent widget
     */
    /****************************************************************************/
    explicit CMainControl(QWidget *p_Parent = 0);

    /****************************************************************************/
    /*!
     *  \brief Destructor
     */
    /****************************************************************************/
    ~CMainControl();

signals:
    /****************************************************************************/
    /*!
     *  \brief Set gui tab click status
     *  \iparam Enable = flag of enable
     */
    /****************************************************************************/
    void SetGUITabEnable(bool Enable);

protected:
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function changeEvent
     *  \param p_Event = QEvent type parameter
     *  \return from changeEvent
     */
    /****************************************************************************/
    void changeEvent(QEvent *p_Event);

private Q_SLOTS:
    /****************************************************************************/
    /*!
     *  \brief Slot for ASB3 test
     */
    /****************************************************************************/
    void StartASB3Test(void);

    /****************************************************************************/
    /*!
     *  \brief Slot for ASB5 test
     */
    /****************************************************************************/
    void StartASB5Test(void);

    /****************************************************************************/
    /*!
     *  \brief Slot for ASB15 test
     */
    /****************************************************************************/
    void StartASB15Test(void);

private:
    Ui::CMainControl *ui;   //!< User Interface
};


} // namespace Diagnostics
#endif // DIAGNOSTICS_MAINCONTROL_H
