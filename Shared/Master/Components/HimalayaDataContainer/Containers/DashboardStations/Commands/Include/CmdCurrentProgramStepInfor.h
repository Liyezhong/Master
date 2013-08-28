/****************************************************************************/
/*! \file CmdCurrentProgramStepInfor.h
 *
 *  \brief CmdCurrentProgramStepInfor command definition.
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

#ifndef MSGCLASSES_CMDCURRENTPROGRAMSTEPINFOR_H
#define MSGCLASSES_CMDCURRENTPROGRAMSTEPINFOR_H

#include <Global/Include/Commands/Command.h>
#include <HimalayaDataContainer/Helper/Include/Global.h>
#include <QTime>

namespace MsgClasses {

/****************************************************************************/
/*!
 *  \brief  This class implements a CmdCurrentProgramStepInfor command.
 *
 * \todo implement
 */
/****************************************************************************/
class CmdCurrentProgramStepInfor : public Global::Command {
    friend QDataStream & operator << (QDataStream &, const CmdCurrentProgramStepInfor &);
    friend QDataStream & operator >> (QDataStream &, CmdCurrentProgramStepInfor &);
public:
    CmdCurrentProgramStepInfor();                                             ///< Not implemented.

    static QString NAME;    ///< Command name.
    /****************************************************************************/
    CmdCurrentProgramStepInfor(int Timeout, const QString& StepName, int CurProgramStepIndex, const QTime& CurRemainingTime);
    ~CmdCurrentProgramStepInfor();
    virtual QString GetName() const;
    inline const QString& StepName() const {return m_StepName;}
    inline const QTime& CurRemainingTime()const {return m_CurRemainingTime;}
    inline int CurProgramStepIndex() const {return m_CurProgramStepIndex;}
private:
    CmdCurrentProgramStepInfor(const CmdCurrentProgramStepInfor &);                     ///< Not implemented.
    const CmdCurrentProgramStepInfor & operator = (const CmdCurrentProgramStepInfor &); ///< Not implemented.
private:
    QString m_StepName;
    int m_CurProgramStepIndex;
    QTime   m_CurRemainingTime; 
    
}; // end class CmdCurrentProgramStepInfor

/****************************************************************************/
/**
 * \brief Streaming operator.
 *
 * \param[in,out]   Stream      Stream to stream into.
 * \param[in]       Cmd         The command to stream.
 * \return                      Stream.
 */
/****************************************************************************/
inline QDataStream & operator << (QDataStream &Stream, const CmdCurrentProgramStepInfor &Cmd)
{
    // copy base class data
    Cmd.CopyToStream(Stream);
    // copy internal data
    Stream << Cmd.m_StepName;
    Stream << Cmd.m_CurProgramStepIndex;
    Stream << Cmd.m_CurRemainingTime;

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
inline QDataStream & operator >> (QDataStream &Stream, CmdCurrentProgramStepInfor &Cmd)
{
    // copy base class data
    Cmd.CopyFromStream(Stream);
    // copy internal data
    Stream >> Cmd.m_StepName;
    Stream >> Cmd.m_CurProgramStepIndex;
    Stream >> Cmd.m_CurRemainingTime;
    return Stream;
}
} // end namespace MsgClasses

#endif // MSGCLASSES_CMDCURRENTPROGRAMSTEPINFOR_H
