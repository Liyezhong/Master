/****************************************************************************/
/*! \file CurrentConfigParameterDlg.h
 *
 *  \brief Definition file for class CCurrentConfigParameterDlg.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2014-5-27
 *   $Author:  $ Dixiong.li
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
#ifndef CURRENTCONFIGPARAMETERDLG_H
#define CURRENTCONFIGPARAMETERDLG_H

#include "ServiceDataManager/Include/SubModule.h"
#include "MainMenu/Include/DialogFrame.h"
#include "MainMenu/Include/BaseTable.h"
#include "Global/Include/Utils.h"
#include <QStandardItemModel>
#include <QWidget>
#include <QEvent>

namespace SystemTracking {

namespace Ui {
class CCurrentConfigParameterDlg;
}

/****************************************************************************/
/**
 * \brief This is the dialog for displaying sub module information.
 */
/****************************************************************************/
class CCurrentConfigParameterDlg : public MainMenu::CDialogFrame
{
    Q_OBJECT

public:
    /****************************************************************************/
    /**
     * \brief  Default constructor.
     * \iparam p_Parent = parent of widget
     */
    /****************************************************************************/
    CCurrentConfigParameterDlg(QWidget *p_Parent = NULL);

    /****************************************************************************/
    /**
     * \brief Destructor.
     */
    /****************************************************************************/
    ~CCurrentConfigParameterDlg();

    /****************************************************************************/
    /*!
     *  \brief  To add data item to the table
     *  \iparam ParamName = Name of the Parameter
     *  \iparam ParamValue = Value of the Parameter
     */
    /****************************************************************************/
    void AddItem(const QString& ParamName, const QString& ParamValue);

    /****************************************************************************/
    /*!
     *  \brief  To initialize the pop up dialog
     *  \iparam p_Parameter = Pointer to Parameter Struct
     */
    /****************************************************************************/
    void InitDialog(ServiceDataManager::Parameter_t *p_Parameter);

    /****************************************************************************/
    /*!
     *  \brief  To clear the table model
     */
    /****************************************************************************/
    void ClearModel();

protected:
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function changeEvent
     *
     *  \param p_Event = QEvent type parameter
     *
     *  \return from changeEvent
     */
    /****************************************************************************/
    void changeEvent(QEvent *p_Event);

private:
    Ui::CCurrentConfigParameterDlg *mp_Ui;      //!< User Interface
    MainMenu::CBaseTable *mp_TableWidget;       //!< Table for submodule list
    QStandardItemModel m_Model;                 //!< Model for the table

private slots:
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function RetranslateUI
     *
     *  \return from RetranslateUI
     */
    /****************************************************************************/
    void RetranslateUI();
};

}   // end of namespace SystemTracking

#endif // CURRENTCONFIGPARAMETERDLG_H
