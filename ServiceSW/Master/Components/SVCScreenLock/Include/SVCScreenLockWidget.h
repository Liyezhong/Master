/****************************************************************************/
/*! \file SVCScreenLockWidget.cpp
 *
 *  \brief Declaration of target temperature screen lock widget.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2014-11-12
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
#ifndef SVCSCREENLOCKWIDGET_H
#define SVCSCREENLOCKWIDGET_H

#include <KeyBoard/Include/KeyBoard.h>
#include "MainMenu/Include/PanelFrame.h"
#include "MainMenu/Include/MessageDlg.h"

namespace SVCScreenLock {

namespace Ui {
    class CSVCScreenLockWidget;
}

/****************************************************************************/
/*!
 *  \brief  Definition/Declaration of class CSVCScreenLockWidget
 */
/****************************************************************************/
class CSVCScreenLockWidget : public QWidget
{
    Q_OBJECT

public:
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function CSVCScreenLockWidget
     *
     *  \param p_Parent = parent widget
     *
     *  \return from CSVCScreenLockWidget
     */
    /****************************************************************************/
    explicit CSVCScreenLockWidget(QWidget *p_Parent = 0);

    /****************************************************************************/
    /*!
     *  \brief Deconstructor
     */
    /****************************************************************************/
    ~CSVCScreenLockWidget();

    /****************************************************************************/
    /*!
     *  \brief start timer
     */
    /****************************************************************************/
    void StartTimer();
    /****************************************************************************/
    /*!
     *  \brief set lock status
     *  \param Status = status of lock
     */
    /****************************************************************************/
    void SetLockStatus(bool Status) {
        m_LockStatus = Status;
    }

private:
    /****************************************************************************/
    /*!
     *  \brief Connects signals and slots of keyboard.
     */
    /****************************************************************************/
    void ConnectKeyBoardSignalSlots();

    /****************************************************************************/
    /*!
     *  \brief Disconnects signals and slots of keyboard.
     */
    /****************************************************************************/
    void DisconnectKeyBoardSignalSlots();

    Ui::CSVCScreenLockWidget *ui;       ///<  Definition/Declaration of variable ui
    MainMenu::CMessageDlg *mp_MessageDlg;               //!< Information dialog
    QString m_EnteredPin;               //!< Service Pin
    QTimer* m_timer;       ///<  Definition/Declaration of variable m_timer
    KeyBoard::CKeyBoard *mp_KeyBoardWidget;             //!< KeyBoard widget
    bool m_LockStatus;      //!< flag of lock status
private slots:
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of AppIdleForLongTime
     */
    /****************************************************************************/
    void AppIdleForLongTime();

    /****************************************************************************/
    /*!
     *  \brief This function is called when OK is clicked
     *  \iparam EnteredString = Stores line edit string
     */
    /****************************************************************************/
    void OnOkClicked(QString EnteredString);

    /****************************************************************************/
    /*!
     *  \brief This function is called when Esc is clicked
     */
    /****************************************************************************/
    void OnESCClicked();

    /****************************************************************************/
    /*!
     *  \brief Slot for display keyboard widget
     */
    /****************************************************************************/
    void DisplayKeyBoard();

    /****************************************************************************/
    /*!
     *  \brief Slot for hide keyboard widget
     */
    /****************************************************************************/
    void HideKeyBoard();
public slots:
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of OnInteractStart
     */
    /****************************************************************************/
    void OnInteractStart();
};

} //end of namespce SVCScreenLock

#endif // SCREENSAVERWIDGET_H

