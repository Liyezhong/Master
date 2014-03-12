/****************************************************************************/
/*! \file HiMenuGroup.h
 *
 *  \brief Declaration of himalaya menu group.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2013-05-24
 *   $Author:  $ R.Wu
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

#ifndef MAINMENU_HIMENUGROUP_H
#define MAINMENU_HIMENUGROUP_H

#include <QWidget>
#include <QPushButton>
#include <QSignalMapper>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QStackedWidget>

#include "MainMenu/Include/PanelFrame.h"

namespace MainMenu {

namespace Ui {
class CHiMenuGroup;
}

/****************************************************************************/
/**
 * \brief This class contains menu buttons and all panels that are accessible
 *        through the menu.
 */
/****************************************************************************/
class CHiMenuGroup : public QWidget
{
    Q_OBJECT

public:
    explicit CHiMenuGroup(QWidget *Parent = 0);
    ~CHiMenuGroup();

    void SetTitle(const QString &Title);

    void AddPanel (const QString &Title,
                   QWidget *p_Content,
                   bool FramePanel = true);

    QWidget *GetCurrentPanel(void);

private:
    void AddButton (const QString &Text);

    Ui::CHiMenuGroup *ui;               //!< User interface

    QHBoxLayout *mp_HorizontalLayout;   //!< Layout splitting button panel and content panel

    CPanelFrame *mp_ButtonPanel;        //!< Panel containing butons
    QVBoxLayout *mp_ButtonLayout;       //!< Layout of the menu buttons
    QButtonGroup *mp_ButtonGroup;       //!< Group containing the menu buttons

    QWidget *mp_ContentPanel;           //!< Panel containing contents
    QVBoxLayout *mp_ContentLayout;      //!< Layout content panel
    QStackedWidget *mp_ContentStack;    //!< Stack holding the content screens

    int m_ItemCount;                    //!< Number of items grouped by this screen

protected:
    void changeEvent(QEvent *p_Event);

private slots:
    void ButtonPressed(int Number);
};

} // end namespace MainMenu

#endif // MAINMENU_HIMENUGROUP_H
