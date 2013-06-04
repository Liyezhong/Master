#ifndef SPLASHWIDGET_H
#define SPLASHWIDGET_H

#include <QWidget>
#include "MainMenu/Include/DialogFrame.h"

namespace Ui {
class SplashWidget;
}

class SplashWidget : public MainMenu::CDialogFrame
{
    Q_OBJECT
    
public:
    explicit SplashWidget(QWidget *parent = 0);
    ~SplashWidget();
    
private:
    Ui::SplashWidget *ui;
};

#endif // SPLASHWIDGET_H
