/*! \file WrapperFmDigitalOutput.h
 *
 *  \brief  Definition file for class WrapperFmDigitalOutput
 *
 *  \version  0.1
 *  \date     2012-04-04
 *  \author   Suhasini S
 *
 *  \b Description:
 *
 *   N/A
 *
 *  \b Company:
 *
 *       Leica Biosystems Nussloch GmbH.
 *
 *  (C) Copyright 2012 by Leica Biosystems Nussloch GmbH. All rights reserved
 *  This is unpublished proprietary source code of Leica. The copyright notice
 *  does not evidence any actual or intended publication.
 */
/****************************************************************************/
#ifndef WRAPPERFMDIGITALOUTPUT_H
#define WRAPPERFMDIGITALOUTPUT_H

#include <QObject>
#include <QEventLoop>
#include "WrapperBase.h"
#include "DeviceControl/Include/SlaveModules/DigitalOutput.h"

using namespace DeviceControl;

/****************************************************************************/
/*! \brief Wrapper for Digital output function module
 *
 *  This wrapper class provides a QtScript interface to digital output function
 *  modules.
 *
 *  Examples:
 *   \dontinclude DigitalOutput.js
 *   \skipline [WrapperFmDigitalOutput]
 *   \until    [WrapperFmDigitalOutput]
 */
/****************************************************************************/
class WrapperFmDigitalOutput : public WrapperBase
{
    Q_OBJECT

public:
    WrapperFmDigitalOutput(QString Name, CDigitalOutput *pDigitalOutput, QObject *pParent);

public slots:
    bool SetHigh();
    bool SetLow();
    bool SetValue(quint16 OutputValue, quint16 Duration = 0, quint16 Delay = 0);
    quint16 GetValue();
    void Reset();

private slots:
    void OnSetOutputValue(quint32, ReturnCode_t, quint16);
    void OnGetOutputValue(quint32, ReturnCode_t, quint16);

private:
    QEventLoop m_LoopSetOutputValue; //!< Loop for blocking commands
    QEventLoop m_LoopGetOutputValue; //!< Loop for blocking commands

    CDigitalOutput *m_pDigitalOutput;    //!< Pointer to the Digital output function module

    qint16 m_TargetOutputValue;     //!< Target output value; for verification of action result
    qint16 m_CurrentOutputValue;    //!< Current output value
};
#endif // WRAPPERFMANALOGINPUT_H
