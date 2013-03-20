/****************************************************************************/
/*! \file Dashboard/Source/DashboardWidget.cpp
 *
 *  \brief Implementation file for class DashboardWidget.
 *
 *  $Version:   $ 0.2
 *  $Date:      $ 2013-01-03, 08-Feb-2013
 *  $Author:    $ Swati Tiwari, Santhosha Kumar Raju
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
#include "Global/Include/Utils.h"
#include "Dashboard/Include/DashboardWidget.h"
#include "ui_DashboardWidget.h"
#include <QDebug>

namespace Dashboard {

CDashboardWidget::CDashboardWidget(Core::CDataConnector *p_DataConnector,
                                   MainMenu::CMainWindow *p_Parent): MainMenu::CPanelFrame(p_Parent),
                                   mp_Ui(new Ui::CDashboardWidget),mp_Parent(p_Parent),
                                   mp_DataConnector(p_DataConnector)
{
     mp_Ui->setupUi(GetContentFrame());
     SetPanelTitle(tr("Dashboard"));

     QPalette Palette = mp_Ui->dashboardView->palette();
     QColor BaseColor = Palette.color(QPalette::Base);
     BaseColor.setAlpha(0);
     Palette.setColor(QPalette::Base, BaseColor);
     mp_Ui->dashboardView->setPalette(Palette);

     mp_DashboardScene = new CDashboardScene(mp_DataConnector, this, mp_Parent);
     mp_Ui->dashboardView->setScene(mp_DashboardScene);

     mp_Separator = new QFrame();
     mp_Separator->setParent(this);  // Set Parent of this Frame as the Dashboard Widget.
     DrawSeparatorLine();

     AddItemsToComboBox();


     CONNECTSIGNALSLOT(mp_Ui->pgmsComboBox, currentIndexChanged(int), mp_Ui->pgmsComboBox, handleSelectionChanged(int));

}

CDashboardWidget::~CDashboardWidget()
{
    try {
        delete mp_Separator;
        delete mp_DashboardScene;
        delete mp_Ui;
    } catch(...) {

    }
}

void CDashboardWidget::DrawSeparatorLine()
{

    mp_Separator->setFrameShape(QFrame::HLine);
    mp_Separator->setLineWidth(2);
    mp_Separator->setFrameShadow(QFrame::Plain);
    mp_Separator->setGeometry(4,460,685,2);
    mp_Separator->show();
}

void CDashboardWidget::AddItemsToComboBox()
{
    mp_Ui->pgmsComboBox->insertItem(0, QIcon(QPixmap(QString(":/HimalayaImages/Icons/MISC/Icon_Leica.png"))), "LeicaPrgm1");
    mp_Ui->pgmsComboBox->insertItem(1, QIcon(QPixmap(QString(":/HimalayaImages/Icons/MISC/Icon_Leica.png"))), "LeicaPrgm2");
    mp_Ui->pgmsComboBox->insertItem(2, QIcon(QPixmap(QString(":/HimalayaImages/Icons/MISC/Icon_Leica.png"))), "LeicaPrgm3");
    mp_Ui->pgmsComboBox->insertItem(3, QIcon(QPixmap(QString(":/HimalayaImages/Icons/MISC/TickOk.png"))), "UserPrgm1");
    mp_Ui->pgmsComboBox->insertItem(4, QIcon(QPixmap(QString(":/HimalayaImages/Icons/MISC/TickOk.png"))), "UserPrgm2");
    mp_Ui->pgmsComboBox->insertItem(5, QIcon(QPixmap(QString(":/HimalayaImages/Icons/MISC/TickOk.png"))), "UserPrgm3");
}


} // End of namespace Dashboard
