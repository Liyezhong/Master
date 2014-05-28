/****************************************************************************/
/*! \file CurrentConfigParameterDlg.cpp
 *
 *  \brief Current Configuration Parameter Dlg definition.
 *
 *  \b Description:
 *          This class displays Parameters of selected submodule of
 *          current system configuration.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2014-05-27
 *   $Author:  $ Dixiong.li
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

#include "SystemTracking/Include/CurrentConfigParameterDlg.h"
#include "ui_CurrentConfigParameterDlg.h"

namespace SystemTracking {

const int FIXED_TABLEWIDGET_WIDTH = 380;        ///< Table widget width
const int FIXED_TABLEWIDGET_HEIGHT = 320;       ///< Table widget height

/****************************************************************************/
/**
 * \brief Constructor
 * \iparam p_Parent = Parent widget
 */
/****************************************************************************/
CCurrentConfigParameterDlg::CCurrentConfigParameterDlg(QWidget *p_Parent)
    : MainMenu::CDialogFrame(p_Parent),
      mp_Ui(new Ui::CCurrentConfigParameterDlg)
{
    mp_Ui->setupUi(GetContentFrame());
    mp_TableWidget = new MainMenu::CBaseTable();
    mp_TableWidget->resize(FIXED_TABLEWIDGET_WIDTH, FIXED_TABLEWIDGET_HEIGHT);


    mp_TableWidget->setModel(&m_Model);
    mp_TableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    mp_TableWidget->setSelectionMode(QAbstractItemView::NoSelection);

    mp_Ui->widget->setMinimumSize(mp_TableWidget->width(), mp_TableWidget->height());
    mp_Ui->widget->SetContent(mp_TableWidget);

    CONNECTSIGNALSLOTGUI(mp_Ui->okBtn, clicked(), this, close());
}

/****************************************************************************/
/**
 * \brief Destructor
 */
/****************************************************************************/
CCurrentConfigParameterDlg::~CCurrentConfigParameterDlg()
{
    try {
        delete mp_TableWidget;
        delete mp_Ui;
    }
    catch (...) {
        // to please Lint
    }
}

/****************************************************************************/
/*!
 *  \brief Event handler for change events
 *
 *  \iparam p_Event = Change event
 */
/****************************************************************************/
void CCurrentConfigParameterDlg::changeEvent(QEvent *p_Event)
{
    QWidget::changeEvent(p_Event);
    switch (p_Event->type()) {
    case QEvent::LanguageChange:
        mp_Ui->retranslateUi(this);
        RetranslateUI();
        break;
    default:
        break;
    }
}

/****************************************************************************/
/*!
 *  \brief  To initialize the pop up dialog
 *  \iparam p_Parameter = Pointer to Parameter Struct
 */
/****************************************************************************/
void CCurrentConfigParameterDlg::InitDialog(ServiceDataManager::Parameter_t *p_Parameter)
{
    AddItem(p_Parameter->ParameterName, p_Parameter->ParameterValue);
}

/****************************************************************************/
/*!
 *  \brief  To add data item to the table
 *  \iparam ParamName = Name of the Parameter
 *  \iparam ParamValue = Value of the Parameter
 */
/****************************************************************************/
void CCurrentConfigParameterDlg::AddItem(QString ParamName, QString ParamValue)
{

    QList<QStandardItem *> ItemList;
    if (ItemList.count() != 0) {
        qDeleteAll(ItemList);
    }
    ItemList << new QStandardItem(ParamName);
    ItemList << new QStandardItem(ParamValue);
    mp_TableWidget->horizontalHeader()->resizeSection(0, 180);      // 0 => Index  180 => Size
    mp_TableWidget->horizontalHeader()->resizeSection(1, 150);      // 1 => Index  150 => Size
    m_Model.appendRow(ItemList);
}

/****************************************************************************/
/*!
 *  \brief  To clear the table model
 */
/****************************************************************************/
void CCurrentConfigParameterDlg::ClearModel()
{
    m_Model.clear();
}

/****************************************************************************/
/*!
 *  \brief Translates the strings in UI to the selected language
 */
/****************************************************************************/
void CCurrentConfigParameterDlg::RetranslateUI()
{

}

}   // end of namespace SystemTracking

