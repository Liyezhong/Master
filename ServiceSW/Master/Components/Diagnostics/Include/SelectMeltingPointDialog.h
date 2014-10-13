/****************************************************************************/
/*! \file SelectMeltingPointDialog.h
 *
 *  \brief Declaration of select paraffin melting point.
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

#ifndef DIAGNOSTICS_SELECTMELTINGPOINTDIALOG_H
#define DIAGNOSTICS_SELECTMELTINGPOINTDIALOG_H

#include "MainMenu/Include/DialogFrame.h"
#include "MainMenu/Include/ScrollWheel.h"

namespace Diagnostics {

namespace Ui {
class CSelectMeltingPointDialog;
}

/****************************************************************************/
/**
 * \brief This dialog is to select paraffin melting point.
 */
/****************************************************************************/
class CSelectMeltingPointDialog : public MainMenu::CDialogFrame
{
    Q_OBJECT

public:
    /****************************************************************************/
    /*!
     *  \brief Constructor
     *  \iparam MeltingPoint = The default melting point
     *  \iparam p_Parent = Parent widget
     */
    /****************************************************************************/
    explicit CSelectMeltingPointDialog(int MeltingPoint, QWidget *p_Parent = 0);

    /****************************************************************************/
    /*!
     *  \brief Destructor
     */
    /****************************************************************************/
    ~CSelectMeltingPointDialog();

    /****************************************************************************/
    /*!
     *  \brief To get selected melting point
     *  \return the melting point
     */
    /****************************************************************************/
    int GetMeltingPoint() {
        return m_MeltingPoint;
    }

protected:
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function changeEvent
     *  \param p_Event = QEvent type parameter
     *  \return from changeEvent
     */
    /****************************************************************************/
    bool eventFilter(QObject *p_Object, QEvent *p_Event);

private Q_SLOTS:

    /****************************************************************************/
    /*!
     *  \brief Abort Dialog
     */
    /****************************************************************************/
    void AbortDialog();

private:
    Ui::CSelectMeltingPointDialog *mp_Ui;     //!< User interface
    MainMenu::CScrollWheel *mp_ScrollWheel;   //!< Temperature scroll wheel
    int m_MeltingPoint;                       //!< Melting point selected by user.

};


} // namespace Diagnostics
#endif // DIAGNOSTICS_SELECTMELTINGPOINTDIALOG_H

