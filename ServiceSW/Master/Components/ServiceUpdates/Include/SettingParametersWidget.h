/****************************************************************************/
/*! \file SettingParametersWidget.h
 *
 *  \brief SettingParametersWidget definition.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2011-09-29
 *   $Author:  $ M.Scherer,C.Adaragunchi
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

#ifndef SERVICEUPDATES_SETTINGPARAMETERSWIDGET_H
#define SERVICEUPDATES_SETTINGPARAMETERSWIDGET_H

#include "MainMenu/Include/PanelFrame.h"


namespace ServiceUpdates {

namespace Ui {
    class CSettingParametersWidget;
}

/****************************************************************************/
/**
 * \brief Main selection panel for the ServiceUpdates screen
 */
/****************************************************************************/
class CSettingParametersWidget : public MainMenu::CPanelFrame
{
    Q_OBJECT

public:
    explicit CSettingParametersWidget(QWidget *p_Parent = NULL);

    virtual ~CSettingParametersWidget(void);

    void SetCurrentRow(int CurrentRow);

Q_SIGNALS:
    void CurrentRowChanged(int CurrentRow);

protected:
    void changeEvent(QEvent *p_Event);

private:
    Ui::CSettingParametersWidget *mp_Ui;
    void RetranslateUI();
};

} // end namespace ServiceUpdates

#endif // SERVICEUPDATES_SETTINGPARAMETERSWIDGET_H
