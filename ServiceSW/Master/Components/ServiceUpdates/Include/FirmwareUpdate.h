/****************************************************************************/
/*! \file FirmwareUpdate.h
 *
 *  \brief CFirmwareUpdate class definition file
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2014-07-10
 *   $Author:  $ Dixiong Li
 *
 *  \b Company:
 *
 *       Leica Biosystems Shanghai.
 *
 *  (C) Copyright 2010 by Leica Biosystems Shanghai. All rights reserved.
 *  This is unpublished proprietary source code of Leica. The copyright notice
 *  does not evidence any actual or intended publication.
 *
 */
/****************************************************************************/

#ifndef SERVICEUPDATES_FIRMWAREUPDATE_H
#define SERVICEUPDATES_FIRMWAREUPDATE_H

#include <QWidget>
#include "MainMenu/Include/BaseTable.h"
#include <QStandardItemModel>

namespace ServiceUpdates {

namespace Ui {
    class CFirmwareUpdate;
}

/****************************************************************************/
/**
 * \brief This widget displays history of ModuleList
 */
/****************************************************************************/
class CFirmwareUpdate : public QWidget
{
    Q_OBJECT

public:
    explicit CFirmwareUpdate(QWidget *p_Parent = 0);
    ~CFirmwareUpdate(void);

    /****************************************************************************/
    /*!
     *  \brief  To add data item to the table
     *  \iparam Slave = ASB name
     *  \iparam SoftwareVersionInfo = Software version Info
     */
    /****************************************************************************/
    void AddItem(QString& Slave, QString& CurrentVersion, QString& LatestVersion);

protected:
    /****************************************************************************/
    /*!
     *  \brief Event handler for change events
     *
     *  \iparam p_Event = Change event
     */
    /****************************************************************************/
    void changeEvent(QEvent *p_Event);

private Q_SLOTS:
    void UpdateFirmware(void);

    /****************************************************************************/
    /*!
     *  \brief Translates the strings in UI to the selected language
     */
    /****************************************************************************/
    void RetranslateUI();

public slots:
    /****************************************************************************/
    /*!
     *  \brief slot for Refresh latest version from firmware config file
     */
    /****************************************************************************/
    void RefreshLatestVersion();

private:
    Ui::CFirmwareUpdate *mp_Ui;             //!< User Interface
    MainMenu::CBaseTable *mp_TableWidget;   //!< Base Table widget
    QStandardItemModel m_Model;             //!< Model for the table

    /****************************************************************************/
    /*!
     *  \brief Initialization data from config file.
     */
    /****************************************************************************/
    void InitData();

    /****************************************************************************/
    /*!
     *  \brief To find item by slave name
     *  \iparam Slave = slave name
     *  \result pointer of QStandardIterm
     */
    /****************************************************************************/
    QStandardItem* FindLastVersionItem(QString& Slave);

};

} // end namespace ServiceUpdates

#endif //SERVICEUPDATES_FIRMWAREUPDATE_H
