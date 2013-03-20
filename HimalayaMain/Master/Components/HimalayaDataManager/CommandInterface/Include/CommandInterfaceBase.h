/****************************************************************************/
/*! \file CommandInterfaceBase.h
 *
 *  \brief Command Interface definition
 *
 *  $Version:   $ 0.1
 *  $Date:      $ 2013-02-02
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
#ifndef DATAMANAGER_COMMANDINTERFACEBASE_H
#define DATAMANAGER_COMMANDINTERFACEBASE_H

#include <QObject>

namespace Threads {
class MasterThreadController;
}

namespace DataManager {

class DataContainer;

/****************************************************************************/
/*!
 * \brief Base class for all DataManager Command Interfaces
 */
/****************************************************************************/
class CommandInterfaceBase : public QObject
{
    Q_OBJECT
public:
    /****************************************************************************/
    /*!
     *  \brief Constructor
     *
     *  \param[in] masterThreadController reference to master thread controllor
     *  \param[in] dataContainer          reference to data container
     */
    /****************************************************************************/
    explicit CommandInterfaceBase(
            Threads::MasterThreadController &masterThreadController,
            DataContainer                   &dataContainer)
        : QObject(0)
        , m_MasterThreadController(&masterThreadController)
        , m_DataContainer(&dataContainer)
    {
    }

    virtual ~CommandInterfaceBase(void) {}

protected:
    Threads::MasterThreadController *m_MasterThreadController;
    DataContainer                   *m_DataContainer;

    virtual void RegisterCommands(void) = 0;

    virtual void UnRegisterCommands(void) {}
};

}// end of namespace DataManager

#endif // DATAMANAGER_COMMANDINTERFACEBASE_H
