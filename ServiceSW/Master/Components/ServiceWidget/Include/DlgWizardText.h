/****************************************************************************/
/*! \file DlgWizardText.h
 *
 *  \brief Declaration of text wizard dialog.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2013-05-28
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

#ifndef MAINMENU_DLGWIZARDTEXT_H
#define MAINMENU_DLGWIZARDTEXT_H

#include <QLabel>

#include "ServiceWidget/Include/DlgWizardBase.h"

namespace MainMenu {

class CDlgWizardText : public CDlgWizardBase
{
    Q_OBJECT

public:
    explicit CDlgWizardText(QWidget *parent = 0);
    ~CDlgWizardText(void);

public Q_SLOTS:
    void SetText(const QString &Text);

private:
    QLabel *mp_Text;
};

} // namespace MainMenu

#endif // MAINMENU_DLGWIZARDTEXT_H
