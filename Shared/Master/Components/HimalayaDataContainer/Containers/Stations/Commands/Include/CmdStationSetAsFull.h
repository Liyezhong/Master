/****************************************************************************/
/*! \file CmdStationSetAsFull.h
 *
 *  \brief Set the station as full command definition.
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

#ifndef MSGCLASSES_CMDSTATIONSETASFULL_H
#define MSGCLASSES_CMDSTATIONSETASFULL_H

#include "HimalayaDataContainer/Containers/ContainerBase/Commands/Include/CommandBase.h"

namespace MsgClasses {

/****************************************************************************/
/*!
 *  \brief  Command for setting the station as full.
 *          The content should be in xml format:
 *          <Station ID="XXX"/>
 *          Attribute ID is mandatory, and should be valid.
 *          this content will set station XXX as full;
 */
/****************************************************************************/
class CmdStationSetAsFull : public CommandBase
{
public:
    CmdStationSetAsFull(int, const QDataStream &);
    CmdStationSetAsFull(void);

    ~CmdStationSetAsFull(void);

    virtual QString GetName(void) const;

    static QString NAME;

private:
    CmdStationSetAsFull(const CmdStationSetAsFull &);
    const CmdStationSetAsFull &operator = (const CmdStationSetAsFull &);

}; // end class CmdStationSetAsFull

} // end namespace MsgClasses

#endif // MSGCLASSES_CMDSTATIONSETASFULL_H
