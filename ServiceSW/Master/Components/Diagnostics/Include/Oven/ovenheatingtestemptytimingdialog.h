#ifndef OVENHEATINGTESTEMPTYTIMINGDIALOG_H
#define OVENHEATINGTESTEMPTYTIMINGDIALOG_H

#include "MainMenu/Include/DialogFrame.h"
#include "Core/Include/ServiceDefines.h"
#include <QDialog>

namespace Ui {
class OvenHeatingTestEmptyTimingDialog;
}

class OvenHeatingTestEmptyTimingDialog : public MainMenu::CDialogFrame
{
    Q_OBJECT
    
public:
    /****************************************************************************/
    /*!
     *  \brief Constructor
     *  \iparam parent = Parent widget
     */
    /****************************************************************************/
    explicit OvenHeatingTestEmptyTimingDialog(QWidget *parent = 0);

    /****************************************************************************/
    /*!
     *  \brief Destructor
     */
    /****************************************************************************/
    ~OvenHeatingTestEmptyTimingDialog();

    /****************************************************************************/
    /*!
     *  \brief To set this dialog title.
     *  \iparam Title = the string of title.
     */
    /****************************************************************************/
    void SetTitle(QString Title);

    /****************************************************************************/
    /*!
     *  \brief Update label status
     *
     *  \iparam Status = Label test status
     */
    /****************************************************************************/
    void UpdateLabel(const Service::ModuleTestStatus &Status);
    
private:
    Ui::OvenHeatingTestEmptyTimingDialog *ui;
};

#endif // OVENHEATINGTESTEMPTYTIMINGDIALOG_H
