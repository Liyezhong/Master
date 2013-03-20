/****************************************************************************/
/*! \file CmdStationResetData.h
 *
 *  \brief Reset station data command definition.
 *
 *  $Version:   $ 0.1
 *  $Date:      $ 2013-02-02
 *  $Author:    $ R. Wu
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

#ifndef MSGCLASSES_CMDSTATIONRESETDATA_H
#define MSGCLASSES_CMDSTATIONRESETDATA_H

#include "HimalayaDataContainer/Containers/ContainerBase/Commands/Include/CommandBase.h"

namespace MsgClasses {


class CmdStationResetData : public CommandBase
{
public:
    CmdStationResetData(int, const QDataStream &);
    CmdStationResetData(void);

    ~CmdStationResetData(void);

    virtual QString GetName(void) const;

    static QString NAME;

private:
    CmdStationResetData(const CmdStationResetData &);
    const CmdStationResetData &operator = (const CmdStationResetData &);

}; // end class CmdStationResetData

} // end namespace MsgClasses

#endif // MSGCLASSES_CMDSTATIONRESETDATA_H
