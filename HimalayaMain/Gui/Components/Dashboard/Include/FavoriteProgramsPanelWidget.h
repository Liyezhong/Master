#ifndef FAVORITEPROGRAMSPANELWIDGET_H
#define FAVORITEPROGRAMSPANELWIDGET_H

#include <QWidget>
#include <QButtonGroup>
#include <QMap>

class QLabel;

namespace MainMenu
{
    class CMainWindow ;
}

namespace Core
{
    class CDataConnector;
}

namespace DataManager
{
    class CDataConnector;
    class CDataProgramList;
}

namespace Dashboard {
    namespace Ui {
    class CFavoriteProgramsPanelWidget;
    }

    class CFavoriteProgramsPanelWidget : public QWidget
    {
        Q_OBJECT

    public:
        explicit CFavoriteProgramsPanelWidget(QWidget *parent = 0);
        ~CFavoriteProgramsPanelWidget();
        void SetPtrToMainWindow(MainMenu::CMainWindow *p_MainWindow, Core::CDataConnector *p_DataConnector);

public slots:
        void AddItemsToFavoritePanel(bool bOnlyAddCleaningProgram);
    private slots:
        void on_pushButton_6_clicked();

    private:
        void SetButtonGroup();
        QMap<int, QString> m_mapButtonProgramId;//(button Id <--> Program ID)
        QMap<int, QLabel*> m_mapLabel;//(label Id <--> QLabel)
        Ui::CFavoriteProgramsPanelWidget *ui;
        QButtonGroup m_ButtonGroup;
        Core::CDataConnector *mp_DataConnector;                     //!< Global data container
        DataManager::CDataProgramList* mp_ProgramList;

    };
}
#endif // FAVORITEPROGRAMSPANELWIDGET_H
