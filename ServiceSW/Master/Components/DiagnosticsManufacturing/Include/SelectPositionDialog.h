/****************************************************************************/
/*! \file CSelectPositionDialog.h
 *
 *  \brief Header file for class CSelectPositionDialog.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2014-06-09
 *   $Author:  $ Sunny Qu
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

#ifndef SELECTPOSITIONDIALOG_H
#define SELECTPOSITIONDIALOG_H

#include "MainMenu/Include/DialogFrame.h"
#include "Core/Include/ServiceDefines.h"
#include <QTimer>
#include <KeyBoard/Include/KeyBoard.h>
#include "MainMenu/Include/ScrollWheel.h"
#include "Core/Include/ServiceDefines.h"

namespace DiagnosticsManufacturing {

namespace Ui {
    class CSelectPositionDialog;
}

/****************************************************************************/
/**
 * \brief This dialog is started, when the user has to wait for the result of
 *        a background process.
 */
/****************************************************************************/
class CSelectPositionDialog : public MainMenu::CDialogFrame
{
    Q_OBJECT
    friend class  CTestMainMenu;

public:
    /****************************************************************************/
    /*!
     *  \brief Constructor
     *
     *  \iparam p_Parent = Parent widget
     */
    /****************************************************************************/
    explicit CSelectPositionDialog(Service::ModuleTestCaseID Id, quint8 Positon, bool TubeFlag, QWidget *p_Parent = 0);

    /****************************************************************************/
    /*!
     *  \brief Destructor
     */
    /****************************************************************************/
    virtual ~CSelectPositionDialog();

    quint8 GetPosition()
    {
        return m_Position;
    }

    bool GetTubeFlag()
    {
        return m_TubeFlag;
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
     *  \brief Abort Dialog
     */
    /****************************************************************************/
    void AbortDialog();

    /****************************************************************************/
    /*!
     *  \brief Connects signals and slots of keyboard.
     */
    /****************************************************************************/
    void RetranslateUI();

protected:
    bool eventFilter(QObject *p_Object, QEvent *p_Event);

private:
    Ui::CSelectPositionDialog *mp_Ui;     //!< User interface
    MainMenu::CScrollWheel *mp_ScrollWheel;         //!< Temperature scroll wheel

    QButtonGroup* m_pButtonGroup;
    quint8 m_Position;                    //!< position selected by user.
    bool m_TubeFlag;                      //!< true -- tube position, falsle -- sealing position


    /****************************************************************************/
    /*!
     *  \brief Disable copy and assignment operator.
     *
     */
    /****************************************************************************/
    Q_DISABLE_COPY(CSelectPositionDialog)

};

} // end namespace DiagnosticsManufacturing

#endif // SELECTPOSITIONDIALOG_H
