/****************************************************************************/
/*! \file CmdUpdateRMSForecast.cpp
 *
 *  \brief CmdUpdateRMSForecast command implementation.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 17.03.2010, 10.06.2012
 *   $Author:  $ Y.Novak, Vikram MK
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

#include <HimalayaDataContainer/Containers/Stations/Commands/Include/CmdUpdateRMSForecast.h>

namespace MsgClasses {

QString CmdUpdateRMSForecast::NAME = "MsgClasses::CmdUpdateRMSForecast";

/****************************************************************************/
/*!
 *  \brief   Constructor for sending
 *
 * \param[in]   Timeout                 Timeout for command.
 * \param[in]   RMSForecastDataStream   Datastream of RMS Forecast Data.
 */
/****************************************************************************/
CmdUpdateRMSForecast::CmdUpdateRMSForecast(int Timeout, const QDataStream &RMSForecastDataStream) :
    Command(Timeout)
{
    m_RMSForecastByteArray = static_cast< QBuffer *>(RMSForecastDataStream.device())->data();
}

/****************************************************************************/
/*!
 * \brief   Constructor for receiving
 */
/****************************************************************************/
CmdUpdateRMSForecast::CmdUpdateRMSForecast() : Command(0)
{
}

/****************************************************************************/
/*!
 *  \brief   Destructor
 */
/****************************************************************************/
CmdUpdateRMSForecast::~CmdUpdateRMSForecast()
{
}

/****************************************************************************/
/*!
 *  \brief   Get command name
 *
 *  \return  command name as string
 */
/****************************************************************************/
QString CmdUpdateRMSForecast::GetName() const
{
    return NAME;
}

} // end namespace MsgClasses
