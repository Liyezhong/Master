/****************************************************************************/
/*! \file CommandBase.cpp
 *
 *  \brief Command base implementation.
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

#include "HimalayaDataContainer/Containers/ContainerBase/Commands/Include/CommandBase.h"

#include <QBuffer>


namespace MsgClasses {

/****************************************************************************/
/*!
 *  \brief   Constructor for sending
 *
 * \param[in] timeout    timeout for command.
 * \param[in] dataStream data stream which contains the initial data.
 */
/****************************************************************************/
CommandBase::CommandBase(int timeout, const QDataStream &dataStream)
    : Command(timeout)
{
    m_ByteArray = static_cast<QBuffer *>(dataStream.device())->data();
}

CommandBase::CommandBase(void)
    : Command(0)
    , m_ByteArray()
{
}

CommandBase::~CommandBase(void)
{
}

/****************************************************************************/
/*!
 *  \brief   Get data
 *
 *  \return  command data as byte array
 */
/****************************************************************************/
const QByteArray &CommandBase::GetData(void) const
{
    return m_ByteArray;
}

/****************************************************************************/
/**
 * \brief output operator.
 *
 * \param[in] stream data stream which receives the command data.
 * \param[in] cmd    command to be copied.
 *
 * \return data stream.
 */
/****************************************************************************/
QDataStream & operator << (QDataStream &stream, const CommandBase &cmd)
{
    // copy base class data
    cmd.CopyToStream(stream);

    // copy internal data
    stream << cmd.m_ByteArray;

    return stream;
}

/****************************************************************************/
/**
 * \brief input operator.
 *
 * \param[in] stream data stream which contains the command data.
 * \param[in] cmd    command to be changed.
 *
 * \return data stream.
 */
/****************************************************************************/
QDataStream & operator >> (QDataStream &stream, CommandBase &cmd)
{
    // copy to base class data
    cmd.CopyFromStream(stream);

    // copy to internal data
    stream >> cmd.m_ByteArray;

    return stream;
}

} // end namespace MsgClasses
