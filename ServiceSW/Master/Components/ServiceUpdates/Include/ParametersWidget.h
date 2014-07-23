/****************************************************************************/
/*! \file ParametersWidget.h
 *
 *  \brief ParametersWidget definition.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2011-09-29
 *   $Author:  $ M.Scherer,C.Adaragunchi, Soumya. D
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

#ifndef SETTINGS_PARAMETERSWIDGET_H
#define SETTINGS_PARAMETERSWIDGET_H

#include "MainMenu/Include/PanelFrame.h"


namespace ServiceUpdates {

namespace Ui {
    class CParametersWidget;
}

/****************************************************************************/
/**
 * \brief Main selection panel for the settings screen
 */
/****************************************************************************/
class CParametersWidget : public MainMenu::CPanelFrame
{
    Q_OBJECT  

public:
    /****************************************************************************/
    /**
     * \brief  Default constructor.
     * \iparam parent = parent of widget
     */
    /****************************************************************************/
    explicit CParametersWidget(QWidget *p_Parent = NULL);

    /****************************************************************************/
    /**
     * \brief Destructor.
     */
    /****************************************************************************/
    virtual ~CParametersWidget();

protected:
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function changeEvent
     *  \param p_Event = QEvent type parameter
     *  \return from changeEvent
     */
    /****************************************************************************/
    void changeEvent(QEvent *p_Event);

private:
    Ui::CParametersWidget *mp_Ui;   //!< User interface

    /****************************************************************************/
    /*!
     *  \brief To retranslate ui.
     */
    /****************************************************************************/
    void RetranslateUI();

signals:
    /****************************************************************************/
    /*!
     *  \brief Changes the selected settings widget
     *
     *  \iparam CurrentRow = Number of the widget
     */
    /****************************************************************************/
    void CurrentRowChanged(int CurrentRow);

};

} // end namespace ServiceUpdates

#endif // SETTINGS_PARAMETERSWIDGET_H
