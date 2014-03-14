/****************************************************************************/
/*! \file DlgUsbStatus.h
 *
 *  \brief Declaration of USB connection status dialog.
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

#ifndef DIAGNOSTICS_SYSTEM_DLGUSBSTATUS_H
#define DIAGNOSTICS_SYSTEM_DLGUSBSTATUS_H

#include <QString>
#include <QStringList>

#include "MainMenu/Include/DialogFrame.h"
#include "MainMenu/Include/BaseTable.h"

#include "Diagnostics/Include/System/FileListModel.h"


namespace Diagnostics {

namespace System {

namespace Ui {
class CDlgUsbStatus;
}

class CDlgUsbStatus : public MainMenu::CDialogFrame
{
    Q_OBJECT
    
public:
    explicit CDlgUsbStatus(QWidget *parent = 0);
    ~CDlgUsbStatus();

public Q_SLOTS:
    void SetText(const QString &Text);

    void SetStatus(const QString &Status);

    void SetFiles(const QStringList &FileList);

protected:
    void changeEvent(QEvent *p_Event);
    
private:
    Ui::CDlgUsbStatus    *ui;

    CFileListModel       *mp_FileList;
    MainMenu::CBaseTable *mp_TableFiles;
};


} // namespace System

} // namespace Diagnostics

#endif // DIAGNOSTICS_SYSTEM_DLGUSBSTATUS_H
