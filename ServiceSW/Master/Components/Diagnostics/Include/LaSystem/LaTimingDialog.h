/****************************************************************************/
/*! \file LaTimingDialog.h
 *
 *  \brief Declaration of  L & A Timing Dialog.
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
#ifndef LATIMINGDIALOG_H
#define LATIMINGDIALOG_H

#include "MainMenu/Include/DialogFrame.h"
#include "Core/Include/ServiceDefines.h"
#include <QDialog>

namespace Ui {
class LaTimingDialog;
}

class LaTimingDialog : public MainMenu::CDialogFrame
{
    Q_OBJECT
    
public:
    /****************************************************************************/
    /*!
     *  \brief Constructor
     *  \iparam parent = Parent widget
     */
    /****************************************************************************/
    explicit LaTimingDialog(QWidget *parent = 0);

    /****************************************************************************/
    /*!
     *  \brief Destructor
     */
    /****************************************************************************/
    ~LaTimingDialog();

    /****************************************************************************/
    /*!
     *  \brief To set this dialog title.
     *  \iparam Title = the string of title.
     */
    /****************************************************************************/
    void SetTitle(QString Title);

    /****************************************************************************/
    /*!
     *  \brief Update label status
     *
     *  \iparam Status = Label test status
     */
    /****************************************************************************/
    void UpdateLabel(const Service::ModuleTestStatus &Status);

private:
    Ui::LaTimingDialog *ui;
};

#endif // LATIMINGDIALOG_H
