/****************************************************************************/
/*! \file CmdLeicaConsumables.cpp
 *
 *  \brief CmdLeicaConsumables command implementation.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 13.08.2012
 *   $Author:  $ Vikram MK
 *
 *  \b Company:
 *
 *       Leica Biosystems Nussloch GmbH.
 *
 *  (C) Copyright 2010 by Leica Biosystems Nussloch GmbH. All rights reserved.
 *  This is unpublished proprietary source code of Leica. The copyright notice
 *  does not evidence any actual or intended publication.
 *
 */
/****************************************************************************/

#include <QBuffer>

#include "RMSCommands/Include/CmdLeicaConsumables.h"

namespace MsgClasses {

QString CmdLeicaConsumables::NAME = "MsgClasses::CmdLeicaConsumables";

/****************************************************************************/
/*!
 * \brief   Constructor for sending
 *
 * \param[in]   Timeout             Timeout for command.
 * \param[in]   ProgramSequence     Program sequence.
 */
/****************************************************************************/
CmdLeicaConsumables::CmdLeicaConsumables(int Timeout, const QDataStream &RFIDStream) :
    Command(Timeout)
{
    m_RFIDByteArray = static_cast< QBuffer *>(RFIDStream.device())->data();
}

/****************************************************************************/
/*!
 * \brief   Constructor for receiving
 */
/****************************************************************************/
CmdLeicaConsumables::CmdLeicaConsumables() : Command(0)
{
}

/****************************************************************************/
/*!
 *  \brief   Destructor
 */
/****************************************************************************/
CmdLeicaConsumables::~CmdLeicaConsumables()
{
}

/****************************************************************************/
/*!
 *  \brief   Get command name
 *
 *  \return  command name as string
 */
/****************************************************************************/
QString CmdLeicaConsumables::GetName() const
{
    return NAME;
}

} // end namespace MsgClasses
