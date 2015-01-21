/****************************************************************************/
/*! \file SelectSealingDialog.h
 *
 *  \brief Header file for class CSelectSealingDialog.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2014-06-30
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
#ifndef SELECTSEALINGDIALOG_H
#define SELECTSEALINGDIALOG_H

#include "MainMenu/Include/DialogFrame.h"
#include "Core/Include/ServiceDefines.h"
#include <QTimer>
#include <KeyBoard/Include/KeyBoard.h>
#include "MainMenu/Include/ScrollWheel.h"

namespace DiagnosticsManufacturing {

namespace Ui {
    class CSelectSealingDialog;
}

/****************************************************************************/
/*!
 *  \brief This is the class of the diagnostics select sealing dialog
 */
/****************************************************************************/
class CSelectSealingDialog : public MainMenu::CDialogFrame
{
    Q_OBJECT
public:
    /****************************************************************************/
    /*!
     *  \brief Constructor
     *
     *  \iparam p_Parent = Parent widget
     */
    /****************************************************************************/
    explicit CSelectSealingDialog(QWidget *p_Parent = 0);

    /****************************************************************************/
    /*!
     *  \brief Destructor
     */
    /****************************************************************************/
    virtual ~CSelectSealingDialog();

    /****************************************************************************/
    /*!
     *  \brief Get the selected mode
     *  \return mode of selected
     */
    /****************************************************************************/
    int GetSelectedMode() const
    {
        return m_SelectedMode;
    }

private slots:

    /****************************************************************************/
    /*!
     *  \brief This function is called when radio button is clicked
     *  \iparam radioBtnIndex = button index
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
     *  \brief Filter Event function.
     *  \iparam p_Object = pointer of object
     *  \iparam p_Event = event object
     *  \return true or false
     */
    /****************************************************************************/
    bool eventFilter(QObject *p_Object, QEvent *p_Event);

private:
    Ui::CSelectSealingDialog *mp_Ui;     //!< User interface
    //MainMenu::CScrollWheel *mp_ScrollWheel;         //!< Temperature scroll wheel

    QButtonGroup* mp_ButtonGroup;   //!< button group
    int m_SelectedMode;             //!< selected auto test mode or manual test mode


    /****************************************************************************/
    /*!
     *  \brief Disable copy and assignment operator.
     *
     */
    /****************************************************************************/
    Q_DISABLE_COPY(CSelectSealingDialog)

};

} // end namespace DiagnosticsManufacturing

#endif // SELECTSEALINGDIALOG_H
