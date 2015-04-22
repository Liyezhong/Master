/****************************************************************************/
/*! \file WrapperBase.cpp
 *
 *  \brief  Implementation file for class WrapperBase
 *
 *  \version  0.1
 *  \date     2012-03-23
 *  \author   T.Scheck
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

#include <QtDebug>
#include <QDateTime>
#include <QMetaProperty>
#include <QTimer>
#include "ServiceDeviceController/Include/DeviceProcessor/Helper/WrapperBase.h"

/****************************************************************************/
/*!
 *   \brief Script-API: log a message to the connected output component.
 *
 *   This method currently adds timestamp and device name.
 *   \todo LogFacility and LogLevel for filtering needed.
 *
 *  \iparam   Message  String which shall be logged.
 *
 */
/****************************************************************************/
void WrapperBase::Log(const QString& Message)
{
#ifndef PRE_ALFA_TEST
    emit LogMessageAdded(
        QDateTime::currentDateTime().toString("hh:mm:ss.zzz")
        + " " + GetName() + ": " + Message
    );
#else
    emit LogMessageAdded(
        QDateTime::currentDateTime().toString("hh:mm:ss.zzz")
        + " " + Message
    );
#endif
}

/****************************************************************************/
/*!
 *  \brief  Handles the ReturnCode of the DeviceControl layer.
 *
 *  \iparam ReturnCode = ReturnCode of DeviceControl Layer
 *
 *  \return Returns false in case of an error, else true
 */
/****************************************************************************/
bool WrapperBase::HandleErrorCode(DeviceControl::ReturnCode_t ReturnCode)
{
    using namespace DeviceControl;

    QString msg = "";

    switch(ReturnCode)
    {
    case DCL_ERR_FCT_CALL_SUCCESS://0
        break;

    case DCL_ERR_FCT_CALL_FAILED://1
        msg = tr("Function was failed, no further information available");
        break;

    case DCL_ERR_FCT_CALL_NOT_FOUND://2
        msg = tr("Inside the function, a parameter concerning action was not done because anything was not found");
        break;

    case DCL_ERR_FCT_NOT_IMPLEMENTED://3
        msg = tr("A method was called which is not implemented or not applicable");
        break;

    case DCL_ERR_INVALID_STATE://10
        msg = tr("A method was called while the internal state machine was not able to process the functionality");
        break ;

    case DCL_ERR_NOT_INITIALIZED://11
        msg = tr("A method was called before complete initialization was done or initialization failed");
        break;

    case  DCL_ERR_TIMEOUT://12
        msg = tr("A timeout appeared (typically a CAN-Bus timeout, while waiting for an answer");
        break;

    case DCL_ERR_INVALID_PARAM://13
        msg = tr("One or more invalid parameters were passed to the method");
        break;

    case DCL_ERR_INTERNAL_ERR://14
        msg = tr("The error reason is internal");
        break;

    case DCL_ERR_EXTERNAL_ERROR://15
        msg = tr("The error reason is external, it was reported via CAN-bus");
        break;

    case DCL_ERR_NULL_PTR_ACCESS://16
        msg = tr("A null pointer would be accessed, but there was an instruction to avoid its direct access");
        break;

    case DCL_ERR_CANMSG_INVALID://20
        msg = tr("An invalid CAN-message was received (e.g. dlc not correct)");
        break;

    case DCL_ERR_CANBUS_NOT_READY: //21
        msg = tr("A CAN-message was tried to be send while the CAN bus is in error state");
        break;

    case DCL_ERR_CANBUS_ERROR://22
        msg = tr("A CAN-message was tried to be send while an 'unknown' CAN bus error is active");
        break;

    case DCL_ERR_UNDEFINED:
        msg = tr("The return code was not set");
        break;

    default:
        msg = tr("Undefined error code");
        break;
    }
    if (!msg.isEmpty()) {
        msg = msg.prepend("DCL ERROR ");
        Log(msg);
        m_LastErrors.append(GetName() + ": " + msg);
        TruncateErrorQueue();
        return false;
    } else {
        return true;
    }
}

/****************************************************************************/
/*!  \brief  Script-API: set size of error queue
 *           (oldest errors are removed first)
 *
 *   If Size is less than 1, the queue is set to 1.
 *
 *   \iparam  Size   number representing the error queue size
 */
/****************************************************************************/
void WrapperBase::SetErrorQueueSize(qint32 Size) 
{
    if (Size < 1) {
        Size = 1;
    }
    m_ErrorQueueSize = Size; 
    TruncateErrorQueue();
}


/****************************************************************************/
/*!
 *   \brief Truncate error queue to m_ErrorQueueSize
 */
/****************************************************************************/
void WrapperBase::TruncateErrorQueue() 
{
    while (m_LastErrors.size() > m_ErrorQueueSize) {
        m_LastErrors.removeFirst();
    }
}

/****************************************************************************/
/*!
 *  \brief  This slot is called when an error was received from the Slave
 *
 *  \iparam ErrorCode = Error code
 *  \iparam ErrorData = Error data
 *  \iparam ErrorTime = Error time
 */
/****************************************************************************/
void WrapperBase::OnError(quint32 InstanceID, quint16 ErrorGroup, quint16 ErrorCode, quint16 ErrorData, QDateTime ErrorTime)
{
    Log(
        QString() +
        "Instance ID: "  + QString().setNum(InstanceID) +
        "Error Group: "  + QString().setNum(ErrorGroup) +
        "Error Code: "   + QString().setNum(ErrorCode) +
        ", Error Data: " + QString().setNum(ErrorData) +
        ", Error Time: " + ErrorTime.toString()
    );
}

QString WrapperBase::GetNameByInstanceID(quint32 instanceID)
{
    QString name;

    name += tr("Board: %1 ").arg(instanceID & 0xFF);
    name += tr("Channel: %1 ").arg((instanceID & 0xFF0000)>>16);
    name += tr("Node index: %1 ").arg((instanceID & 0xFF00)>>8);

    return name;
}

void WrapperBase::SetEventLoopTimeOut(QTimer &timer, QEventLoop &p_EventLoop, quint32 MilliSeconds)
{
    timer.setSingleShot(true);
    timer.setInterval(MilliSeconds);
    timer.start();
    (void)connect(&timer, SIGNAL(timeout()), &p_EventLoop, SLOT(quit()));
}

/*
void WrapperBase::ShowProperties()
{
  const QMetaObject *metaobject = metaObject();
  int count = metaobject->propertyCount();
  for (int i=0; i<count; ++i) {
    QMetaProperty metaproperty = metaobject->property(i);
    const char *name = metaproperty.name();
    QVariant value = property(name);
    qDebug() << name \
      << " " << value \
      << " " << metaproperty.typeName() \
      << " " << metaproperty.type() \
      << " " << (metaproperty.isEnumType() ? metaproperty.enumerator().valueToKey(value.toInt()) : "") \
    ;
  }
}
*/


/****************************************************************************/
/*
 *  API documentation, which is commonly used in several methods, can
 *  be stored below in a separate page, for instance named PageName.
 *  The page contents can then be included with the command:
 *  \copydoc PageName
 */
/****************************************************************************/

/****************************************************************************/
/*!
 *   \page TimeoutIntro Introduction to the timeout concept of asynchronous calls
 *
 *   <b>Timeout concept of asynchronous calls</b>
 *
 *   For a script author most actions are quasi-atomic, i.e. they take less
 *   than 100ms to execute and should never block.
 *
 *   Some actions take several seconds to execute, though.
 *   In these cases a method provides an optional last parameter "Timeout".
 *
 *   Depending on the timeout value the behaviour of the call can be changed:
 *     - Timeout = -1 (default)
 *       - infinite timeout: The call blocks until the action is finished and
 *         returns true on success, else false.
 *       - Use case1: This is useful for sequentially executed actions.
 *     - Timeout = 0
 *       - no timeout: The call only initiates the action and
 *         - either returns true immediately (non-blocking) if the action was
 *           initiated successfully,
 *         - or returns false if there was an error.
 *       - Use case 2: Multiple actions shall be initiated at the same time.
 *         For example, two drawers shall be opened simultaneously.
 *       - Use case 3: During a long-term action, another action shall be called.
 *         For example, during a movement the actual position shall
 *         periodically be printed.
 *     - Timeout > 0
 *       - timeout in milli seconds (ms): The call initiates the action, blocks
 *         no longer than the given timeout and
 *         - either returns true if the action finishes before the timeout
 *           (no matter with which error status),
 *         - or returns false if the timeout is triggered before the
 *           action is finished.
 *       - Use case 4: See Use case 2 above.
 *
 *   The action status can be checked asynchronously with corresponding
 *   getter methods or a Wait() method which is specific to a class instance.
 *
 *   The Wait() method waits for all pending asynchronous actions of a class instance
 *   to finish. It also has an optional parameter "Timeout" which behaves as
 *   described above.
 */
/****************************************************************************/

/****************************************************************************/
/*!
 *   \page ErrorHandlingIntro Introduction to error handling
 *
 *   <b>Introduction to error handling</b>
 *
 *   \todo describe general error handling approach
 */
/****************************************************************************/

// vi: set ts=4 sw=4 et:

