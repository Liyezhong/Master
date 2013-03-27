/****************************************************************************/
/*! \file ModifyReagentRMSDlg.h
 *
 *  \brief ModifyReagentDlg definition.
 *
 *   $Version: $ 0.2
 *   $Date:    $ 2011-08-22, 2013-01-17
 *   $Author:  $ M.Scherer, C.Adaragunchi, Swati Tiwari
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

#ifndef REAGENTS_MODIFYREAGENTRMSDLG_H
#define REAGENTS_MODIFYREAGENTRMSDLG_H

#include "HimalayaDataContainer/Containers/Reagents/Include/Reagent.h"
#include "MainMenu/Include/DialogFrame.h"
#include "MainMenu/Include/ScrollWheel.h"
#include "KeyBoard/Include/KeyBoardObserver.h"
#include "KeyBoard/Include/KeyBoard.h"
#include "Reagents/Include/ReagentRMSWidget.h"
#include "Core/Include/ReagentGroupModel.h"
#include <QButtonGroup>

//lint -sem(Reagents::CModifyReagentRMSDlg::Init,initializer)
//lint -e1565
namespace Reagents {
        #define MAX_LONGNAME_LENGTH 32  //!< Maximum length reagent long name
        #define MIN_LONGNAME_LENGTH 2   //!< Manimum length reagent long name

namespace Ui {
    class CModifyReagentRMSDlg;
}


/****************************************************************************/
/**
 * \brief Edit dialog for  reagents of the user
 */
/****************************************************************************/
class CModifyReagentRMSDlg : public MainMenu::CDialogFrame, public KeyBoard::CKeyBoardObserver
{
    Q_OBJECT
    friend class CTestReagents;

private:
    Ui::CModifyReagentRMSDlg *mp_Ui;                   //!< User interface
    KeyBoard::CKeyBoard *mp_KeyBoardWidget;         //!< Reference to Keyboard widget
    MainMenu::CMainWindow *mp_MainWindow;           //!< Reference to main window.
    Core::CDataConnector *mp_DataConnector;         //!< DataConnector local object
    DataManager::CReagent m_Reagent;                //!< Local reagent object    
    KeyBoard::ValidationType_t m_ValidationType;    //!< Keyboard input type validation
    bool m_ReagentNameBtnClicked;                   //!< Reagent Name btn clicked status
    bool m_CassetteValueBtnClicked;                 //!< Reagent cassette value btn clicked status
    Reagents::ButtonType_t m_ButtonType;            //!< Informs which button was clicked
    bool m_ProcessRunning;                          //!< Process running state
    DataManager::CDataReagentList m_ReagentCloneList;   //!< ReagentList local object
    MainMenu::CMessageDlg m_MessageDlg;             //! Message dialog object
    MainMenu::CBaseTable *mp_TableWidget;           //!< Reagent table
    Core::CReagentGroupModel m_ReagentGroupModel;             //!< Model for the table
    DataManager::CDataReagentGroupList *mp_ReagentGroupList;  //!< Reagent list
    Global::RMSOptions_t m_RMSOption;
    bool m_SelectionFlag;


public:
    explicit CModifyReagentRMSDlg(QWidget *p_Parent = NULL, KeyBoard::CKeyBoard *p_KeyBoard = NULL,
                               MainMenu::CMainWindow *p_MainWindow = NULL,
                               Core::CDataConnector *p_DataConnector= NULL);
    virtual ~CModifyReagentRMSDlg();
    void InitDialog(DataManager::CReagent const *p_Reagent,
                    const DataManager::CDataReagentGroupList *p_ReagentGroupList,
                    Global::RMSOptions_t Option);
    void Update();
    void UpdateOnESC();
    void NewReagent();
    void EnableElements(bool m_Enable);

    /****************************************************************************/
    /*!
     *  \brief Informs which button was clicked on Menu reagent screen
     *
     *  \iparam ButtonType - Edit/New/Copy
     */
    /****************************************************************************/
    void SetButtonType(ButtonType_t ButtonType) { m_ButtonType = ButtonType;}

private:
    void Init();
    void RetranslateUI();
    void ResizeHorizontalSection();

private slots:
    void OnOk();
    void OnCancel();
    void OnEditName();
    void OnEditCassetteValue();
    void OnOkClicked();
    void SelectionChanged(QModelIndex Index);

protected:
    void changeEvent(QEvent *p_Event);

signals:
    /****************************************************************************/
    /*!
     *  \brief This signal is emitted to update the selected reagent.
     *
     *  \iparam Reagent = Selected reagent.
     */
    /****************************************************************************/
    void UpdateReagent(DataManager::CReagent &Reagent);

    /****************************************************************************/
    /*!
     *  \brief This signal is emitted to add new reagent.
     *
     *  \iparam Reagent = New reagent.
     */
    /****************************************************************************/
    void AddReagent(DataManager::CReagent &Reagent);

    /****************************************************************************/
    /*!
     *  \brief This signal is emitted when Cancel button in the dailog is pressed.
     *
     */
    /****************************************************************************/
    void CancelPressed();
};

} // end namespace Reagents

#endif // REAGENTS_MODIFYREAGENTDLG_H
