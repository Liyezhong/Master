/****************************************************************************/
/*! \file System.h
 *
 *  \brief CSystem class definition file
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2014-07-16
 *   $Author:  $ Dixiong Li
 *
 *  \b Company:
 *
 *       Leica Biosystems Shanghai.
 *
 *  (C) Copyright 2010 by Leica Biosystems Shanghai.. All rights reserved.
 *  This is unpublished proprietary source code of Leica. The copyright notice
 *  does not evidence any actual or intended publication.
 *
 */
/****************************************************************************/

#ifndef SERVICEUPDATES_SYSTEM_H
#define SERVICEUPDATES_SYSTEM_H

#include <QWidget>

#include "MainMenu/Include/MessageDlg.h"


namespace ServiceUpdates {

namespace Ui {
    class CServiceSystem;
}
/****************************************************************************/
/**
 * \brief
 */
/****************************************************************************/
class CSystem : public QWidget
{
    Q_OBJECT

public:
    /****************************************************************************/
    /**
     * \brief  Default constructor.
     * \iparam parent = parent of widget
     */
    /****************************************************************************/
    explicit CSystem(QWidget *p_Parent = 0);

    /****************************************************************************/
    /**
     * \brief Destructor.
     */
    /****************************************************************************/
    ~CSystem(void);

private Q_SLOTS:

    /****************************************************************************/
    /**
     * \brief Slot for shut down.
     */
    /****************************************************************************/
    void OnShutDown(void);

signals:
    /*******************************************************************************/
    /*!
     *  \brief Signal emitted to shut down system
     */
    /*******************************************************************************/
    void ShutdownSystem(bool NeedUpdate=true);

private:
    Ui::CServiceSystem   *mp_Ui;            //!< User Interface

    MainMenu::CMessageDlg *mp_MessageDlg;   //!< Information dialog
};

} // end namespace ServiceUpdates

#endif //SERVICEUPDATES_SHUTDOWN_H
