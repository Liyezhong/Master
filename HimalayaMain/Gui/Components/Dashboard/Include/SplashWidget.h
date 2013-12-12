#ifndef SPLASHWIDGET_H
#define SPLASHWIDGET_H

#include <QDialog>

namespace Ui {
class SplashWidget;
}

class SplashWidget : public QDialog
{
    Q_OBJECT
    
public:
    explicit SplashWidget(QWidget *parent = 0);
    ~SplashWidget();

private:
    Ui::SplashWidget *ui;
};

#endif // SPLASHWIDGET_H
