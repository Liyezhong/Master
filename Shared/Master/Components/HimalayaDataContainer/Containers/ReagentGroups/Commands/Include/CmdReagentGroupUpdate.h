/****************************************************************************/
/*! \file CmdReagentGroupUpdate.h
 *
 *  \brief Update reagent group command definition.
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

#ifndef MSGCLASSES_CMDREAGENTGROUPUPDATE_H
#define MSGCLASSES_CMDREAGENTGROUPUPDATE_H

#include "HimalayaDataContainer/Containers/ContainerBase/Commands/Include/CommandBase.h"

namespace MsgClasses {

/****************************************************************************/
/*!
 *  \brief  Command for updating a existing reagent group.
 *          The content should be in xml format:
 *          <ReagentGroup ID="***" Color="******"/>
 *          Attributes ID and Color are mandatory
 *          ID value should be valid Reagent Group ID to be updated,
 *          and Color should be a Hex number.
 */
/****************************************************************************/
class CmdReagentGroupUpdate : public CommandBase
{
public:
    CmdReagentGroupUpdate(int, const QDataStream &);
    CmdReagentGroupUpdate(void);

    ~CmdReagentGroupUpdate(void);

    virtual QString GetName(void) const;

    static QString NAME;

private:
    CmdReagentGroupUpdate(const CmdReagentGroupUpdate &);
    const CmdReagentGroupUpdate &operator = (const CmdReagentGroupUpdate &);

}; // end class CmdReagentGroupUpdate

} // end namespace MsgClasses

#endif // MSGCLASSES_CMDREAGENTGROUPUPDATE_H
