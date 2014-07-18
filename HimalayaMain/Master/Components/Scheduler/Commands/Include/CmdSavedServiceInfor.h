/****************************************************************************/
/*! \file CmdSavedServiceInfor.h
 *
 *  \brief Definition file for class CmdSavedServiceInfor
 *
 *  $Version:   $ 0.1
 *  $Date:      $ 2014-7-12
 *  $Author:    $ Abe Yang
 *
 *  \b Company:
 *
 *       Leica Microsystems Ltd. Shanghai.
 *
 *  (C) Copyright 2013 by Leica Microsystems Shanghai. All rights reserved.
 *  This is unpublished proprietary source code of Leica. The copyright notice
 *  does not evidence any actual or intended publication.
 *
 */
/****************************************************************************/

#ifndef NETCOMMANDS_CMDSAVEDSERVICEINFOR_H
#define NETCOMMANDS_CMDSAVEDSERVICEINFOR_H

#include <Global/Include/Commands/Command.h>

namespace MsgClasses {
/****************************************************************************/
/*!
 *  \brief  Definition/Declaration of class CmdSavedServiceInfor
 */
/****************************************************************************/

class CmdSavedServiceInfor : public Global::Command {
public:
    static QString NAME;    ///< Command name.
    /****************************************************************************/
    CmdSavedServiceInfor(int Timeout, const QString& deviceType);
    ~CmdSavedServiceInfor();
    virtual QString GetName() const;
    const QString& DeviceType() const { return m_DeviceType; }
private:
    CmdSavedServiceInfor(const CmdSavedServiceInfor &);                       ///< Not implemented.
    const CmdSavedServiceInfor & operator = (const CmdSavedServiceInfor &);   ///< Not implemented.
    QString m_DeviceType;
}; // end class CmdSavedServiceInfor

}// end namespace NetCommands
#endif // NETCOMMANDS_CMDSAVEDSERVICEINFOR_H
