/****************************************************************************/
/*! \file DlgSelectTestOptions.h
 *
 *  \brief
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2014-05-12
 *   $Author:  $ Abe Yang
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

#ifndef DLGSELECTTESTOPTIONS_H
#define DLGSELECTTESTOPTIONS_H

#include <QWidget>

class QButtonGroup;

namespace MainMenu {

namespace Ui {
    class CDlgSelectTestOptions;
}

/****************************************************************************/
/**
 * \brief This class for select test options dialog
 */
/****************************************************************************/
class CDlgSelectTestOptions : public QWidget
{
    Q_OBJECT

public:
    /****************************************************************************/
    /*!
     *  \brief  Constructor
     *  \iparam p_Parent = the parent widget
     */
    /****************************************************************************/
    explicit CDlgSelectTestOptions(QWidget *p_Parent = 0);

    /****************************************************************************/
    /*!
     *  \brief Deconstructor
     */
    /****************************************************************************/
    ~CDlgSelectTestOptions(void);

    /****************************************************************************/
    /*!
     *  \brief To enable next button
     *  \iparam bEnable = falg for next button
     */
    /****************************************************************************/
    void EnableNextButton(bool bEnable);
protected:
    /****************************************************************************/
    /*!
     *  \brief Event handler for change events
     *
     *  \iparam p_Event = Change event
     */
    /****************************************************************************/
    void changeEvent(QEvent *p_Event);

signals:
    /****************************************************************************/
    /*!
     *  \brief Signal emit for radio button selected
     *  \iparam radioBtnIndex = index of redio button
     */
    /****************************************************************************/
    void RadioBtnSelected(int radioBtnIndex);

private:
    Ui::CDlgSelectTestOptions *ui;  //!< user interface
    QButtonGroup* m_pButtonGroup;   //!< group of button
};

} // namespace MainMenu

#endif // DLGSELECTTESTOPTIONS_H
