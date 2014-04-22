#ifndef SCREENSAVERWIDGET_H
#define SCREENSAVERWIDGET_H

#include "MainMenu/Include/PanelFrame.h"

namespace Ui {
class ScreenSaverWidget;
}

/****************************************************************************/
/*!
 *  \brief  Definition/Declaration of class ScreenSaverWidget
 */
/****************************************************************************/
class ScreenSaverWidget : public MainMenu::CPanelFrame
{
    Q_OBJECT
    
public:
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function ScreenSaverWidget
     *
     *  \param parent
     *
     *  \return from ScreenSaverWidget
     */
    /****************************************************************************/
    explicit ScreenSaverWidget(QWidget *parent = 0);
    ~ScreenSaverWidget();
    
private:
    Ui::ScreenSaverWidget *ui;       ///<  Definition/Declaration of variable ui
    QTimer* m_timer;       ///<  Definition/Declaration of variable m_timer
    bool m_isFirstShow;       ///<  Definition/Declaration of variable m_isFirstShow
private slots:
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of AppIdleForLongTime
     */
    /****************************************************************************/
    void AppIdleForLongTime();

public slots:
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of OnInteractStart
     */
    /****************************************************************************/
    void OnInteractStart();
};

#endif // SCREENSAVERWIDGET_H

