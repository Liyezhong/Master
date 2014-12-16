/****************************************************************************/
/*! \file WrapperBase.h
 *
 *  \brief  Definition file for class WrapperBase
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

#ifndef WRAPPERBASE_H
#define WRAPPERBASE_H

#include <QObject>
#include <QStringList>
#include "DeviceControl/Include/Global/DeviceControlGlobal.h"
#include "DeviceControl/Include/SlaveModules/Module.h"

using namespace DeviceControl;

/****************************************************************************/
/*!  \brief Base functionality for Utility, Device and Function Module wrappers.
 *
 *   The wrappers provide a QtScript interface to the DeviceControl layer.
 */
/****************************************************************************/
class WrapperBase : public QObject
{
    Q_OBJECT

public:
    /****************************************************************************/
    /*!
     *  \brief  Constructor
     *
     *  \iparam Name = Name of the device for logging and the QtScript identifier
     *  \iparam Parent = Pointer to parent for QObject hierarchy and auto-delete
     *  \iparam p_Module = Slave module used by the wrapper
     */
    /****************************************************************************/
    inline WrapperBase(const QString& Name, QObject *Parent, const CModule *p_Module = NULL):
        QObject(Parent), m_Name(Name), m_LastErrors(), m_ErrorQueueSize(50), mp_Module(p_Module) {};

    /****************************************************************************/
    /*!
     *  \brief  Destructor
     */
    /****************************************************************************/
    virtual ~WrapperBase() { mp_Module = NULL; }

    /****************************************************************************/
    /*!
     *   \brief  getter for device name
     *    Getter is used by connection of wrappers to QScriptEngine
     *    and internally.
     *
     *   \return device name
     */
    /****************************************************************************/
    inline const QString &GetName() const { return m_Name; }

    /****************************************************************************/
    /*!
     *   \brief  Returns the Slave module the wrapper is representing
     *
     *   \return Pointer to the module or NULL in case of a device
     */
    /****************************************************************************/
    const CModule *GetSlaveModule() const { return mp_Module; }

    /****************************************************************************/
    /*!
     *   \brief  Can be used to disconnect all signals connected by a script
     *
     *   \return True in case of success, else false
     */
    /****************************************************************************/
    virtual bool DisconnectSignals() { return true; }

    /****************************************************************************/
    /*!
     *   \brief  get name by instance id
     *   \param  instanceID = request id
     *   \return name
     */
    /****************************************************************************/
    QString GetNameByInstanceID(quint32 instanceID);
public slots:
    void Log(const QString &);

    /****************************************************************************/
    /*!  \brief  Script-API: get last errors
     *   \return last errors as string, empty string if there are no errors 
     */
    /****************************************************************************/
    inline QString GetLastErrors() { 
        return m_LastErrors.join("\n"); 
    }

    /****************************************************************************/
    /*!  \brief  Script-API: clears list of last errors
     */
    /****************************************************************************/
    inline void ResetLastErrors() { m_LastErrors.clear(); }

    /****************************************************************************/
    /*!  \brief  Script-API: get size of error queue
     *   \return size of error queue
     */
    /****************************************************************************/
    inline qint32 GetErrorQueueSize() { return m_ErrorQueueSize; }

    void SetErrorQueueSize(qint32 Size);
    //void ShowProperties();

signals:
    /****************************************************************************/
    /*!
     *   \brief  Signal: send log message to outer handler.
     */
    /****************************************************************************/
    void LogMessageAdded(const QString &);

protected:
    bool HandleErrorCode(DeviceControl::ReturnCode_t ReturnCode);

private slots:
    //void OnError(quint32 ErrorCode, quint16 ErrorData, QDateTime ErrorTime);
    void OnError(quint32 InstanceID, quint16 ErrorGroup, quint16 ErrorCode, quint16 ErrorData, QDateTime ErrorTime);

private:
    void TruncateErrorQueue();
    QString m_Name;          //!< Name of the Device or FunctionModule.
    QStringList m_LastErrors; //!< Last error message from DeviceControl layer.
    qint32 m_ErrorQueueSize; //!< Size of error list. 
    const CModule *mp_Module;   //!< The module to be wrapped
};

#endif

// vi: set ts=4 sw=4 et:

