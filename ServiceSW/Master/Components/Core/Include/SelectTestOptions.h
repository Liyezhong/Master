/****************************************************************************/
/*! \file SelectTestOptions.h
 *
 *  \brief
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2014-05-12
 *   $Author:  $ Abe Yang
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

#ifndef CORE_SELECTTESTOPTIONS_H
#define CORE_SELECTTESTOPTIONS_H
#include <QObject>

namespace Core {

/****************************************************************************/
/**
 * \brief Enum for RMS mode.
 */
/****************************************************************************/
typedef enum  {
    MANUFACTURAL_UNDEFINED_TEST = -1,  ///< Undefind. Used for initialization.
    MANUFACTURAL_STATIONTEST,            ///< Station Test
    MANUFACTURAL_ENDTEST                 ///< End Test
} ManufacturalTestMode_t;

/****************************************************************************/
/**
 * \brief This class for user to select a working mode(StationTest/EndTest) when launch Manufacturing Software .
 */
/****************************************************************************/

class CSelectTestOptions : public QObject
{
    Q_OBJECT

public:
    /****************************************************************************/
    /*!
     *  \brief Constructor
     */
    /****************************************************************************/
    CSelectTestOptions();

    /****************************************************************************/
    /*!
     *  \brief Destructor
     */
    /****************************************************************************/
    ~CSelectTestOptions();
    /****************************************************************************/
    /**
     * \brief Get reference to instance.
     *
     * \return      Reference to instance.
     */
    /****************************************************************************/
    static ManufacturalTestMode_t GetCurTestMode();

    /****************************************************************************/
    /**
     * \brief Set current test mode.
     * iparam testMod = test mode.
     */
    /****************************************************************************/
    static void SetCurTestMode(ManufacturalTestMode_t testMod);
private:
    static ManufacturalTestMode_t m_TestMod; ///< The one and only instance.

};

}   // end of namespace Core


#endif // CORE_SELECTTESTOPTIONS_H
