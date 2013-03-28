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

#include <QString>

namespace DataManager {
namespace SN {

const QString ELEMENT_STATIONS          ("Stations");
const QString ATTRIBUTE_VERSION         ("Version");

const QString ELEMENT_STATION           ("Station");
const QString ATTRIBUTE_ID              ("ID");
const QString ATTRIBUTE_NAME            ("Name");
const QString ATTRIBUTE_PARAFFIN_BATH   ("ParaffinBath");

const QString ELEMENT_REAGENT           ("Reagent");
const QString ATTRIBUTE_EXCHANGE_DATE   ("ExchangeDate");
const QString ATTRIBUTE_ACTUAL_CASSETTES("ActualCassettes");
const QString ATTRIBUTE_ACTUAL_CYCLES   ("ActualCycles");
const QString ATTRIBUTE_STATUS          ("Status");

const QString VALUE_TRUE                ("true");
const QString VALUE_FALSE               ("false");

const QString VALUE_FULL                ("full");
const QString VALUE_EMPTY               ("empty");

}
}

#endif // DATAMANAGER_STATION_XMLDEFINITIONS_H
