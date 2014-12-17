/****************************************************************************/
/*! \file SVCValveStateInfoDlg.cpp
 *
 *  \brief Implementation of valve state info dialog.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2014-12-02
 *   $Author:  $ Dixiong Li
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

#include "SVCDiagnostics/Include/SVCValveStateInfoDlg.h"
#include "ui_SVCValveStateInfoDlg.h"
#include "Global/Include/Utils.h"

namespace SVCDiagnostics {

CSVCValveStateInfoDlg::CSVCValveStateInfoDlg(bool PumpState, bool Valve1State, bool Valve2State, QWidget* p_Parent):
    MainMenu::CDialogFrame(p_Parent),
    mp_Ui(new Ui::CSVCValveStateInfoDlg)
{
    mp_Ui->setupUi(GetContentFrame());
    setModal(true);

    this->SetDialogTitle("Valve State Info");

    mp_TableWidget = new MainMenu::CBaseTable;

    mp_TableWidget->resize(310, 240);

    mp_TableWidget->horizontalHeader()->show();

    mp_TableWidget->setModel(&m_Model);

    m_Model.setHorizontalHeaderLabels(QStringList()<<tr("")<<tr("Pressure")<<tr("Vaccum")<<tr("Ambient"));

    AddItem("Pump", "On", "On", "Off");
    AddItem("Valve1", "On", "Off", "Off");
    AddItem("Valve2", "Off", "On", "Off");

    mp_TableWidget->horizontalHeader()->resizeSection(0, 70);
    mp_TableWidget->horizontalHeader()->resizeSection(1, 70);
    mp_TableWidget->horizontalHeader()->resizeSection(2, 70);
    mp_TableWidget->horizontalHeader()->resizeSection(3, 70);

//    mp_TableWidget->verticalHeader()->resizeSection(0,150);
//    mp_TableWidget->verticalHeader()->resizeSection(1,150);

    mp_TableWidget->setSelectionMode(QAbstractItemView::NoSelection);
    mp_TableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    mp_Ui->widget->setMinimumSize(mp_TableWidget->width(), mp_TableWidget->height());
    mp_Ui->widget->SetContent(mp_TableWidget);

    CONNECTSIGNALSLOT(mp_Ui->okBtn, clicked(), this, accept());
}

CSVCValveStateInfoDlg::~CSVCValveStateInfoDlg()
{
    try {
        delete mp_Ui;
        delete mp_TableWidget;
    }
    catch (...) {

    }
}

void CSVCValveStateInfoDlg::AddItem(const QString& Name, const QString& State1, const QString& State2, const QString& State3)
{
    QList<QStandardItem *> ItemList;
    ItemList << new QStandardItem(Name);
    ItemList << new QStandardItem(State1);
    ItemList << new QStandardItem(State2);
    ItemList << new QStandardItem(State3);

    m_Model.appendRow(ItemList);
}

}
