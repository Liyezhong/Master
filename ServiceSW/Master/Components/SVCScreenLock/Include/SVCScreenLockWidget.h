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
 *  \brief  Definition/Declaration of class ScreenSaverWidget
 */
/****************************************************************************/
class CSVCScreenLockWidget : public QWidget
{
    Q_OBJECT

public:
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function ScreenSaverWidget
     *
     *  \param parent
     *
     *  \return from ScreenSaverWidget
     */
    /****************************************************************************/
    explicit CSVCScreenLockWidget(QWidget *p_Parent = 0);
    ~CSVCScreenLockWidget();

private:

    void ConnectKeyBoardSignalSlots();
    void DisconnectKeyBoardSignalSlots();

    Ui::CSVCScreenLockWidget *ui;       ///<  Definition/Declaration of variable ui
    MainMenu::CMessageDlg *mp_MessageDlg;               //!< Information dialog
    QString m_EnteredPin;               //!< Service Pin
    QTimer* m_timer;       ///<  Definition/Declaration of variable m_timer
    int m_CurrentTab;
    KeyBoard::CKeyBoard *mp_KeyBoardWidget;             //!< KeyBoard widget
private slots:
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of AppIdleForLongTime
     */
    /****************************************************************************/
    void AppIdleForLongTime();

    void OnOkClicked(QString EnteredString);
    void OnESCClicked();

    void DisplayKeyBoard();
    void HideKeyBoard();
public slots:
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of OnInteractStart
     */
    /****************************************************************************/
    void OnInteractStart();

    void OnCurrentTabChanged(int Index);
};

} //end of namespce SVCScreenLock

#endif // SCREENSAVERWIDGET_H

