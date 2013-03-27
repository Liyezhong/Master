/****************************************************************************/
/*! \file DataProgramListVerifier.h
 *
 *  \brief Definition file for class CDataProgramListVerifier.
 *
 *  $Version:   $ 0.1
 *  $Date:      $ 2011-08-01
 *  $Author:    $ F. Toth, N.Kamath
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

#ifndef DATAMANAGER_DATAPROGRAMLISTVERIFIER_H
#define DATAMANAGER_DATAPROGRAMLISTVERIFIER_H

#include <QString>
#include <QXmlStreamReader>

#include "DataManager/Containers/ContainerBase/Include/DataContainerBase.h"
#include "HimalayaDataContainer/Containers/Programs/Include/DataProgramList.h"
#include "DataManager/Containers/ContainerBase/Include/VerifierInterface.h"


namespace DataManager {
class CDataContainer;

// constraints
const int PROGRAMS_MAX = 50;                //!< Max program allowed
const int NEXT_STEP_ID_MIN = 1;             //!< Minimum value of step Id
const int NAME_LENGTH_MIN = 1;        //!< name length Min
const int NAME_LENGTH_MAX = 20;        //!< name length Max
const int LONG_NAME_LENGTH_MIN = 1;         //!< Long name length Min
const int LONG_NAME_LENGTH_MAX = 32;        //!< Long name length Max
const int OVEN_TEMPERATURE_MAX = 70;        //!< Oven temperature Max
const int OVEN_TEMPERATURE_MIN = 10;        //!< Oven temperature Min
const int OVEN_TEMPERATURE_STEP_VALUE = 5;  //!< Oven temperature steps value
const int PROGRAM_STEPS_MAX = 50;           //!< Max program steps per program
const int PARALLEL_STATIONS_MAX = 3;        //!< Max number of parallel stations
const int PARALLEL_STATIONS_MIN = 0;        //!< Minimum number of parallel stations
const int STEP_DURATION_MIN = 1;            //!< Minimum value of step duration in secs
const int STEP_DURATION_MAX = 86400;        //!< Maximum value of step duration in secs
const int SECONDS_PER_MIN = 60;             //!< number of secs per minute
const int MINS_PER_HOUR = 60;               //!< Number of mins per se
const int HOURS_PER_DAY = 24;               //!< Number of hours per day
const int ZERO_PERCENT = 0;                 //!< 0 %
const int HUNDRED_PERCENT = 100;            //!< 100 %
const int INTENSITY_MAX = 10;               //!< Maximum value of intensity
const int INTENSITY_MIN = 0;                //!< Minimum value of intensity
const int NUMBEROFSTEPS_MIN = 2;            //!< Minimum value of number of steps
const int NUMBEROFSTEPS_MAX = 9;            //!< Maximum value of number of steps

/****************************************************************************/
/*!
 *  \brief  This class implements Program list verifier
 */
/****************************************************************************/
class CDataProgramListVerifier:public IVerifierInterface
{
public:
    CDataProgramListVerifier();
    CDataProgramListVerifier(CDataContainer *p_DataContainer);

    bool VerifyData(CDataContainerBase* p_DataProgramList);  // use concrete class for concrete verifier
    //lint -esym(1536,GetErrors )
    ErrorHash_t &GetErrors();

    void ResetLastErrors();

    bool IsLocalVerifier();

    /****************************************************************************/
    /*!
     *  \brief  Destructor
     */
    /****************************************************************************/
    virtual ~CDataProgramListVerifier() {}
private:
    CDataProgramList* mp_DPL;           //!< pointer to program list container
    ErrorHash_t m_ErrorsHash;           //!< To store Error ID and any arguments associated
    QStringList m_LeicaReagentIDList;   //!< To store the Leica reagent IDs
    CDataContainer *mp_DataContainer;   //!< DataContainer
    void CheckProgramStep(CProgram* p_Program,bool &VerifiedData);
    void CheckDurationFormat(QStringList SplitList, bool &VerifiedData, QString ProgramName);

};

} // namespace DataManager

#endif // DATAMANAGER_DATAPROGRAMLISTVERIFIER_H
