/****************************************************************************/
/*! \file Select110v220vDialog.h
 *
 *  \brief Header file for class CSelect110v220vDialog.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2014-06-18
 *   $Author:  $ Dixiong Li
 *
 *  \b Company:
 *
 *       Leica Biosystems Shanghai.
 *
 *  (C) Copyright 2010 by Leica Biosystems Shanghai. All rights reserved.
 *  This is unpublished proprietary source code of Leica. The copyright notice
 *  does not evidence any actual or intended publication.
 *
 */
/****************************************************************************/
#ifndef SELECT110V220VDIALOG_H
#define SELECT110V220VDIALOG_H

#include "MainMenu/Include/DialogFrame.h"
#include "Core/Include/ServiceDefines.h"
#include <QTimer>
#include <KeyBoard/Include/KeyBoard.h>
#include "MainMenu/Include/ScrollWheel.h"

namespace DiagnosticsManufacturing {

namespace Ui {
    class CSelect110v220vDialog;
}

class CSelect110v220vDialog : public MainMenu::CDialogFrame
{
    Q_OBJECT
public:
    /****************************************************************************/
    /*!
     *  \brief Constructor
     *
     *  \iparam DefaultVoltage = default voltage
     *
     *  \iparam p_Parent = Parent widget
     */
    /****************************************************************************/
    explicit CSelect110v220vDialog(int DefaultVoltage, QWidget *p_Parent = 0);

    /****************************************************************************/
    /*!
     *  \brief Destructor
     */
    /****************************************************************************/
    virtual ~CSelect110v220vDialog();

    /****************************************************************************/
    /*!
     *  \brief Get selected voltage
     */
    /****************************************************************************/
    int GetSelectedVoltage() const
    {
        return m_SelectedVoltage;
    }

private slots:

    /****************************************************************************/
    /*!
     *  \brief This function is called when radio button is clicked
     */
    /****************************************************************************/
    void OnRadioBtnSelected(int radioBtnIndex);

    /****************************************************************************/
    /*!
     *  \brief Connects signals and slots of keyboard.
     */
    /****************************************************************************/
    void RetranslateUI();

protected:
    /****************************************************************************/
    /*!
     *  \brief Event filter
     *  \iparam p_Object = object
     *  \iparam p_Event = event
     */
    /****************************************************************************/
    bool eventFilter(QObject *p_Object, QEvent *p_Event);

private:
    Ui::CSelect110v220vDialog *mp_Ui;     //!< User interface
    //MainMenu::CScrollWheel *mp_ScrollWheel;         //!< Temperature scroll wheel

    QButtonGroup* mp_ButtonGroup;
    int m_SelectedVoltage;             //!< selected voltage test 110v or 220v


    /****************************************************************************/
    /*!
     *  \brief Disable copy and assignment operator.
     *
     */
    /****************************************************************************/
    Q_DISABLE_COPY(CSelect110v220vDialog)

};

} // end namespace DiagnosticsManufacturing

#endif // SELECT110V220VDIALOG_H
