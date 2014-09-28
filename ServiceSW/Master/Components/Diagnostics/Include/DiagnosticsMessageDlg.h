/****************************************************************************/
/*! \file DiagnosticsMessageDlg.h
 *
 *  \brief Declaration of show message dialog for diagnostics.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2014-09-16
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

#ifndef DIAGNOSTICS_MESSAGEDLG_H
#define DIAGNOSTICS_MESSAGEDLG_H

#include "MainMenu/Include/MessageDlg.h"

namespace Diagnostics {

/****************************************************************************/
/**
 * \brief This dialog is to show message for diagnostics.
 */
/****************************************************************************/
class CDiagnosticsMessageDlg : public QObject
{
    Q_OBJECT

public:
    /****************************************************************************/
    /*!
     *  \brief Constructor
     *  \iparam parent = Parent widget
     */
    /****************************************************************************/
    explicit CDiagnosticsMessageDlg(QWidget *parent = 0);

    /****************************************************************************/
    /*!
     *  \brief Destructor
     */
    /****************************************************************************/
    virtual ~CDiagnosticsMessageDlg();

public slots:

private:
    MainMenu::CMessageDlg   *mp_MessageDlg;
};


} // namespace Diagnostics
#endif // DIAGNOSTICS_SELECTMELTINGPOINTDIALOG_H

