/****************************************************************************/
/*! \file ModifyReagentGroupColorDlg.cpp
 *
 *  \brief ModifyReagentGroupColorDlg Implementation.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2013-01-29
 *   $Author:  $ Swati Tiwari
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
#ifndef ModifyReagentGroupColorDlg_H
#define ModifyReagentGroupColorDlg_H


#include "HimalayaDataContainer/Containers/Reagents/Include/Reagent.h"
#include "MainMenu/Include/DialogFrame.h"
#include "MainMenu/Include/ScrollWheel.h"
#include "KeyBoard/Include/KeyBoard.h"
#include "Reagents/Include/ReagentStatusWidget.h"
#include "MainMenu/Include/ScrollTable.h"
#include "Core/Include/ReagentStationEditModel.h"

namespace Reagents {

namespace Ui {
    class CModifyReagentGroupColorDlg;
}

/****************************************************************************/
/**
 * \brief Edit dialog for  reagents of the user
 */
/****************************************************************************/
class CModifyReagentGroupColorDlg : public MainMenu::CDialogFrame
{
    Q_OBJECT

private:
    Ui::CModifyReagentGroupColorDlg *mp_Ui;                   //!< User interface
    MainMenu::CMessageDlg m_MessageDlg;             //! Message dialog object       ///<  Definition/Declaration of variable m_MessageDlg
    DataManager::CReagentGroup m_ReagentGroup;
    DataManager::CReagentGroupColorList *mp_CReagentGroupColorList;       ///<  Definition/Declaration of variable mp_CReagentGroupColorList
    QMap<int, QPushButton*> m_QPushButtonMap;       ///<  Definition/Declaration of variable m_QPushButtonMap
    //Flags
    bool m_ProcessRunning;                          //!< Process running state
    //UI related
    QStringList m_ColorNames;                       //!< store color names
    QButtonGroup m_ButtonGroup;       ///<  Definition/Declaration of variable m_ButtonGroup
    QString m_strCancel;       ///<  Definition/Declaration of variable m_strCancel
    QString m_strClose;       ///<  Definition/Declaration of variable m_strClose
    QString m_strOK;       ///<  Definition/Declaration of variable m_strOK
    void SetButtonGroup();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function RetranslateUI
     *
     *  \return from RetranslateUI
     */
    /****************************************************************************/
    void RetranslateUI();

public:
    explicit CModifyReagentGroupColorDlg(QWidget *p_Parent = NULL,
                               MainMenu::CMainWindow *p_MainWindow = NULL);
    virtual ~CModifyReagentGroupColorDlg();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function SetReagentGroupList
     *
     *  \return from SetReagentGroupList
     */
    /****************************************************************************/
    void SetReagentGroupList(DataManager::CDataReagentGroupList &ReagentGroupList, const DataManager::CReagentGroup &ReagentGroup);
    void SetReagentGroupColorListptr(DataManager::CReagentGroupColorList  *p_CReagentGroupColorList);
private slots:
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of slot UpdateReagentList
     */
    /****************************************************************************/
    void OnOk();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of slot UpdateReagentList
     */
    /****************************************************************************/
    void OnCancel();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of slot UpdateReagentList
     */
    /****************************************************************************/
    void OnButtonGroup(int Id);
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of slot UpdateReagentList
     */
    /****************************************************************************/
    void OnProcessStateChanged();

public slots:
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of slot UpdateReagentList
     */
    /****************************************************************************/
    void UpdateReagentGroupColor();
protected:
    void changeEvent(QEvent *p_Event);

signals:
    /****************************************************************************/
    /*!
     *  \brief This signal is emitted when Cancel button in the dailog is pressed.
     *
     */
    /****************************************************************************/
    void CancelPressed();

    /****************************************************************************/
    /*!
     *  \brief This signal is emitted when ReagentList is updated.
     *
     */
    /****************************************************************************/
    void UpdateReagentGroup(DataManager::CReagentGroup& ReagentGroup);

};
}

#endif // ModifyReagentGroupColorDlg_H
