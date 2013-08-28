#ifndef SCREENSAVERWIDGET_H
#define SCREENSAVERWIDGET_H

#include "MainMenu/Include/PanelFrame.h"

namespace Ui {
class ScreenSaverWidget;
}

class ScreenSaverWidget : public MainMenu::CPanelFrame
{
    Q_OBJECT
    
public:
    explicit ScreenSaverWidget(QWidget *parent = 0);
    ~ScreenSaverWidget();
    
private:
    Ui::ScreenSaverWidget *ui;
    QTimer* m_timer;
    bool m_isFirstShow;
private slots:
    void AppIdleForLongTime();

public slots:
    void OnInteractStart();
};

#endif // SCREENSAVERWIDGET_H
