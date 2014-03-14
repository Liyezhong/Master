/****************************************************************************/
/*! \file WrapperFmAnalogInput.h
 *
 *  \brief  Definition file for class WrapperFmAnalogInput
 *
 *  \version  0.1
 *  \date     2012-04-03
 *  \author   Jerin Antony
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

#ifndef WRAPPERFMANALOGINPUT_H
#define WRAPPERFMANALOGINPUT_H

#include <QObject>
#include <QEventLoop>
#include "WrapperBase.h"
#include "DeviceControl/Include/SlaveModules/AnalogInput.h"

using namespace DeviceControl;

/****************************************************************************/
/*! \brief Wrapper for Analog Input function modules
 *
 *  This wrapper class provides a QtScript interface to Analog Input function
 *  modules.
 *
 *  Examples:
 *   \dontinclude analoginput.js
 *   \skipline [WrapperFmAnalogInput]
 *   \until    [WrapperFmAnalogInput]
 */
/****************************************************************************/
class WrapperFmAnalogInput : public WrapperBase
{
    Q_OBJECT

public:
    WrapperFmAnalogInput(QString Name, CAnalogInput *pAnalogInput, QObject *pParent);

public slots:
    bool SetState(bool Enable);
    qint16 GetValue();
    void Reset();

signals:
    /****************************************************************************/
    /*! \brief Signal which is emitted after input has changed
     *
     *  Examples:
     *   \dontinclude AnalogInput.js
     *   \skipline [AnalogIn.ValueChanged]
     *   \until    [AnalogIn.ValueChanged]
     */
    /****************************************************************************/
    void ValueChanged(quint16);

private slots:
    void OnSetState(quint32 /*InstanceID*/, ReturnCode_t ReturnCode);
    void OnGetValue(quint32 /*InstanceID*/, ReturnCode_t ReturnCode, quint16 InputValue);

private:
    CAnalogInput *m_pAnalogInput;   //!< Pointer to CAnalogInput

    QEventLoop m_LoopSetState;      //!< Loop for blocking commands
    QEventLoop m_LoopGetValue;      //!< Loop for blocking commands

    bool m_State;                   //!< Enable/disable flag
    quint16 m_CurrentInputValue;    //!< Analog input value
};

#endif // WRAPPERFMANALOGINPUT_H
