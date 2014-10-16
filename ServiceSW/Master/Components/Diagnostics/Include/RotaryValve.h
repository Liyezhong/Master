/****************************************************************************/
/*! \file RotaryValve.h
 *
 *  \brief Declaration of RotaryValve test.
 *
 *   $Version: $ 0.2
 *   $Date:    $ 2014-10-11
 *   $Author:  $ Arthur Li
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

#ifndef DIAGNOSTICS_ROTARYVALVE_H
#define DIAGNOSTICS_ROTARYVALVE_H

#include <QWidget>
#include "Diagnostics/Include/DiagnosticMessageDlg.h"

namespace Diagnostics {

namespace Ui {
class CRotaryValve;
}

class CRotaryValve : public QWidget
{
    Q_OBJECT
    
public:
    /****************************************************************************/
    /*!
     *  \brief Constructor
     *  \iparam parent = parent widget
     */
    /****************************************************************************/
    explicit CRotaryValve(QWidget *parent = 0);
    /****************************************************************************/
    /*!
     *  \brief Deconstructor
     */
    /****************************************************************************/
    ~CRotaryValve();

private Q_SLOTS:
    /****************************************************************************/
    /*!
     *  \brief To run movement test case.
     */
    /****************************************************************************/
    void StartMovementTest(void);

private:
    Ui::CRotaryValve *ui;  //!< Rotary valve ui
    CDiagnosticMessageDlg *mp_MessageDlg; //!< For show message dialog
};


} // namespace Diagnostics
#endif // DIAGNOSTICS_ROTARYVALVE_H
