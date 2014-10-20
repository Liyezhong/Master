/****************************************************************************/
/*! \file LaSystem.h
 *
 *  \brief Declaration of Liquid and air system test.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2013-05-27
 *   $Author:  $ R.Wu
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

#ifndef DIAGNOSTICS_LASYSTEM_H
#define DIAGNOSTICS_LASYSTEM_H

#include <QWidget>
#include "Diagnostics/Include/DiagnosticMessageDlg.h"

namespace Diagnostics {

namespace Ui {
class CLaSystem;
}

class CLaSystem : public QWidget
{
    Q_OBJECT
    
public:
    /****************************************************************************/
    /*!
     *  \brief Constructor
     *  \iparam p_Parent = parent widget
     */
    /****************************************************************************/
    explicit CLaSystem(QWidget *parent = 0);
    /****************************************************************************/
    /*!
     *  \brief Deconstructor
     */
    /****************************************************************************/
    ~CLaSystem();

protected:
    /****************************************************************************/
    /*!
     *  \brief When event has changed, this function will be called to do some event action.
     *  \iparam p_Event = Upcoming event
     */
    /****************************************************************************/
    void changeEvent(QEvent *p_Event);

private Q_SLOTS:
    void StartLiquidHeatingTubeTest(void);
    void StartAirHeatingTubeTest(void);
    
private:
    Ui::CLaSystem *ui; //!< L & A ui
    CDiagnosticMessageDlg *dlg; //!< For show message dialog
};


} // namespace Diagnostics
#endif // DIAGNOSTICS_LASYSTEM_H
