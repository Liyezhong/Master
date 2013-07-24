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
    void WorkAsButton(bool);
protected:
    void paintEvent(QPaintEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void changeEvent(QEvent *p_Event);
signals:
    void ButtonPress();
private:
    QWidget *mp_Popup;
    QListView *mp_ListView;
    QString m_SelProgName;
    bool m_IsWorkAsButton;
    QPixmap m_PixmapEnable, m_PixmapRunning;
    QPixmap* m_Pixmap;
    void RetranslateUI();
};

} // end namespace Dashboard

#endif // DASHBOARD_DASHBOARDCOMBOBOX_H
