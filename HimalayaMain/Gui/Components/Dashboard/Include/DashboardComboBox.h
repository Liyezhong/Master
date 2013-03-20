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
#include <QMessageBox>
#include <QFrame>
#include <QMouseEvent>


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

private:
    QWidget *mp_Popup;
    QMessageBox *mp_Msg;

public slots:
    void handleSelectionChanged(int);


};

} // end namespace Dashboard

#endif // MAINMENU_COMBOBOX_H
