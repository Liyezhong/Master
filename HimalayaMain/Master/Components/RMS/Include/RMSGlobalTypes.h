/****************************************************************************/
/*! \file RMS/Include/RMSGlobalTypes.h
 *
 *  \brief Global Data definations.
 *
 *  $Version:   $ 0.1
 *  $Date:      $ 2012-08-15 05-10-2012
 *  $Author:    $ Vikram MK,Ningu
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

#ifndef RMSGLOBALTYPES_H
#define RMSGLOBALTYPES_H

const int LEICACONSUMABLES_KIT_TYPEID = 1;         //!< Kit type ID
const int LEICACONSUMABLES_BOTTLE_TYPEID = 2;      //!< Bottle type ID
const int SEPIA_MOUNTING_MEDIUM_TYPEID = 3;        //!< Sepia mounting medium type ID
const int SEPIA_COVERSLIPS_TYPEID = 4;             //!< Sepia coverslips type ID
const int CENTURY_21 = 2000;                        //!< Century Identifier
const int LEICACONSUMABLES_SCANNED_ALREADY = 0xCC;  //!< Kit/Bottle already scanned identifier

typedef enum
{
    NODATA_READ,        //!< No data read indicator
    KITDATA_READ,       //!< Kit data read indicator
    BOTTLEDATA_READ     //!< Bottle data read indicator
} RfidDataType_t;
//typedef enum
//{
//    SCAN_START,         //!< Scan process started indicator
//    SCAN_PROGRESS,      //!< Scan progress indicator
//    SCAN_ERROR          //!< Scan error indicator
//} ScanStatus_t;


#endif // RMSGLOBALTYPES_H
