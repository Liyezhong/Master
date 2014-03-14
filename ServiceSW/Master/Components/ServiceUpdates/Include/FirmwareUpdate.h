/****************************************************************************/
/*! \file FirmwareUpdate.h
 *
 *  \brief
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2013-05-08
 *   $Author:  $ Soumya. D
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

private Q_SLOTS:
    void UpdateFirmware(void);

private:
    Ui::CFirmwareUpdate *mp_Ui;
    MainMenu::CBaseTable *mp_TableWidget;
    QStandardItemModel m_Model;

};

} // end namespace ServiceUpdates

#endif //SERVICEUPDATES_FIRMWAREUPDATE_H
