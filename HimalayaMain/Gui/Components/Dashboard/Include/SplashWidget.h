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
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function SplashWidget
     *
     *  \param parent
     *
     *  \return from SplashWidget
     */
    /****************************************************************************/
    explicit SplashWidget(QWidget *parent = 0);
    ~SplashWidget();

public slots:
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of slot OnProgramSelected
     */
    /****************************************************************************/
    void ShowModel();

private:
    Ui::SplashWidget *ui;       ///<  Definition/Declaration of variable ui
};

#endif // SPLASHWIDGET_H

