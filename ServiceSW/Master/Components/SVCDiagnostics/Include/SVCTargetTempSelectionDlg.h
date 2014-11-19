/****************************************************************************/
/*! \file SVCTargetTempSelectionDlg.cpp
 *
 *  \brief Declaration of target temperature selection dialog.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2014-11-10
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
#ifndef SVCTARGETTEMPSELECTIONDLG_H
#define SVCTARGETTEMPSELECTIONDLG_H

#include "MainMenu/Include/DialogFrame.h"
#include "MainMenu/Include/ScrollWheel.h"

namespace SVCDiagnostics {

namespace Ui {
class CSVCTargetTempSelectionDlg;
}

/****************************************************************************/
/*!
 *  \brief  Definition/Declaration of class CSVCTargetTempSelectionDlg
 */
/****************************************************************************/
class CSVCTargetTempSelectionDlg : public MainMenu::CDialogFrame
{
    Q_OBJECT
public:
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function CSVCTargetTempSelectionDlg
     *  \iparam DefaultTemp = The default temperature
     *  \iparam MinTemp  = The min temperature
     *  \iparam MaxTemp  = The max temperature
     *  \iparam p_Parent = parent widget
     */
    /****************************************************************************/
    explicit CSVCTargetTempSelectionDlg(int DefaultTemp, int MinTemp, int MaxTemp, QWidget* p_Parent);

    /****************************************************************************/
    /*!
     *  \brief Deconstructor
     */
    /****************************************************************************/
    ~CSVCTargetTempSelectionDlg();

    //void SetText(const QString& Text);

    /****************************************************************************/
    /*!
     *  \brief  To get target temperature
     *  \return target temperature
     */
    /****************************************************************************/
    int GetTargetTemp() {
        return m_TargetTemp;
    }

protected:
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function changeEvent
     *  \param p_Object = The event object
     *  \param p_Event = QEvent type parameter
     *  \return ture or false
     */
    /****************************************************************************/
    bool eventFilter(QObject *p_Object, QEvent *p_Event);

private Q_SLOTS:

    /****************************************************************************/
    /*!
     *  \brief  Slot for Ok button clicked
     */
    /****************************************************************************/
    void OnOk();

private:
    Ui::CSVCTargetTempSelectionDlg* mp_Ui;    //!< User interface
    MainMenu::CScrollWheel *mp_ScrollWheel;   //!< Temperature scroll wheel
    int m_TargetTemp;                         //!< Target temperature selected by user.
};

}//end of namespace SVCDiagnostics

#endif // SVCTARGETTEMPSELECTIONDLG_H
