/****************************************************************************/
/*! \file HiMenuGroup.cpp
 *
 *  \brief Implementation of himalaya menu group.
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

#include "SystemTracking/Include/HiMenuGroup.h"

#include <QDebug>

#include "ui_HiMenuGroup.h"

namespace MainMenu {

/****************************************************************************/
/*!
 *  \brief Constructor
 *  \iparam Parent = Parent widget
 */
/****************************************************************************/
CHiMenuGroup::CHiMenuGroup(QWidget *Parent) :
    QWidget(Parent),
    ui(new Ui::CHiMenuGroup),
    m_ItemCount(0)
{
    ui->setupUi(this);

    // create horizontal layout
    mp_HorizontalLayout = new QHBoxLayout(this);
    mp_HorizontalLayout->setSpacing(0);
    mp_HorizontalLayout->setContentsMargins(0, 0, 0, 0);
    mp_HorizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));

    // create button panel
    mp_ButtonPanel = new CPanelFrame(this);
    mp_ButtonPanel->setObjectName(QString::fromUtf8("menuWidget"));

    mp_ButtonPanel->SetPanelTitle(tr("Menu"));

    mp_HorizontalLayout->addWidget(mp_ButtonPanel);

    // create button layout
    mp_ButtonLayout = new QVBoxLayout;
    mp_ButtonLayout->addStretch(1);
    mp_ButtonPanel->SetContent(mp_ButtonLayout);

    // create content container
    mp_ContentPanel = new QWidget(this);
    mp_ContentPanel->setObjectName(QString::fromUtf8("contentWidget"));

    mp_HorizontalLayout->addWidget(mp_ContentPanel);
    mp_HorizontalLayout->setStretch(1, 1);

    // create content layout
    mp_ContentLayout = new QVBoxLayout;
    mp_ContentLayout->setContentsMargins(0, 0, 0, 0);
    mp_ContentPanel->setLayout(mp_ContentLayout);

    // create stacked widget
    mp_ContentStack = new QStackedWidget;
    mp_ContentLayout->addWidget(mp_ContentStack);

    // create button group
    mp_ButtonGroup = new QButtonGroup;

    if (!connect(mp_ButtonGroup, SIGNAL(buttonClicked(int)), this, SLOT(ButtonPressed(int)))) {
        qDebug() << "CHiMenuGroup: cannot connect 'buttonClicked' signal";
    }
}

CHiMenuGroup::~CHiMenuGroup()
{
    try {
        delete mp_ButtonGroup;

        delete mp_ContentStack;
        delete mp_ContentLayout;
        delete mp_ContentPanel;

        delete mp_ButtonLayout;
        delete mp_ButtonPanel;

        delete mp_HorizontalLayout;

        delete ui;
    }
    catch (...) {}
}

void CHiMenuGroup::SetTitle(const QString &Title)
{
    mp_ButtonPanel->SetPanelTitle(tr("%1").arg(Title));
}

/****************************************************************************/
/*!
 *  \brief Adds a panel to a menu group
 *
 *  \iparam Title = Title of the panel
 *  \iparam p_Content = The content widget
 *  \iparam FramePanel = whether the content widget be hosted in a frame panel
 */
/****************************************************************************/
void CHiMenuGroup::AddPanel(const QString &Title,
                            QWidget *p_Content,
                            bool FramePanel)
{
    AddButton(Title);

    QWidget *p_Widget = 0;
    // create a frame panel to contain the content
    if (FramePanel)
    {
        CPanelFrame *p_Panel = new CPanelFrame;

        QString Text = Title;
        Text.replace(QString("&&"), QString("&"));
        p_Panel->SetPanelTitle(Text);

        QVBoxLayout *p_Layout = new QVBoxLayout;

        p_Panel->SetContent(p_Layout);

        p_Layout->addWidget(p_Content);

        p_Widget = p_Panel;
    }
    else
    {
        p_Widget = p_Content;
    }

    m_ItemCount = mp_ContentStack->addWidget(p_Widget);

    ++m_ItemCount;

    ButtonPressed(0);
}

/****************************************************************************/
/*!
 *  \brief Returns the currently selected panel
 *
 *  \return Current panel widget
 */
/****************************************************************************/
QWidget *CHiMenuGroup::GetCurrentPanel()
{
    return (mp_ContentStack->currentWidget());
}

/****************************************************************************/
/*!
 *  \brief Adds a button to a menu group
 *
 *  \iparam Text = Text of the Bbutton
 */
/****************************************************************************/
void CHiMenuGroup::AddButton(const QString &Text)
{
    //lint -esym(429, p_Button)
    QPushButton *p_Button = new QPushButton;

    p_Button->setFocusPolicy(Qt::NoFocus);
    p_Button->setCheckable(true);

    p_Button->setText(tr("%1").arg(Text));

    mp_ButtonGroup->addButton(p_Button, m_ItemCount);

    mp_ButtonLayout->setStretch(m_ItemCount, 0);
    mp_ButtonLayout->insertWidget(m_ItemCount, p_Button);
    mp_ButtonLayout->addStretch(1);
}

/****************************************************************************/
/*!
 *  \brief Event handler for change events
 *
 *  \iparam p_Event = Change event
 */
/****************************************************************************/

void CHiMenuGroup::changeEvent(QEvent *p_Event)
{
    QWidget::changeEvent(p_Event);
    switch (p_Event->type()) {
        case QEvent::LanguageChange:
            ui->retranslateUi(this);
            break;
        default:
            break;
    }
}

/****************************************************************************/
/*!
 *  \brief Slot called when a button is pressed
 *
 *  \iparam Number = Button number
 */
/****************************************************************************/
void CHiMenuGroup::ButtonPressed(int Number)
{
    mp_ButtonGroup->button(Number)->setChecked(true);
    mp_ContentStack->setCurrentIndex(Number);
}

}
