/****************************************************************************/
/*! \file CmdSavedServiceInfor.cpp
 *
 *  \brief CmdSavedServiceInfor command implementation.
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

#include "Scheduler/Commands/Include/CmdSavedServiceInfor.h"
#include <QBuffer>
namespace MsgClasses {

QString CmdSavedServiceInfor::NAME = "MsgClasses::CmdSavedServiceInfor";

/****************************************************************************/
/*!
 *  \brief   Constructor
 *
 * \param[in]   Timeout              Timeout for command.
 * \param[in]   ProgramDataStream    The changed program.
 */
/****************************************************************************/
CmdSavedServiceInfor::CmdSavedServiceInfor(int Timeout, const QString& deviceType)
    :Command(Timeout), m_DeviceType(deviceType)
{
}


/****************************************************************************/
/*!
 *  \brief   Destructor
 */
/****************************************************************************/
CmdSavedServiceInfor::~CmdSavedServiceInfor()
{
}

/****************************************************************************/
/*!
 *  \brief   Get command name
 *
 *  \return  command name as string
 */
/****************************************************************************/
QString CmdSavedServiceInfor::GetName() const
{
    return NAME;
}

} // end namespace MsgClasses
