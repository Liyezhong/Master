/****************************************************************************/
/*! \file Diagnostics/Include/RotaryValve.h
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

/****************************************************************************/
/**
 * \brief This widget provides options to perform Rotary valve module tests
 */
/****************************************************************************/
class CRotaryValve : public QWidget
{
    Q_OBJECT
    
public:
    /****************************************************************************/
    /*!
     *  \brief Constructor
     *  \iparam p_Parent = parent widget
     */
    /****************************************************************************/
    explicit CRotaryValve(QWidget *p_Parent = 0);
    /****************************************************************************/
    /*!
     *  \brief Deconstructor
     */
    /****************************************************************************/
    ~CRotaryValve();

signals:
    /****************************************************************************/
    /*!
     *  \brief To set test button enable when movement test is pass
     */
    /****************************************************************************/
    void EnableTestButton();

    /****************************************************************************/
    /*!
     *  \brief Set gui tab click status
     *  \iparam Enable = flag of enable
     */
    /****************************************************************************/
    void SetGUITabEnable(bool Enable);

private Q_SLOTS:
    /****************************************************************************/
    /*!
     *  \brief To run movement test case.
     */
    /****************************************************************************/
    void StartMovementTest(void);

private:
    /****************************************************************************/
    /*!
     *  \brief Write result to service log file
     *  \iparam TestName = the name of test case
     *  \iparam RetError = test result
     */
    /****************************************************************************/
    void LogResult(QString& TestName, ErrorCode_t RetError);

    Ui::CRotaryValve *ui;  //!< Rotary valve ui
    CDiagnosticMessageDlg *mp_MessageDlg; //!< For show message dialog
};


} // namespace Diagnostics
#endif // DIAGNOSTICS_ROTARYVALVE_H
