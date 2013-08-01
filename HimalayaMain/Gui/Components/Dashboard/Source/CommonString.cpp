#include "Dashboard/Include/CommonString.h"
#include <QApplication>
#include <Dashboard/Include/DashboardWidget.h>

namespace CommonString
{
    QString strOK;
    QString strYes;
    QString strNo;
    QString strClose;
    QString strInforMsg;
    QString strCancel;
    QString strConfirmMsg;
    void RetranslateUIString()
    {
        strOK = QApplication::translate("CommonString", "OK", 0, QApplication::UnicodeUTF8);
        strYes = QApplication::translate("CommonString", "Yes", 0, QApplication::UnicodeUTF8);
        strNo  =  QApplication::translate("CommonString", "No", 0, QApplication::UnicodeUTF8);
        strClose = QApplication::translate("CommonString", "Close", 0, QApplication::UnicodeUTF8);
        strCancel = QApplication::translate("CommonString", "Cancel", 0, QApplication::UnicodeUTF8);
        strInforMsg = QApplication::translate("CommonString", "Information Message", 0, QApplication::UnicodeUTF8);
        strConfirmMsg = QApplication::translate("CommonString", "Confirmation Message", 0, QApplication::UnicodeUTF8);

    }
}
