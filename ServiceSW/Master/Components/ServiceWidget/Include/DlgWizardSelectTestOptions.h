/****************************************************************************/
/*! \file DlgSelectTestOptions.h
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

#ifndef DLGWIZARDSELECTTESTOPTIONS_H
#define DLGWIZARDSELECTTESTOPTIONS_H

#include "ServiceWidget/Include/DlgWizardBase.h"

namespace MainMenu {

class CDlgSelectTestOptions;

class CDlgWizardSelectTestOptions : public CDlgWizardBase
{
    Q_OBJECT

public:
    explicit CDlgWizardSelectTestOptions(QWidget *parent = 0, QWidget *pMainWindow = 0);
    ~CDlgWizardSelectTestOptions(void);
signals:

private slots:
    void OnRadioBtnSelected(int radioBtnIndex);
    void OnClickedNextButton();
signals:
     void ClickedNextButton(int radioBtnIndex);

private:
    CDlgSelectTestOptions* m_pDlgSelectTestOptions;
    int m_radioBtnIndex;

};

} // namespace MainMenu

#endif // DLGWIZARDSELECTTESTOPTIONS_H
