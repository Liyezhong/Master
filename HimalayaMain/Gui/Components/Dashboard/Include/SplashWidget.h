#ifndef SPLASHWIDGET_H
#define SPLASHWIDGET_H

#include <QWidget>

namespace Ui {
class SplashWidget;
}

class SplashWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit SplashWidget(QWidget *parent = 0);
    ~SplashWidget();
    
private:
    Ui::SplashWidget *ui;
};

#endif // SPLASHWIDGET_H
