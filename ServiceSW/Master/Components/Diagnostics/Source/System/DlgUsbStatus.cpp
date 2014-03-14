/****************************************************************************/
/*! \file DlgUsbStatus.cpp
 *
 *  \brief Implementation of USB connection status dialog.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2013-05-29
 *   $Author:  $ R.Wu
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

#include "Diagnostics/Include/System/DlgUsbStatus.h"

#include <QDebug>

#include "ui_DlgUsbStatus.h"


namespace Diagnostics {

namespace System {

CDlgUsbStatus::CDlgUsbStatus(QWidget *parent) :
    MainMenu::CDialogFrame(parent),
    ui(new Ui::CDlgUsbStatus)
{
    QWidget *p_Content = GetContentFrame();
    ui->setupUi(p_Content);

    mp_FileList = new CFileListModel;
    mp_FileList->SetVisibleRows(6);

    mp_TableFiles = new MainMenu::CBaseTable;
    mp_TableFiles->setModel(mp_FileList);

    ui->scrollUsbFiles->SetContent(mp_TableFiles);

    mp_TableFiles->SetVisibleRows(6);
    mp_TableFiles->horizontalHeader()->show();

    (void)connect(ui->pushYes, SIGNAL(clicked()), this, SLOT(accept()) );
    (void)connect(ui->pushNo, SIGNAL(clicked()), this, SLOT(reject()) );
}

CDlgUsbStatus::~CDlgUsbStatus()
{
    try
    {
        delete mp_TableFiles;

        delete mp_FileList;

        delete ui;
    }
    catch (...) { }
}

void CDlgUsbStatus::SetText(const QString &Text)
{
    ui->labelText->setText(Text);
}

void CDlgUsbStatus::SetStatus(const QString &Status)
{
    ui->labelStatus->setText(Status);
}

void CDlgUsbStatus::SetFiles(const QStringList &FileList)
{
    mp_FileList->SetFileList(FileList);
}

/****************************************************************************/
/*!
 *  \brief Event handler for change events
 *
 *  \iparam p_Event = Change event
 */
/****************************************************************************/
void CDlgUsbStatus::changeEvent(QEvent *p_Event)
{
    MainMenu::CDialogFrame::changeEvent(p_Event);

    switch (p_Event->type())
    {
    case QEvent::LanguageChange:
    {
        ui->retranslateUi(this);
    }
        break;
    default:
        break;
    }
}

} // namespace System

} // namespace Diagnostics
