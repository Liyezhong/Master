/****************************************************************************/
/*! \file DlgSelectTestOptions.cpp
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

#include "ServiceWidget/Include/DlgSelectTestOptions.h"
#include "ui_DlgSelectTestOptions.h"
#include "Global/Include/Utils.h"
#include <QButtonGroup>

namespace MainMenu {

CDlgSelectTestOptions::CDlgSelectTestOptions(QWidget *parent)
    : QWidget(parent),
      ui(new Ui::CDlgSelectTestOptions)
{
    ui->setupUi(this);
    m_pButtonGroup = new QButtonGroup();
    m_pButtonGroup->addButton(ui->radioBtnStationTest, 0);
    m_pButtonGroup->addButton(ui->radioBtnEndTest, 1);
    CONNECTSIGNALSIGNAL(m_pButtonGroup, buttonClicked(int), this, RadioBtnSelected(int));
}

CDlgSelectTestOptions::~CDlgSelectTestOptions(void)
{
    try
    {
        delete ui;
        delete m_pButtonGroup;
    }
    catch (...)
    {
    }
}

/****************************************************************************/
/*!
 *  \brief Event handler for change events
 *
 *  \iparam p_Event = Change event
 */
/****************************************************************************/

void CDlgSelectTestOptions::changeEvent(QEvent *p_Event)
{
    QWidget::changeEvent(p_Event);
    switch (p_Event->type()) {
        case QEvent::LanguageChange:
            ui->retranslateUi(this);
            break;
        default:
            break;
    }
}



} // namespace MainMenu
