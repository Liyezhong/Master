/****************************************************************************/
/*! \file HimalayaDeviceEventCodes.h
 *
 *  \brief All event codes used by Error handling for Himalaya device .
 *
 *  $Version:   $ 0.1
 *  $Date:      $ 2013-2-27
 *  $Author:    $ Abe Yang
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

#ifndef HIMALAYADEVICEEVENTCODES_H
#define HIMALAYADEVICEEVENTCODES_H
const quint32 EVENT_PRESSURESENSOR_SELFTEST_FAILED	= 500040001;
	
const quint32 EVENT_BOTTLECHECK_FAILED_EMPTY	= 500040011;
const quint32 EVENT_BOTTLECHECK_FAILED_INSUFFICIENT	= 500040012;
const quint32 EVENT_BOTTLECHECK_FAILED_BLOCKAGE	= 500040013;
	
const quint32 EVENT_SEALING_FAILED_PRESSURE	= 500040101;
const quint32 EVENT_SEALING_FAILED_VACUUM	= 500040151;
	
const quint32 EVENT_SUCKING_TIMEOUT_2MIN	= 500040201;
	
const quint32 EVENT_SUCKING_TIMEOUT_4MIN	= 500040221;
	
const quint32 EVENT_SUCKING_OVERFLOW	= 500040241;
	
const quint32 EVENT_SUCKING_INTERUPT	= 500040261;
	
const quint32 EVENT_DRAINING_TIMEOUT_BULIDPRESSURE	= 500040301;
	
const quint32 EVENT_DRAINING_TIMEOUT_EMPTY_2MIN	= 500040321;
	
const quint32 EVENT_DRAINING_TIMEOUT_EMPTY_4MIN	= 500040341;
	
const quint32 EVENT_DRAINING_INTERUPT	= 500040361;
	
const quint32 EVENT_RELEASING_TIMEOUT	= 500040401;
	
const quint32 EVENT_STATUS_EXHAUSTFAN	= 500040501;
	
const quint32 EVENT_TUBEHEATING_TUBE1_ABNORMAL	= 500040601;
const quint32 EVENT_TUBEHEATING_TUBE2_ABNORMAL	= 500040711;
	
const quint32 EVENT_TEMPSENSOR_TUBE1_OUTOFRANGE	= 500040621;
const quint32 EVENT_TEMPSENSOR_TUBE2_OUTOFRANGE	= 500040731;


#endif // HIMALAYADEVICEEVENTCODES_H
