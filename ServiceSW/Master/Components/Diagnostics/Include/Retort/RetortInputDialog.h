/****************************************************************************/
/*! \file RetortInputDialog.h
 *
 *  \brief Declaration of Retort Input Dialog.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2014-10-13
 *   $Author:  $ Arthur Li
 *
 *  \b Company:
 *
 *       Leica Biosystems R&D Center Shanghai.
 *
 *  (C) Copyright 2014 by LBS R&D Center Shanghai. All rights reserved.
 *  This is unpublished proprietary source code of Leica. The copyright notice
 *  does not evidence any actual or intended publication.
 *
 */
/****************************************************************************/
#ifndef RETORTINPUTDIALOG_H
#define RETORTINPUTDIALOG_H
#include "Diagnostics/Include/DiagnosticMessageDlg.h"
#include "MainMenu/Include/DialogFrame.h"
#include "Core/Include/ServiceDefines.h"
#include <KeyBoard/Include/KeyBoard.h>
#include <QDialog>

namespace Ui {
class RetortInputDialog;
}


/****************************************************************************/
/**
 * \brief This dialog is started, when the user has to wait for the result of
 *        a background process.
 */
/****************************************************************************/
class RetortInputDialog : public MainMenu::CDialogFrame
{
    Q_OBJECT
    
public:
    /****************************************************************************/
    /*!
     *  \brief Constructor
     *  \iparam p_Parent = Parent widget
     */
    /****************************************************************************/
    explicit RetortInputDialog(QWidget *p_Parent = 0);

    /****************************************************************************/
    /*!
     *  \brief Destructor
     */
    /****************************************************************************/
    ~RetortInputDialog();

    /****************************************************************************/
    /*!
     *  \brief To set this dialog title.
     *  \iparam Title = the string of title.
     */
    /****************************************************************************/
    void SetTitle(QString Title);

    /****************************************************************************/
    /*!
     *  \brief To get edit content.
     *  \iparam text = the string of edit.
     */
    /****************************************************************************/
    void getEdit(QString &text);
private slots:
    /****************************************************************************/
    /*!
     *  \brief This function is called when OK is clicked
     *  \iparam EnteredString = Stores line edit string
     */
    /****************************************************************************/
    void OnOkClicked(QString EnteredString);

    /****************************************************************************/
    /*!
     *  \brief This function hides the keyboard when Esc is clicked
     */
    /****************************************************************************/
    void OnESCClicked();

    /****************************************************************************/
    /*!
     *  \brief To do Ok action.
     */
    /****************************************************************************/
    void clickOk(void);
private:
    void ShowMessage(QString title, QString  text);
protected:
    bool eventFilter(QObject *p_Object, QEvent *p_Event);

    /****************************************************************************/
    /*!
     *  \brief To connect key board signal and slots.
     */
    /****************************************************************************/
    void ConnectKeyBoardSignalSlots();

    /****************************************************************************/
    /*!
     *  \brief To disconnect key board signal and slots.
     */
    /****************************************************************************/
    void DisconnectKeyBoardSignalSlots();

private:
    Ui::RetortInputDialog *ui; //!< Retort Dialog ui
    KeyBoard::CKeyBoard *mp_KeyBoardWidget;     //!< Keyboard widget
    QString m_LineEditString;                   //!< Stores user input value
    QString _title; //!< dialog title
};

#endif // RETORTINPUTDIALOG_H

