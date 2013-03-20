/****************************************************************************/
/*! \file CmdProgramUpdate.cpp
 *
 *  \brief CmdProgramUpdate command implementation.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 18.03.2010
 *   $Author:  $ Y.Novak
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

#include "HimalayaDataContainer/Containers/Programs/Commands/Include/CmdProgramUpdate.h"
#include <QBuffer>
namespace MsgClasses {

QString CmdProgramUpdate::NAME = "MsgClasses::CmdProgramUpdate";

/****************************************************************************/
/*!
 *  \brief   Constructor
 *
 * \param[in]   Timeout              Timeout for command.
 * \param[in]   ProgramDataStream    The changed program.
 */
/****************************************************************************/
CmdProgramUpdate::CmdProgramUpdate(int Timeout, const QDataStream &ProgramDataStream)
    :Command(Timeout),
    m_ProgramByteArray(static_cast< QBuffer *>(ProgramDataStream.device())->data()),
    m_ProgramColorReplaced(false)
{
    setIdleStateAllowed(true);
}
/****************************************************************************/
/*!
 *  \brief   Constructor
 *
 * \param[in]   Timeout                  Timeout for command.
 * \param[in]   ProgramDataStream        The program which will be set to white color
 * \param[in]   NextProgramDataStream    The changed program
 */
/****************************************************************************/
CmdProgramUpdate::CmdProgramUpdate(int Timeout, const QDataStream &ProgramDataStream, const QDataStream &NextProgramDataStream) :
    Command(Timeout),
    m_ProgramByteArray(static_cast< QBuffer *>(ProgramDataStream.device())->data()),
    m_ColorAssignedProgramByteArray(static_cast<QBuffer *>(NextProgramDataStream.device())->data()),
    m_ProgramColorReplaced(false)
{
    setIdleStateAllowed(true);
}

/****************************************************************************/
/*!
 * \brief   Constructor for receiving
 */
/****************************************************************************/
CmdProgramUpdate::CmdProgramUpdate() : Command(0)
{
}

/****************************************************************************/
/*!
 *  \brief   Destructor
 */
/****************************************************************************/
CmdProgramUpdate::~CmdProgramUpdate()
{
}

/****************************************************************************/
/*!
 *  \brief   Get command name
 *
 *  \return  command name as string
 */
/****************************************************************************/
QString CmdProgramUpdate::GetName() const
{
    return NAME;
}

} // end namespace MsgClasses
