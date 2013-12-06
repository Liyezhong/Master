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

namespace DataManager
{
    class CDataProgramList;
    class CProgram;
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
    bool IsParaffinInProgram(const DataManager::CProgram* p_Program);
    int GetASAPTime(int, int, int, bool&);
    Ui::CDashboardWidget2 *ui;
    Core::CDataConnector *mp_DataConnector;          //!< Data object
    MainMenu::CMainWindow *mp_MainWindow;           //!< Reference to main window.
    DataManager::CDataProgramList *mp_ProgramList;

public slots:
    void OnUnselectProgram();


private slots:
    void PrepareSelectedProgramChecking();

 signals:
    void AddItemsToFavoritePanel(bool bOnlyAddCleaningProgram = false);

};

} // end namespace Dashboard

#endif // CDASHBOARDWIDGET2_H
