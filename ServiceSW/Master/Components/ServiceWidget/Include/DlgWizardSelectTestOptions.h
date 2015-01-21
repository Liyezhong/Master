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

#ifndef DLGWIZARDSELECTTESTOPTIONS_H
#define DLGWIZARDSELECTTESTOPTIONS_H

#include "ServiceWidget/Include/DlgWizardBase.h"

namespace MainMenu {

class CDlgSelectTestOptions;

/****************************************************************************/
/**
 * \brief This class for wizard selected test option dialog
 */
/****************************************************************************/
class CDlgWizardSelectTestOptions : public CDlgWizardBase
{
    Q_OBJECT

public:
    /****************************************************************************/
    /*!
     *  \brief  Constructor
     *  \iparam p_Parent = the parent widget
     *  \iparam p_MainWindow = main window widget
     */
    /****************************************************************************/
    explicit CDlgWizardSelectTestOptions(QWidget *p_Parent = 0, QWidget *p_MainWindow = 0);

    /****************************************************************************/
    /*!
     *  \brief Deconstructor
     */
    /****************************************************************************/
    ~CDlgWizardSelectTestOptions(void);
signals:

private slots:

    /****************************************************************************/
    /*!
     *  \brief Slot for radio button selected.
     *  \iparam radioBtnIndex = index of radio button
     */
    /****************************************************************************/
    void OnRadioBtnSelected(int radioBtnIndex);

    /****************************************************************************/
    /*!
     *  \brief Slot for clicked next button
     */
    /****************************************************************************/
    void OnClickedNextButton();
signals:

    /****************************************************************************/
    /*!
     *  \brief Signal emit for clicked next button
     *  \iparam radioBtnIndex
     */
    /****************************************************************************/
    void ClickedNextButton(int radioBtnIndex);

private:
    CDlgSelectTestOptions* m_pDlgSelectTestOptions; //!< Select test options dialog
    int m_radioBtnIndex;    //!< store radio button index

};

} // namespace MainMenu

#endif // DLGWIZARDSELECTTESTOPTIONS_H
