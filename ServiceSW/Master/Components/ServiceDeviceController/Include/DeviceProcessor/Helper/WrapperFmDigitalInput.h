/****************************************************************************/
/*! \file WrapperFmDigitalInput.h
 *
 *  \brief  Definition file for class WrapperFmDigitalInput
 *
 *  \version  0.1
 *  \date     2012-04-03
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

#ifndef WRAPPERFMDIGITALINPUT_H
#define WRAPPERFMDIGITALINPUT_H

#include <QObject>
#include <QEventLoop>
#include "ServiceDeviceController/Include/DeviceProcessor/Helper/WrapperBase.h"
#include "DeviceControl/Include/SlaveModules/DigitalInput.h"

using namespace DeviceControl;


/****************************************************************************/
/*! \brief Wrapper for Digital Input function modules
 *
 *  This wrapper class provides a QtScript interface to digital input function
 *  modules.
 *
 *  Examples:
 *   \dontinclude DigitalInput.js
 *   \skipline [WrapperFmDigitalInput]
 *   \until    [WrapperFmDigitalInput]
 */
/****************************************************************************/
class WrapperFmDigitalInput : public WrapperBase
{
    Q_OBJECT

   // define the enabled property
   // Q_PROPERTY( bool enabled WRITE setEnabled READ isEnabled )

public:
    WrapperFmDigitalInput(QString Name, CDigitalInput *pDigitalInput, QObject *pParent);

public slots:
    qint32 GetValue();
    qint32 ReadEdgeCount();
    void Reset();

 signals:
    /****************************************************************************/
    /*! \brief Signal which is emitted after input has changed
     *
     *  Examples:
     *   \dontinclude DigitalInput.js
     *   \skipline [DigitalIn.ValueChanged]
     *   \until    [DigitalIn.ValueChanged]
     */
    /****************************************************************************/
    void ValueChanged(quint16);

private slots:
    void OnGetValue(quint32 /*InstanceID*/, ReturnCode_t ReturnCode, quint16 InputValue);

private:
    CDigitalInput *m_pDigitalInput;  //!< Pointer to the Digital Input function module
    QEventLoop m_LoopGetValue;  //!< Loop for blocking commands
    qint32 m_CurrentInputValue; //!< Current digital input value
    qint32 m_EdgeCount;         //!< Edge count (value change count for multi bit values)
};

#endif // WRAPPERFMDIGITALINPUT_H
