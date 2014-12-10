/****************************************************************************/
/*! \file Retort.h
 *
 *  \brief Declaration of Retort test.
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

#ifndef DIAGNOSTICS_RETORT_H
#define DIAGNOSTICS_RETORT_H

#include <QWidget>
#include "Diagnostics/Include/DiagnosticMessageDlg.h"

namespace Diagnostics {

namespace Ui {
class CRetort;
}

/****************************************************************************/
/**
 * \brief This widget provides options to perform Retort module tests
 */
/****************************************************************************/
class CRetort : public QWidget
{
    Q_OBJECT
    
public:
    /****************************************************************************/
    /*!
     *  \brief Constructor
     *  \iparam p_Parent = parent widget
     */
    /****************************************************************************/
    explicit CRetort(QWidget *p_Parent = 0);

    /****************************************************************************/
    /*!
     *  \brief Deconstructor
     */
    /****************************************************************************/
    ~CRetort();

public slots:
    /****************************************************************************/
    /*!
     *  \brief This slot is for enable test button when movement test is pass
     */
    /****************************************************************************/
    void OnEnableTestButton();

protected:
    /****************************************************************************/
    /*!
     *  \brief When event has changed, this function will be called to do some event action.
     *  \iparam p_Event = Upcoming event
     */
    /****************************************************************************/
    void changeEvent(QEvent *p_Event);

private Q_SLOTS:
    void StartLidLockTest(void);
    void StartDrainReagentTest(void);
    void StartLevelSensorDetectionTest(void);
    void StartHeatingTestEmpty(void);
    void StartHeatingTestWithWater(void);
    
private:
    Ui::CRetort *mp_Ui;                         //!< User Interface
    CDiagnosticMessageDlg*   mp_MessageDlg;     //!< The pointer message dialog

};


} // namespace Diagnostics
#endif // DIAGNOSTICS_RETORT_H
