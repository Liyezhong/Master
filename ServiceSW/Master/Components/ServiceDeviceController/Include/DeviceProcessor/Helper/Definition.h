/****************************************************************************/
/*! \file Definition.h
 *
 *  \brief  Contains mapping of station strings into rows and columns
 *
 *  \version  0.1
 *  \date     2013-03-11
 *  \author   M.Scherer
 *
 *  \b Description:
 *
 *   N/A
 *
 *  \b Company:
 *
 *       Leica Biosystems Nussloch GmbH.
 *
 *  (C) Copyright 2012 by Leica Biosystems Nussloch GmbH. All rights reserved
 *  This is unpublished proprietary source code of Leica. The copyright notice
 *  does not evidence any actual or intended publication.
 */
/****************************************************************************/

#ifndef DEFINITION_H
#define DEFINITION_H

#include <QObject>

//! Contains position data of a Colorado station
typedef struct
{
    QString name;       //!< Station name
    int row;            //!< Row of the station
    int col;            //!< Column of the station
    QString typeName;   //!< Station type string
} StationFixed_t;

//! Contains name, category & position information of each station in Colorado
const StationFixed_t stationInit[] =
{
    {"OvenStation1", 1, 1, "OVEN"},
    {"OvenStation2", 1, 2, "OVEN"},
    {"OvenStation3", 1, 3, "OVEN"},
    {"OvenStation4", 2, 1, "OVEN"},
    {"OvenStation5", 2, 2, "OVEN"},
    {"OvenStation6", 2, 3, "OVEN"},

    {"SLIDE", 3, 2,  "SLIDE"},

    {"HeatVesselStation1", 1, 4, "HEATED"},
    {"HeatVesselStation2", 1, 5, "HEATED"},
    {"HeatVesselStation3", 2, 4, "HEATED"},
    {"HeatVesselStation4", 2, 5, "HEATED"},
    {"HeatVesselStation5", 3, 4, "HEATED"},
    {"HeatVesselStation6", 3, 5, "HEATED"},

    {"ReagentStation1",  1, 6,  "REAGENT"},
    {"ReagentStation2",  1, 7,  "REAGENT"},
    {"ReagentStation3",  1, 8,  "REAGENT"},
    {"ReagentStation4",  1, 9,  "REAGENT"},
    {"ReagentStation5",  1, 10, "REAGENT"},
    {"ReagentStation6",  1, 13, "REAGENT"},
    {"ReagentStation7",  1, 14, "REAGENT"},
    {"ReagentStation8",  1, 15, "REAGENT"},
    {"ReagentStation9",  1, 16, "REAGENT"},
    {"ReagentStation10", 1, 17, "REAGENT"},
    {"ReagentStation11", 2, 6,  "REAGENT"},
    {"ReagentStation12", 2, 7,  "REAGENT"},
    {"ReagentStation13", 2, 8,  "REAGENT"},
    {"ReagentStation14", 2, 9,  "REAGENT"},
    {"ReagentStation15", 2, 10, "REAGENT"},
    {"ReagentStation16", 2, 13, "REAGENT"},
    {"ReagentStation17", 2, 14, "REAGENT"},
    {"ReagentStation18", 2, 15, "REAGENT"},
    {"ReagentStation19", 2, 16, "REAGENT"},
    {"ReagentStation20", 2, 17, "REAGENT"},
    {"ReagentStation21", 3, 6,  "REAGENT"},
    {"ReagentStation22", 3, 7,  "REAGENT"},
    {"ReagentStation23", 3, 8,  "REAGENT"},
    {"ReagentStation24", 3, 9,  "REAGENT"},
    {"ReagentStation25", 3, 10, "REAGENT"},
    {"ReagentStation26", 3, 13, "REAGENT"},
    {"ReagentStation27", 3, 14, "REAGENT"},
    {"ReagentStation28", 3, 15, "REAGENT"},
    {"ReagentStation29", 3, 16, "REAGENT"},
    {"ReagentStation30", 3, 17, "REAGENT"},

    {"WaterStation1", 1, 11,  "WATER"},
    {"WaterStation2", 1, 12,  "WATER"},
    {"WaterStation3", 2, 11,  "WATER"},
    {"WaterStation4", 2, 12,  "WATER"},
    {"WaterStation5", 3, 11,  "WATER"},
    {"WaterStation6", 3, 12,  "WATER"},

    {"LoaderStation1", 4, 6,  "LOAD"},
    {"LoaderStation2", 4, 7,  "LOAD"},
    {"LoaderStation3", 4, 8,  "LOAD"},
    {"LoaderStation4", 4, 9,  "LOAD"},
    {"LoaderStation5", 4, 10, "LOAD"},

    {"DryStation1", 4, 11,  "DRY"},
    {"DryStation2", 4, 12,  "DRY"},

    {"UnloadStation1", 4, 13,  "UNLOAD"},
    {"UnloadStation2", 4, 14,  "UNLOAD"},
    {"UnloadStation3", 4, 15,  "UNLOAD"},
    {"UnloadStation4", 4, 16,  "UNLOAD"},
    {"UnloadStation5", 4, 17,  "UNLOAD"},

    {"TransferStation1", 2, 18,  "TRANSFER"},

    {"ParkStation1", 4, 1,  "PARK"},
    {"ParkStation2", 1, 18, "PARK"},
};

#endif // DEFINITION_H
