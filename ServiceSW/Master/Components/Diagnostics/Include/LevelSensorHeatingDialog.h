/****************************************************************************/
/*! \file LevelSensorHeatingDialog.h
 *
 *  \brief Declaration of Level sensor heating.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2014-10-09
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

#ifndef DIAGNOSTICS_LEVELSENSORHEATINGDIALOG_H
#define DIAGNOSTICS_LEVELSENSORHEATINGDIALOG_H

#include "MainMenu/Include/DialogFrame.h"

namespace Diagnostics {

namespace Ui {
class CLevelSensorHeatingDialog;
}

/****************************************************************************/
/**
 * \brief This dialog is to heating level sensor.
 */
/****************************************************************************/
class CLevelSensorHeatingDialog : public MainMenu::CDialogFrame
{
    Q_OBJECT

public:
    /****************************************************************************/
    /*!
     *  \brief Constructor
     *  \iparam MeltingPoint = The default melting point
     *  \iparam parent = Parent widget
     */
    /****************************************************************************/
    explicit CLevelSensorHeatingDialog(bool XyleneFlag, QWidget *parent = 0);

    /****************************************************************************/
    /*!
     *  \brief Destructor
     */
    /****************************************************************************/
    ~CLevelSensorHeatingDialog();

    /****************************************************************************/
    /*!
     *  \brief To set this dialog title.
     *  \iparam Title = the string of title.
     */
    /****************************************************************************/
    void SetTitle(QString& Title);

    /****************************************************************************/
    /*!
     *  \brief To start heating.
     */
    /****************************************************************************/
    bool StartHeating();

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

    /****************************************************************************/
    /*!
     *  \brief Slot for Update UI
     *  \iparam UsedTime = the used time
     *  \iparam EDTime   = the Estimated Duration time
     *  \iparam TargetTem = the heating target temperature.
     *  \iparam CurrentTemp = the heating current temp.
     */
    /****************************************************************************/
    void UpdateUI(int UsedTime, int EDTime, qreal TargetTemp, qreal CurrentTemp);

private:
    Ui::CLevelSensorHeatingDialog *mp_Ui;     //!< User interface
    bool m_XyleneFlag;                       //!< true-Xylene, false-other reagents.
    bool m_Abort;                             //!< Flag for abort.

};


} // namespace Diagnostics
#endif // DIAGNOSTICS_LEVELSENSORHEATINGDIALOG_H

