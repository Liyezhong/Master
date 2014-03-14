/****************************************************************************/
/*! \file ClientUserProtocolTxCommand.cpp
 *
 *  \brief ClientUserProtocolTxCommand implementation.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2011-07-07
 *   $Author:  $ M.Scherer
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

#include "ClientUser/Include/ClientUserProtocolTxCommand.h"
#include "ClientUser/Include/ChangeDateAndTime.h"
#include "ClientUser/Include/DataExport.h"
#include "ClientUser/Include/DataImport.h"


namespace ClientUser {

/*! Defintion of the ChangeDateAndTime TX command registration */
DEFINE_REG_CLASS(ClientUser::CClientUserProtocolTxCommand, ChangeDateAndTime);
/*! Defintion of the DataExport TX command registration */
DEFINE_REG_CLASS(ClientUser::CClientUserProtocolTxCommand, DataExport);
/*! Defintion of the DataImport TX command registration */
DEFINE_REG_CLASS(ClientUser::CClientUserProtocolTxCommand, DataImport);

/****************************************************************************/
/*!
 *  \brief   Constructor
 *
 ****************************************************************************/
CClientUserProtocolTxCommand::CClientUserProtocolTxCommand() :
        ProtocolTxCommand::ProtocolTxCommand()
{
}

/****************************************************************************/
/*!
 * \brief This method must be called prior to first use of the class object.
 *
 *        Calling this method ensures that the linker will create all needed
 *        static members. Otherwise it will not. Seem to be a linker bug.
 *
 */
/****************************************************************************/
void CClientUserProtocolTxCommand::LinkerTamer()
{
    qDebug() << "CClientUserProtocolTxCommand: LinkerTamer called !";
}


} // end namespace ClientUser
