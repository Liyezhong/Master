/****************************************************************************/
/*! \file DashboardComboBox.h
 *
 *  \brief DashboardComboBox definition.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 08-Feb-2013
 *   $Author:  $ Santhosha Kumar Raju
 *
 *  \b Company:
 *
 *       Leica Microsystems Ltd. Shanghai.
 *
 *  (C) Copyright 2012 by Leica Microsystems Shanghai. All rights reserved.
 *  This is unpublished proprietary source code of Leica. The copyright notice
 *  does not evidence any actual or intended publication.
 *
 */
/****************************************************************************/

#ifndef DASHBOARD_DASHBOARDCOMBOBOX_H
#define DASHBOARD_DASHBOARDCOMBOBOX_H

#include <QComboBox>
#include <QListView>
#include <QPainter>
#include <QMessageBox>
#include <QFrame>
#include <QMouseEvent>

#include "Dashboard/Include/DashboardComboBoxModel.h"

namespace Dashboard {

/****************************************************************************/
/**
 * \brief This is a touchscreen optimized combobox
 */
/****************************************************************************/
class CDashboardComboBox : public QComboBox
{
    Q_OBJECT

public:
    explicit CDashboardComboBox(QWidget *p_Parent = 0);
    ~CDashboardComboBox();
    void showPopup();
    void hidePopup();
    void UpdateSelectedProgramName(QString & SelProgramName);

protected:
    void paintEvent(QPaintEvent *e);
private:
    QWidget *mp_Popup;
    QListView *mp_ListView;
    QString m_SelProgName;

};

} // end namespace Dashboard

#endif // DASHBOARD_DASHBOARDCOMBOBOX_H
