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

#include "MainMenu/Include/DialogFrame.h"
#include "Core/Include/ServiceDefines.h"
#include <QDialog>

namespace Ui {
class RetortInputDialog;
}

class RetortInputDialog : public MainMenu::CDialogFrame
{
    Q_OBJECT
    
public:
    /****************************************************************************/
    /*!
     *  \brief Constructor
     *  \iparam parent = Parent widget
     */
    /****************************************************************************/
    explicit RetortInputDialog(QWidget *parent = 0);

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
    int getEdit(QString &text);
    
private:
    Ui::RetortInputDialog *ui;
};

#endif // RETORTINPUTDIALOG_H

