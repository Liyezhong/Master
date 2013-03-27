/****************************************************************************/
/*! \file CmdParaffinBathStatus.cpp
 *
 *  \brief CmdParaffinBathStatus command implementation.
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

#include "HimalayaDataContainer/Containers/DashboardStations/Commands/Include/CmdParaffinBathStatus.h"
namespace MsgClasses {

QString CmdParaffinBathStatus::NAME = "MsgClasses::CmdParaffinBathStatus";


CmdParaffinBathStatus::CmdParaffinBathStatus(int Timeout, const QString& ParaffinBathID,
                                    DataManager::ParaffinBathStatusType_t ParaffinBathStatusType) :
    Command(Timeout),
    m_ParaffinBathID(ParaffinBathID),
    m_ParaffinBathStatusType(ParaffinBathStatusType)
{
}

CmdParaffinBathStatus::CmdParaffinBathStatus()
    : Command(0)
{
}

/****************************************************************************/
/*!
 *  \brief   Destructor
 */
/****************************************************************************/
CmdParaffinBathStatus::~CmdParaffinBathStatus()
{
}

/****************************************************************************/
/*!
 *  \brief   Get command name
 *
 *  \return  command name as string
 */
/****************************************************************************/
QString CmdParaffinBathStatus::GetName() const
{
    return NAME;
}

} // end namespace MsgClasses
