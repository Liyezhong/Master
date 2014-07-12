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

class CResetData;

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

    //! Lifecycle time in minutes
    Q_PROPERTY(quint32 Lifetime READ GetLifetime)
    //! Number of on/off cycles
    Q_PROPERTY(quint32 Lifecycles READ GetLifecycles)

public:
    WrapperFmDigitalOutput(QString Name, CDigitalOutput *pDigitalOutput, QObject *pParent);
    ~WrapperFmDigitalOutput();

public slots:
    bool SetHigh();
    bool SetLow();
    bool SetValue(quint16 OutputValue, quint16 Duration = 0, quint16 Delay = 0);
    quint16 GetValue();
    bool GetLifetimeData();
    bool ReqDataReset();
    void Reset();
    /****************************************************************************/
    /*!
     *  \brief  Getter for the lifecycles property
     *
     *  \return Lifecycles
     */
    /****************************************************************************/
    quint32 GetLifecycles();

    /****************************************************************************/
    /*!
     *  \brief  Getter for the lifetime property
     *
     *  \return Lifetime in minutes
     */
    /****************************************************************************/
    quint32 GetLifetime();

private slots:
    void OnSetOutputValue(quint32, ReturnCode_t, quint16);
    void OnGetOutputValue(quint32, ReturnCode_t, quint16);
    void OnGetLifetimeData(quint32, ReturnCode_t, quint32, quint32);

private:
    QEventLoop m_LoopSetOutputValue; //!< Loop for blocking commands
    QEventLoop m_LoopGetOutputValue; //!< Loop for blocking commands
    QEventLoop m_LoopGetLifetimeData;   //!< Loop for blocking commands

    CDigitalOutput *m_pDigitalOutput;    //!< Pointer to the Digital output function module
    CResetData *mp_ResetData;           //!< Support for the reset data command

    qint16 m_TargetOutputValue;     //!< Target output value; for verification of action result
    qint16 m_CurrentOutputValue;    //!< Current output value
    quint32 m_Lifetime;     //!< Active time in minutes
    quint32 m_Lifecycles;   //!< Number of times switched
};
#endif // WRAPPERFMANALOGINPUT_H
