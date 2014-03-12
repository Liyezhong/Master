/****************************************************************************/
/*! \file TestScreen.cpp
 *
 *  \brief TestScreen implementation.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2011-05-23
 *   $Author:  $ M.Scherer
 *
 *  \b Company:
 *
 *       Leica Biosystems Nussloch GmbH.
 *
 *  (C) Copyright 2010 by Leica Biosystems Nussloch GmbH. All rights reserved.
 *  This is unpublished proprietary source code of Leica. The copyright notice
 *  does not evidence any actual or intended publication.
 *
 */
/****************************************************************************/

#include "TestFramework/Include/TestScreen.h"
#include "ui_TestScreen.h"

namespace TestFramework {

/****************************************************************************/
/*!
 *  \brief Constructor
 *
 *  \iparam p_Parent = Parent widget
 */
/****************************************************************************/
CTestScreen::CTestScreen(QWidget *p_Parent) : QWidget(p_Parent), mp_Ui(new Ui::CTestScreen)
{
    mp_Ui->setupUi(this);

    mp_TestCaseLayout = new QGridLayout();
    mp_TestCaseLayout->setRowStretch(0, 1);
    mp_TestCaseLayout->setVerticalSpacing(18);
    mp_TestCaseLayout->setContentsMargins(0, 0, 0, 0);

    mp_TestCaseWidget = new QWidget();
    mp_TestCaseWidget->setLayout(mp_TestCaseLayout);

    mp_ScrollArea = new QScrollArea();
    mp_ScrollArea->setWidgetResizable(true);
    mp_ScrollArea->setWidget(mp_TestCaseWidget);

    QPalette Palette = mp_ScrollArea->palette();
    Palette.setColor(QPalette::Window, QColor(226, 227, 228));
    mp_ScrollArea->setPalette(Palette);

    mp_Ui->contentScroller->SetContent(mp_ScrollArea);
    mp_Ui->contentScroller->SimpleArrows();
    mp_Ui->resultLed->setPixmap(QPixmap(":/RadioButton/RadioButton-Disabled.png"));

    m_LineCounter = 0;
}

/****************************************************************************/
/*!
 *  \brief Destructor
 */
/****************************************************************************/
CTestScreen::~CTestScreen()
{
    try {
        while (!m_Caption.isEmpty()) {
            delete m_Caption.takeFirst();
        }

        delete mp_TestCaseLayout;
        delete mp_TestCaseWidget;
        delete mp_ScrollArea;
        delete mp_Ui;
    }
    catch (...) {}
}

/****************************************************************************/
/*!
 *  \brief Adds a caption of an assembly group to the main screen
 *
 *  \iparam Caption = Title for a section
 */
/****************************************************************************/
void CTestScreen::AddCaption(QString Caption)
{
    //lint -esym(429, p_Label)
    QLabel *p_Label = new QLabel(Caption);
    QFont Font;
    Font.setBold(true);
    p_Label->setAlignment(Qt::AlignHCenter);
    p_Label->setFont(Font);
    mp_TestCaseLayout->addWidget(p_Label, m_LineCounter, 0, 1, 8);

    mp_TestCaseLayout->setRowStretch(m_LineCounter, 0);
    mp_TestCaseLayout->setRowStretch(m_LineCounter + 1, 1);

    m_Caption << p_Label;

    m_LineCounter++;
}

/****************************************************************************/
/*!
 *  \brief Adds a test case to the main screen
 *
 *  \iparam p_TestCase = Test case
 */
/****************************************************************************/
void CTestScreen::AddTestCase(TestCases::CTestCaseBase *p_TestCase)
{
    //lint -esym(429, p_TestCase)
    qint32 Column = 0;

    for (qint32 i = 0; i < 5; i++) {
        QWidget *p_Temp = p_TestCase->GetLayoutItem(i);
        if (p_Temp != NULL) {
            mp_TestCaseLayout->addWidget(p_Temp, m_LineCounter, Column, 1, p_TestCase->GetLayoutSpan(i));
        }
        Column++;
        if (i != 2 && i != 4) {
            mp_TestCaseLayout->setColumnStretch(Column, 1);
            Column++;
        }
    }

    mp_TestCaseLayout->setRowStretch(m_LineCounter, 0);
    mp_TestCaseLayout->setRowStretch(m_LineCounter + 1, 1);

    m_LineCounter++;
}

} // end namespace TestFramework
