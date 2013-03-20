/****************************************************************************/
/*! \file CmdUpdateRMSForecast.h
 *
 *  \brief CmdUpdateRMSForecast command definition.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 17.03.2011, 10.06.2012
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

#ifndef MSGCLASSES_CMDUPDATERMSFORECAST_H
#define MSGCLASSES_CMDUPDATERMSFORECAST_H

#include <Global/Include/Commands/Command.h>
#include <QByteArray>
#include <QBuffer>
#include <QDebug>

namespace MsgClasses {

/****************************************************************************/
/*!
 *  \brief  This class implements a CmdUpdateRMSForecast command.
 *
 * \todo implement
 */
/****************************************************************************/
class CmdUpdateRMSForecast : public Global::Command {
    friend QDataStream & operator << (QDataStream &, const CmdUpdateRMSForecast &);
    friend QDataStream & operator >> (QDataStream &, CmdUpdateRMSForecast &);
public:
    static QString NAME;    ///< Command name.
    /****************************************************************************/
    CmdUpdateRMSForecast(int Timeout, const QDataStream &RMSForecastDataStream);
    CmdUpdateRMSForecast();
    ~CmdUpdateRMSForecast();
    virtual QString GetName() const;
    /****************************************************************************/
    /**
     * \brief Get the RMS forcast data.
     * \return Byte array.
     */
    /****************************************************************************/
    QByteArray const & GetRMSForecastData() const { return m_RMSForecastByteArray;}

private:
    CmdUpdateRMSForecast(const CmdUpdateRMSForecast &);                     ///< Not implemented.
    const CmdUpdateRMSForecast & operator = (const CmdUpdateRMSForecast &); ///< Not implemented.
    
    QByteArray m_RMSForecastByteArray; //!< Byte array for forcast
}; // end class CmdUpdateRMSForecast

/****************************************************************************/
/**
 * \brief Streaming operator.
 *
 * \param[in,out]   Stream      Stream to stream into.
 * \param[in]       Cmd         The command to stream.
 * \return                      Stream.
 */
/****************************************************************************/
inline QDataStream & operator << (QDataStream &Stream, const CmdUpdateRMSForecast &Cmd)
{
    // copy base class data
    Cmd.CopyToStream(Stream);
    // copy internal data
    Stream << Cmd.m_RMSForecastByteArray;
    return Stream;
}

/****************************************************************************/
/**
 * \brief Streaming operator.
 *
 * \param[in,out]   Stream      Stream to stream from.
 * \param[in]       Cmd         The command to stream.
 * \return                      Stream.
 */
/****************************************************************************/
inline QDataStream & operator >> (QDataStream &Stream, CmdUpdateRMSForecast &Cmd)
{
    // copy base class data
    Cmd.CopyFromStream(Stream);
    // copy internal data
    Stream >> Cmd.m_RMSForecastByteArray;
    //    Cmd.mp_RMSForecastBuffer->setData(Cmd.m_RMSForecastByteArray);
    return Stream;
}

} // end namespace MsgClasses

#endif // MSGCLASSES_CMDUPDATERMSFORECAST_H
