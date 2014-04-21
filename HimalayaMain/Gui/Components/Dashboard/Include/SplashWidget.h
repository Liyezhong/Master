#ifndef SPLASHWIDGET_H
#define SPLASHWIDGET_H

#include <QDialog>

namespace Ui {
class SplashWidget;
}

/****************************************************************************/
/*!
 *  \brief  Definition/Declaration of class SplashWidget
 */
/****************************************************************************/
class SplashWidget : public QDialog
{
    Q_OBJECT
    
public:
    explicit SplashWidget(QWidget *parent = 0);
    ~SplashWidget();

public slots:
    void ShowModel();

private:
    Ui::SplashWidget *ui;
};

#endif // SPLASHWIDGET_H
