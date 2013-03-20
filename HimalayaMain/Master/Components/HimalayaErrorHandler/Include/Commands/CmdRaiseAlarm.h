/****************************************************************************/
/*! \file CmdRaiseAlarm.h
 *
 *  \brief Definition file for class CmdRaiseAlarm
 *
 *  $Version:   $ 0.1
 *  $Date:      $ 2013-3-4
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

#ifndef HIMALAYAERRORHANDLER_CMDRAISEALARM_H
#define HIMALAYAERRORHANDLER_CMDRAISEALARM_H

#include <Global/Include/Commands/Command.h>

namespace HimalayaErrorHandler {

class CmdRaiseAlarm : public Global::Command {
public:
    static QString NAME;    ///< Command name.
    /****************************************************************************/
    CmdRaiseAlarm(bool isLocalAlarm);
    ~CmdRaiseAlarm();
    virtual QString GetName() const;

private:
    CmdRaiseAlarm(const CmdRaiseAlarm &);                       ///< Not implemented.
    const CmdRaiseAlarm & operator = (const CmdRaiseAlarm &);   ///< Not implemented.

public:
     bool m_localAlarm;


}; // end class CmdRaiseAlarm

}// end namespace HimalayaErrorHandler
#endif // HIMALAYAERRORHANDLER_CMDRAISEALARM_H
