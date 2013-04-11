/****************************************************************************/
/*! \file CmdProgramStartReady.cpp
 *
 *  \brief CmdProgramStartReady command implementation.
 *
 *   $Version: $ 0.1
*   $Date:    $ 27.03.2013
 *   $Author:  $ Abe Yang
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

#include "HimalayaDataContainer/Containers/DashboardStations/Commands/Include/CmdProgramStartReady.h"
namespace MsgClasses {

QString CmdProgramStartReady::NAME = "MsgClasses::CmdProgramStartReady";


CmdProgramStartReady::CmdProgramStartReady(int Timeout, bool IsReady) :
    Command(Timeout),
    m_IsReady(IsReady)
{
}

CmdProgramStartReady::CmdProgramStartReady()
    : Command(0)
{
}

/****************************************************************************/
/*!
 *  \brief   Destructor
 */
/****************************************************************************/
CmdProgramStartReady::~CmdProgramStartReady()
{
}

/****************************************************************************/
/*!
 *  \brief   Get command name
 *
 *  \return  command name as string
 */
/****************************************************************************/
QString CmdProgramStartReady::GetName() const
{
    return NAME;
}

} // end namespace MsgClasses
