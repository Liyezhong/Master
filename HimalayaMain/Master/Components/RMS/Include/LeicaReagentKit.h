/****************************************************************************/
/*! \file RMS/Include/LeicaReagentKit.h
 *
 *  \brief Definition file for class CLeicaReagentKit.
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
#ifndef LEICAREAGENT_KIT_H
#define LEICAREAGENT_KIT_H

#include <QString>
#include <QDate>
#include <QHash>
#include <QList>

#include "Global/Include/GlobalDefines.h"
#include "RMS/Include/LeicaReagentBottle.h"

namespace HimalayaRMS {

typedef QList<int> ListOfLotNumbers_t;

/****************************************************************************/
/**
 * \brief This is a Leica Reagent Kit. Which stores the information of
 *        Leica Reagent Kit.
 *
 * \warning This class is not thread safe!
 */
/****************************************************************************/
class CLeicaReagentKit
{

private:

    CLeicaReagentKit(const CLeicaReagentKit &);                         //!< Not implemented.
    const CLeicaReagentKit & operator = (const CLeicaReagentKit &);     //!< Not implemented.
    void DeleteAllLotNumbers();

    void SetProgramID(int ProgramNumber); //!< appends L to the Program number and stores in Program ID

    /****************************************************************************/
    /*!
     *  \brief Set's the Type ID of Kit
     *
     *  \iparam Value = Kit type ID
     *
     *  \return
     */
    /****************************************************************************/
    void SetTypeID(const int Value){m_TypeID.setNum(Value);}

    /****************************************************************************/
    /*!
     *  \brief Set's the Number of Reagents in the kit
     *  \iparam Count = Count of Reagent Bottles present
     *  \return
     */
    /****************************************************************************/
    void SetReagentBottleCount(const int Count){m_ReagentBottleCount = Count;}

    /****************************************************************************/
    /*!
     *  \brief Set's the Open Shell Life Time of the Reagent in Kit
     *  \iparam number = Open Via Shell Life Time in number of Days
     *  \return
     */
    /****************************************************************************/
    void SetOpenShellLifetime(const int number){m_OpenShellLifeTime = number;}

    /****************************************************************************/
    /*!
     *  \brief Set's the MaxSlides of the Reagent in Kit
     *  \iparam number = maximum Slides for reagent to replace
     *  \return
     */
    /****************************************************************************/
    void SetMaxSlides(const int number){m_MaxSlides = number;}

    /****************************************************************************/
    /*!
     *  \brief Sets the Best Before Date
     *
     *  \iparam Value = Best Before Date in String (format yyyy-mm-dd)
     *
     *  \return
     */
    /****************************************************************************/
    void SetBestBeforeDate(const QString Value){m_BestBeforeDate = Value;}

protected:
    QString m_TypeID;                               //!< Type ID - Kit Specific
    QString m_ProgramID;                            //!< Kit ID is program ID associated with Kit
    int m_ReagentBottleCount;                       //!< Count of Reagent Bottles present
    QString m_BestBeforeDate;                       //!< Best before Date of the Kit Reagents
    int m_OpenShellLifeTime;                        //!< Open Via Shell Life Time in number of Days
    int m_MaxSlides;                                //!< maximum Slides for reagent to replace
    ListOfLotNumbers_t m_ReagentLotNumbers;         //!< List of Reagents Lot Numbers

public:
    CLeicaReagentKit();
    ~CLeicaReagentKit();

    bool DeserializeKitContent(const QByteArray *KitTagValue); // This method deserializes the Kit Information
                                                               // and stores it in the CLeicaReagentKit
    bool LotNumberExists(const int Lotnumber);
    void DecrementBottleCount();

    /******************** INLINE FuNCTIONS **************************************/
    /****************************************************************************/
    /*!
     *  \brief Get's the Type ID of the Kit
     *
     *  \return Type Id String
     */
    /****************************************************************************/
    QString GetTypeID() {return m_TypeID;}

    /****************************************************************************/
    /*!
     *  \brief Get's the Program ID of the Kit
     *
     *  \return Program Id String
     */
    /****************************************************************************/
    QString GetProgramID() {return m_ProgramID;}

    /****************************************************************************/
    /*!
     *  \brief Get's the Number of Reagents in the kit
     *
     *  \return number of reagents
     */
    /****************************************************************************/
    int GetReagentBottleCount() {return m_ReagentBottleCount;}

    /****************************************************************************/
    /*!
     *  \brief Get's the Open Shell Life Time of the Reagent in Kit
     *
     *  \return Open Shell Life Time
     */
    /****************************************************************************/
    int GetOpenShellLifetime() {return m_OpenShellLifeTime;}

    /****************************************************************************/
    /*!
     *  \brief Get's the MaxSlides of the Reagent in Kit
     *
     *  \return MaxSlides
     */
    /****************************************************************************/
    int GetMaxSlides() {return m_MaxSlides;}

    /****************************************************************************/
    /*!
     *  \brief Gets the Best before date
     *
     *  \return Best Before date
     */
    /****************************************************************************/
    QString GetBestBeforeDate() {return m_BestBeforeDate;}



};
}

#endif // LEICAREAGENT_KIT_H
