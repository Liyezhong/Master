/****************************************************************************/
/*! \file CmdHeatingTest.cpp
 *
 *  \brief Implementation file for class CmdHeatingTest.
 *
 *  $Version:   $ 0.1
 *  $Date:      $ 2013-05-18
 *  $Author:    $ Brandon Shao
 *
 *  \b Company:
 *
 *       Leica Biosystems Shanghai R&D Center.
 *
 *  (C) Copyright 2013 by Leica Biosystems Shanghai. All rights reserved.
 *  This is unpublished proprietary source code of Leica. The copyright notice
 *  does not evidence any actual or intended publication.
 *
 */
/****************************************************************************/

#include <ServiceDeviceController/Include/Commands/CmdHeatingTest.h>

namespace DeviceCommandProcessor {

QString CmdHeatingTest::NAME             = "DeviceCommandProcessor::CmdHeatingTest";
int     CmdHeatingTest::m_CmdTimeout     = 3000;

#if 0
/****************************************************************************/
CmdHeatingTest::CmdHeatingTest(DeviceControl::DevInstanceID_t DevInstanceID, quint32 TargetTemp,
                             quint32 Tolerance, quint32 HeatingSeconds, quint8 HeaterIndex)
    : Global::Command(m_CmdTimeout)
    , m_DevInstanceID(DevInstanceID)
    , m_TargetTemp(TargetTemp)
    , m_Tolerance(Tolerance)
    , m_HeatingSeconds(HeatingSeconds)
    , m_HeaterIndex(HeaterIndex)
{
}
#endif
/****************************************************************************/
CmdHeatingTest::CmdHeatingTest(quint32 DevInstanceID, quint8 HeaterIndex,
               quint8 CmdType)
    : Global::Command(m_CmdTimeout)
    , m_DevInstanceID(DevInstanceID)
    , m_HeaterIndex(HeaterIndex)
    , m_CmdType(CmdType)
{

}

/****************************************************************************/
CmdHeatingTest::~CmdHeatingTest() {
}

} // end namespace DeviceCommandProcessor

