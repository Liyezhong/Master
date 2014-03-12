/****************************************************************************/
/*! \file FirmwareUpdate.cpp
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

#include "ServiceUpdates/Include/FirmwareUpdate.h"

#include "Global/Include/Utils.h"

#include "ui_FirmwareUpdate.h"

namespace ServiceUpdates {

CFirmwareUpdate::CFirmwareUpdate(QWidget *p_Parent)
    : QWidget(p_Parent)
    , mp_Ui(new Ui::CFirmwareUpdate)
{
    mp_Ui->setupUi(this);

    m_Model.setHorizontalHeaderLabels(QStringList() << "Slave" << "Version Info" << "");

    mp_TableWidget = new MainMenu::CBaseTable;
    mp_TableWidget->resize(380,500);

    mp_TableWidget->horizontalHeader()->show();

    mp_TableWidget->setModel(&m_Model);
    mp_TableWidget->horizontalHeader()->resizeSection(0, 100);
    mp_TableWidget->horizontalHeader()->resizeSection(1, 350);
    mp_TableWidget->horizontalHeader()->resizeSection(2, 45);

    mp_TableWidget->verticalHeader()->resizeSection(0,55);
    mp_TableWidget->verticalHeader()->resizeSection(1,55);

    mp_TableWidget->setSelectionMode(QAbstractItemView::NoSelection);
    mp_TableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    mp_Ui->widget->SetContent(mp_TableWidget);

    CONNECTSIGNALSLOT(mp_Ui->refreshBtn,
                      clicked(),
                      this,
                      UpdateFirmware());
}

CFirmwareUpdate::~CFirmwareUpdate(void)
{
    try
    {
        delete mp_TableWidget;
        delete mp_Ui;
    }
    catch (...) {
        // to please Lint
    }
}

void CFirmwareUpdate::UpdateFirmware(void)
{

}

} // end namespace ServiceUpdates
