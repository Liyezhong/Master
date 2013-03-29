/****************************************************************************/
/*! \file DashboardDateTimeWidget.cpp
 *
 *  \brief DashboardDateTimeWidget implementation.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 27-Mar-2013
 *   $Author:  $ Santhosha Kumar Raju
 *
 *  \b Company:
 *
 *       Leica Microsystems Ltd. Shanghai.
 *
 *  (C) Copyright 2012 by Leica Microsystems Shanghai. All rights reserved.
 *  This is unpublished proprietary source code of Leica. The copyright notice
 *  does not evidence any actual or intended publication.
 *
 */
/****************************************************************************/

#include "Dashboard/Include/DashboardDateTimeWidget.h"
#include <Global/Include/AdjustedTime.h>
#include "Application/Include/LeicaStyle.h"

#include "ui_DashboardDateTimeWidget.h"
#include <QDebug>
#include <QPainter>

namespace Dashboard {

/****************************************************************************/
/*!
 *  \brief Constructor
 *
 *  \iparam p_Parent = Parent object
 */
/****************************************************************************/
CDashboardDateTimeWidget::CDashboardDateTimeWidget(QString ProgramName, QWidget *p_Parent) : MainMenu::CPanelFrame(p_Parent),
    mp_Ui(new Ui::CDashboardDateTimeWidget)
{

    mp_Ui->setupUi(GetContentFrame());
    mp_Ui->wdgtDateTime->DisableApplyButton();
    mp_Ui->wdgtDateTime->RefreshDateTime(Global::TIME_24);

    m_selDateTime = mp_Ui->wdgtDateTime->GetDateTime();

    QString PanelTitle = QString(tr("End Time for Program \"%1\"").arg(ProgramName));
    SetPanelTitle(PanelTitle);
    SetTitleCenter();

    mp_Ui->lblDateTimeDisplay->setText(m_selDateTime.toString());


    CONNECTSIGNALSLOT(mp_Ui->btnCancel, clicked(), this, OnCancel());
    CONNECTSIGNALSLOT(mp_Ui->btnASAP, clicked(), this, close()); // to do
    CONNECTSIGNALSLOT(mp_Ui->btnOK, clicked(), this, OnOK());
}

/****************************************************************************/
/*!
 *  \brief Destructor
 */
/****************************************************************************/
CDashboardDateTimeWidget::~CDashboardDateTimeWidget()
{
    try {
       delete mp_Ui;
    }
    catch (...) {
        // to please Lint.
    }
}

void CDashboardDateTimeWidget::OnOK()
{
   m_selDateTime = mp_Ui->wdgtDateTime->GetDateTime();
   emit OnSelectDateTime(m_selDateTime);
   close();
}



QDateTime & CDashboardDateTimeWidget::GetChosenProgamEndTime()
{
    return m_selDateTime;
}


void CDashboardDateTimeWidget::OnCancel()
{
    close();
}


} // end namespace Dashboard
