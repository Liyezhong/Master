/****************************************************************************/
/*! \file Diagnostics/Include/Oven.h
 *
 *  \brief Declaration of Oven test.
 *
 *   $Version: $ 1.0
 *   $Date:    $ 2014-10-14
 *   $Author:  $ Arthur Li
 *
 *  \b Company:
 *
 *       Leica Biosystems R&D Center Shanghai.
 *
 *  (C) Copyright 2014 by LBS R&D Center Shanghai. All rights reserved.
 *  This is unpublished proprietary source code of Leica. The copyright notice
 *  does not evidence any actual or intended publication.
 *
 */
/****************************************************************************/

#ifndef DIAGNOSTICS_OVEN_H
#define DIAGNOSTICS_OVEN_H

#include <QWidget>
#include "Diagnostics/Include/DiagnosticMessageDlg.h"
#include "Diagnostics/Include/Oven/OvenHeatingTestEmpty.h"

namespace Diagnostics {

namespace Ui {
class COven;
}

/****************************************************************************/
/**
 * \brief This widget provides options to perform Oven module tests
 */
/****************************************************************************/
class COven : public QWidget
{
    Q_OBJECT

public:
    /****************************************************************************/
    /*!
     *  \brief Constructor
     *  \iparam p_Parent = parent widget
     */
    /****************************************************************************/
    explicit COven(QWidget *p_Parent = 0);
    /****************************************************************************/
    /*!
     *  \brief Deconstructor
     */
    /****************************************************************************/
    ~COven();

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
     *  \brief When event has changed, this function will be called to do some event action.
     *  \iparam p_Event = Upcoming event
     */
    /****************************************************************************/
    void changeEvent(QEvent *p_Event);

private Q_SLOTS:
    /****************************************************************************/
    /*!
     *  \brief To run heating test empty test case.
     */
    /****************************************************************************/
    void StartHeatingTestEmpty(void);
    /****************************************************************************/
    /*!
     *  \brief To run cover sensor test case.
     */
    /****************************************************************************/
    void StartCoverSensorTest(void);

private:
    Ui::COven *ui; //!< Oven ui
    CDiagnosticMessageDlg *dlg; //!< For show message dialog
};


} // namespace Diagnostics
#endif // DIAGNOSTICS_OVEN_H
