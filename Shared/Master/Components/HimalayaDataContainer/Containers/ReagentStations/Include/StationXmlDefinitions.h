/****************************************************************************/
/*! \file StationXmlDefinitions.h
 *
 *  \brief Global definition file for station xml element and attribute.
 *
 *  $Version:   $ 0.1
 *  $Date:      $ 2013-02-07
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

#ifndef DATAMANAGER_STATION_XMLDEFINITIONS_H
#define DATAMANAGER_STATION_XMLDEFINITIONS_H



namespace DataManager {
namespace SN {

/****************************************************************************/
/*!
 *  \brief  Definition/Declaration of function ELEMENT_STATIONS
 *
 *  \return from ELEMENT_STATIONS
 */
/****************************************************************************/
const QString ELEMENT_STATIONS          ("Stations");
/****************************************************************************/
/*!
 *  \brief  Definition/Declaration of function ATTRIBUTE_VERSION
 *
 *  \return from ATTRIBUTE_VERSION
 */
/****************************************************************************/
const QString ATTRIBUTE_VERSION         ("Version");

/****************************************************************************/
/*!
 *  \brief  Definition/Declaration of function ELEMENT_STATION
 *
 *  \return from ELEMENT_STATION
 */
/****************************************************************************/
const QString ELEMENT_STATION           ("Station");
/****************************************************************************/
/*!
 *  \brief  Definition/Declaration of function ATTRIBUTE_ID
 *
 *  \return from ATTRIBUTE_ID
 */
/****************************************************************************/
const QString ATTRIBUTE_ID              ("ID");
/****************************************************************************/
/*!
 *  \brief  Definition/Declaration of function ATTRIBUTE_NAME
 *
 *  \return from ATTRIBUTE_NAME
 */
/****************************************************************************/
const QString ATTRIBUTE_NAME            ("Name");
/****************************************************************************/
/*!
 *  \brief  Definition/Declaration of function ATTRIBUTE_PARAFFIN_BATH
 *
 *  \return from ATTRIBUTE_PARAFFIN_BATH
 */
/****************************************************************************/
const QString ATTRIBUTE_PARAFFIN_BATH   ("ParaffinBath");

/****************************************************************************/
/*!
 *  \brief  Definition/Declaration of function ELEMENT_REAGENT
 *
 *  \return from ELEMENT_REAGENT
 */
/****************************************************************************/
const QString ELEMENT_REAGENT           ("Reagent");
/****************************************************************************/
/*!
 *  \brief  Definition/Declaration of function ATTRIBUTE_EXCHANGE_DATE
 *
 *  \return from ATTRIBUTE_EXCHANGE_DATE
 */
/****************************************************************************/
const QString ATTRIBUTE_EXCHANGE_DATE   ("ExchangeDate");
/****************************************************************************/
/*!
 *  \brief  Definition/Declaration of function ATTRIBUTE_ACTUAL_CASSETTES
 *
 *  \return from ATTRIBUTE_ACTUAL_CASSETTES
 */
/****************************************************************************/
const QString ATTRIBUTE_ACTUAL_CASSETTES("ActualCassettes");
/****************************************************************************/
/*!
 *  \brief  Definition/Declaration of function ATTRIBUTE_ACTUAL_CYCLES
 *
 *  \return from ATTRIBUTE_ACTUAL_CYCLES
 */
/****************************************************************************/
const QString ATTRIBUTE_ACTUAL_CYCLES   ("ActualCycles");
/****************************************************************************/
/*!
 *  \brief  Definition/Declaration of function ATTRIBUTE_STATUS
 *
 *  \return from ATTRIBUTE_STATUS
 */
/****************************************************************************/
const QString ATTRIBUTE_STATUS          ("Status");

/****************************************************************************/
/*!
 *  \brief  Definition/Declaration of function VALUE_TRUE
 *
 *  \return from VALUE_TRUE
 */
/****************************************************************************/
const QString VALUE_TRUE                ("true");
/****************************************************************************/
/*!
 *  \brief  Definition/Declaration of function VALUE_FALSE
 *
 *  \return from VALUE_FALSE
 */
/****************************************************************************/
const QString VALUE_FALSE               ("false");

/****************************************************************************/
/*!
 *  \brief  Definition/Declaration of function VALUE_FULL
 *
 *  \return from VALUE_FULL
 */
/****************************************************************************/
const QString VALUE_FULL                ("full");
/****************************************************************************/
/*!
 *  \brief  Definition/Declaration of function VALUE_EMPTY
 *
 *  \return from VALUE_EMPTY
 */
/****************************************************************************/
const QString VALUE_EMPTY               ("empty");

}
}

#endif // DATAMANAGER_STATION_XMLDEFINITIONS_H
