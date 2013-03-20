/****************************************************************************/
/*! \file RMS/Include/LeicaReagentBottle.h
 *
 *  \brief Definition file for class CLeicaReagentBottle.
 *
 *  $Version:   $ 0.1
 *  $Date:      $ 2012-08-08
 *  $Author:    $ Vikram MK
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
#ifndef LEICAREAGENT_BOTTLE_H
#define LEICAREAGENT_BOTTLE_H

#include <QString>

namespace HimalayaRMS {


/****************************************************************************/
/**
 * \brief This is a Leica Reagent Bottle. Which stores the information of
 *        Leica Reagent Bottle.
 *
 * \warning This class is not thread safe!
 */
/****************************************************************************/
class CLeicaReagentBottle
{
    friend class CRMSHandler;
private:

    void SetReagentID(int ReagentNumber); //!< appends L to the Reagent number and stores in Reagent ID

    /****************************************************************************/
    /*!
     *  \brief Set's the Station ID associated with bottle
     *
     *  \iparam Value = Station ID
     *
     */
    /****************************************************************************/
    void SetStationID(QString Value){m_StationID = Value;}

    /****************************************************************************/
    /*!
     *  \brief Set's the Type ID of Bottle
     *
     *  \iparam Value = Bottle type ID
     *
     *  \return
     */
    /****************************************************************************/
    void SetTypeID(const int Value){m_TypeID.setNum(Value);}

    /****************************************************************************/
    /*!
     *  \brief Set's the LotNumber of the Reagent Bottle
     *  \iparam number = Reagents lot number
     *  \return
     */
    /****************************************************************************/
    void SetLotNumber(const  int number){m_LotNumber = number;}

    /****************************************************************************/
    /*!
     *  \brief Get's the Max Slides for one kit
     *
     *  \return Max Slides
     */
    /****************************************************************************/
    int GetMaxSlidesKit() {return m_MaxSlidesKit;}

    /****************************************************************************/
    /*!
     *  \brief Set's the Max Slides for one kit
     *  \iparam number = Maximum slides for one kit
     *  \return
     */
    /****************************************************************************/
    void SetMaxSlidesKit(const  int number){m_MaxSlidesKit = number;}

protected:
    QString m_TypeID;                   //!< Type ID - Bottle Specific
    QString m_ReagentID;                //!< Reagent ID without 'L' ,storing with L
    QString m_StationID;                //!< Station ID
    int m_LotNumber;                    //!< Reagents Lot Number
    int m_MaxSlidesKit;                 //!< Maximum No. of Slides for one kit

public:
    CLeicaReagentBottle();
    CLeicaReagentBottle(const CLeicaReagentBottle&);
    CLeicaReagentBottle& operator=(const CLeicaReagentBottle&);

    bool DeserializeBottleContent(const QByteArray *KitTagValue);   //!< This method deserializes the bottle Information
                                                                    //!< and stores it in the CLeicaReagentBottle

    /******************** INLINE FuNCTIONS **************************************/
    /****************************************************************************/
    /*!
     *  \brief Get's the Type ID of the Reagent Bottle
     *
     *  \return Type Id String
     */
    /****************************************************************************/
    QString  GetTypeID() {return m_TypeID;}

    /****************************************************************************/
    /*!
     *  \brief Get's the Reagent ID of the Reagent Bottle
     *
     *  \return Reagent Id String
     */
    /****************************************************************************/
    QString  GetReagentID() {return m_ReagentID;}

    /****************************************************************************/
    /*!
     *  \brief Get's the Station ID
     *
     *  \return Station Id String
     */
    /****************************************************************************/
    QString  GetStationID() {return m_StationID;}

    /****************************************************************************/
    /*!
     *  \brief Get's the LotNumber of the Reagent Bottle
     *
     *  \return Lotnumber
     */
    /****************************************************************************/
    int GetLotNumber() {return m_LotNumber;}
};

}

#endif // LEICAREAGENT_BOTTLE_H
