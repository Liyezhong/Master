/****************************************************************************/
/*! \file System.h
 *
 *  \brief Declaration of System test.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2013-06-03
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

#ifndef DIAGNOSTICS_SYSTEM_H
#define DIAGNOSTICS_SYSTEM_H

#include <QWidget>
#include "Diagnostics/Include/DiagnosticMessageDlg.h"

namespace Diagnostics {

namespace Ui {
class CSystem;
}

/****************************************************************************/
/**
 * \brief This widget provides options to perform system module tests
 */
/****************************************************************************/
class CSystem : public QWidget
{
    Q_OBJECT
    
public:
    /****************************************************************************/
    /*!
     *  \brief Constructor
     *  \iparam p_Parent = Parent widget
     */
    /****************************************************************************/
    explicit CSystem(QWidget *p_Parent = 0);

    /****************************************************************************/
    /*!
     *  \brief Destructor
     */
    /****************************************************************************/
    ~CSystem();

private Q_SLOTS:
    /****************************************************************************/
    /*!
     *  \brief Slot for sealing test
     */
    /****************************************************************************/
    void StartSealingTest(void);

    /****************************************************************************/
    /*!
     *  \brief Slot for filling & draining test
     */
    /****************************************************************************/
    void StartFillingNDrainingTest(void);

    /****************************************************************************/
    /*!
     *  \brief Slot for liquid hose test
     */
    /****************************************************************************/
    void StartLiquidHoseTest(void);

    /****************************************************************************/
    /*!
     *  \brief Slot for speaker test
     */
    /****************************************************************************/
    void StartSpeakerTest(void);

    /****************************************************************************/
    /*!
     *  \brief Slot for local alarm test
     */
    /****************************************************************************/
    void StartLocalAlarmTest(void);

    /****************************************************************************/
    /*!
     *  \brief Slot for remote alarm test
     */
    /****************************************************************************/
    void StartRemoteAlarmTest(void);

    /****************************************************************************/
    /*!
     *  \brief Slot for exhaust fan test
     */
    /****************************************************************************/
    void StartExhaustFanTest(void);

    /****************************************************************************/
    /*!
     *  \brief Slot for ventilation fan test
     */
    /****************************************************************************/
    void StartVentilationFanTest(void);
    
private:
    Ui::CSystem *mp_Ui;                         //!< User Interface
    CDiagnosticMessageDlg*   mp_MessageDlg;     //!< The pointer message dialog
};


} // namespace Diagnostics
#endif // DIAGNOSTICS_SYSTEM_H
