/****************************************************************************/
/*! \file CmdStationSetAsEmpty.h
 *
 *  \brief Set the station as empty command definition.
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

#ifndef MSGCLASSES_CMDSTATIONSETASEMPTY_H
#define MSGCLASSES_CMDSTATIONSETASEMPTY_H

#include "HimalayaDataContainer/Containers/ContainerBase/Commands/Include/CommandBase.h"

namespace MsgClasses {

/****************************************************************************/
/*!
 *  \brief  Command for setting the station as empty.
 *          The content should be in xml format:
 *          <Station ID="XXX"/>
 *          Attribute ID is mandatory, and should be valid.
 *          this content will set station XXX as emtpy;
 */
/****************************************************************************/
class CmdStationSetAsEmpty : public CommandBase
{
public:
    CmdStationSetAsEmpty(int, const QDataStream &);
    CmdStationSetAsEmpty(void);

    ~CmdStationSetAsEmpty(void);

    virtual QString GetName(void) const;

    static QString NAME;

private:
    CmdStationSetAsEmpty(const CmdStationSetAsEmpty &);
    const CmdStationSetAsEmpty &operator = (const CmdStationSetAsEmpty &);

}; // end class CmdStationSetAsEmpty

} // end namespace MsgClasses

#endif // MSGCLASSES_CMDSTATIONSETASEMPTY_H
