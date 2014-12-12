/****************************************************************************/
/*! \file SelectBottleNReagentDialog.h
 *
 *  \brief Declaration of select desired bottle number and reagent group.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2014-10-08
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

#ifndef DIAGNOSTICS_SELECTDESIREDBOTTLEDIALOG_H
#define DIAGNOSTICS_SELECTDESIREDBOTTLEDIALOG_H

#include <QPushButton>
#include "MainMenu/Include/DialogFrame.h"
#include "MainMenu/Include/ScrollWheel.h"

namespace Diagnostics {

namespace Ui {
class CSelectBottleNReagentDialog;
}

/****************************************************************************/
/**
 * \brief This dialog is to select desired bottle number and reagent group.
 */
/****************************************************************************/
class CSelectBottleNReagentDialog : public MainMenu::CDialogFrame
{
    Q_OBJECT

public:
    /****************************************************************************/
    /*!
     *  \brief Constructor
     *  \iparam parent = Parent widget
     */
    /****************************************************************************/
    explicit CSelectBottleNReagentDialog(int MaxBottleNum, QWidget *parent = 0);

    /****************************************************************************/
    /*!
     *  \brief Destructor
     */
    /****************************************************************************/
    ~CSelectBottleNReagentDialog();

    /****************************************************************************/
    /*!
     *  \brief To get selected Bottle number
     *  \return the bottle number
     */
    /****************************************************************************/
    int GetBottleNumber() {
        return m_BottleNumber;
    }
    /****************************************************************************/
    /*!
     *  \brief To get selected radio button option
     *  \return Index of the radio button.
     */
    /****************************************************************************/
    int GetOption() {
        return m_Option;
    }

    /****************************************************************************/
    /*!
     *  \brief To Set radio button visible
     *  \iparam Visible = flag visible.
     */
    /****************************************************************************/
    void SetRadioButtonVisible(bool Visible);

    /****************************************************************************/
    /*!
     *  \brief To Set scrool wheel visible
     *  \iparam Visible = flag visible.
     */
    /****************************************************************************/
    void SetScrollWheelVisible(bool Visible);

    /****************************************************************************/
    /*!
     *  \brief To Set this dialog title
     *  \iparam Title = title string.
     */
    /****************************************************************************/
    void SetTitle(QString& Title);

    /****************************************************************************/
    /*!
     *  \brief To Set this label text
     *  \iparam Text = text string.
     */
    /****************************************************************************/
    void SetLableText(QString& Text);

    /****************************************************************************/
    /*!
     *  \brief To Set this radio button text
     *  \iparam Button1 = button 1 text string.
     *  \iparam Button2 = button 2 text string
     */
    /****************************************************************************/
    void SetRadioButtonText(QString& Button1, QString& Button2);

    /****************************************************************************/
    /*!
     *  \brief Hide dialog when clicked abort
     */
    /****************************************************************************/
    void HideAbort();
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
     *  \brief This function is called when radio button is clicked
     */
    /****************************************************************************/
    void OnRadioBtnSelected(int radioBtnIndex);

    /****************************************************************************/
    /*!
     *  \brief The slot for ok button
     */
    /****************************************************************************/
    void OnOk();

    /****************************************************************************/
    /*!
     *  \brief The slot for abort button
     */
    /****************************************************************************/
    void OnAbort();

private:
    Ui::CSelectBottleNReagentDialog *mp_Ui;     //!< User interface
    MainMenu::CScrollWheel *mp_ScrollWheel;   //!< Temperature scroll wheel

    QButtonGroup* mp_ButtonGroup;             //!< redio button group
    int m_BottleNumber;                       //!< Desired bottle number selected by user.
    int m_Option;                             //!< Radio button option (0--button1, 1--button2).

};


} // namespace Diagnostics
#endif // DIAGNOSTICS_SELECTDESIREDBOTTLEDIALOG_H

