#ifndef CDASHBOARDWIDGET2_H
#define CDASHBOARDWIDGET2_H

#include <QWidget>

namespace Core
{
 class CDataConnector;
}

namespace MainMenu
{
 class CMainWindow;
}

namespace Dashboard {

namespace Ui {
    class CDashboardWidget2;
}

class CDashboardWidget2 : public QWidget
{
    Q_OBJECT
    
public:
    explicit CDashboardWidget2(Core::CDataConnector *p_DataConnector,
                               MainMenu::CMainWindow *p_Parent = NULL);
    ~CDashboardWidget2();
    
private:
    Ui::CDashboardWidget2 *ui;
    Core::CDataConnector *mp_DataConnector;          //!< Data object
    MainMenu::CMainWindow *mp_MainWindow;           //!< Reference to main window.

public slots:
    void OnUnselectProgram();

};

} // end namespace Dashboard

#endif // CDASHBOARDWIDGET2_H
