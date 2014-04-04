#ifndef FAVORITEPROGRAMSPANELWIDGET_H
#define FAVORITEPROGRAMSPANELWIDGET_H

#include <QWidget>
#include <QButtonGroup>
#include <QMap>
#include <QDateTime>

#include "HimalayaDataContainer/Containers/Programs/Include/Program.h"

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

namespace Dashboard
{
    class CDashboardDateTimeWidget;
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
        static QString SELECTED_PROGRAM_NAME;

    public slots:
        void AddItemsToFavoritePanel(bool bOnlyAddCleaningProgram);
    private slots:
        void OnSelectDateTime(const QDateTime&);
        void OnEndTimeButtonClicked();
        void OnProcessStateChanged();
        void ProgramSelected(QString& programId, int asapEndTime, bool bProgramStartReady);
        void UndoProgramSelection();
        void UpdateProgram(DataManager::CProgram &Program);
    signals:
        void PrepareSelectedProgramChecking(const QString& selectedProgramId);
        void OnSelectEndDateTime(const QDateTime &);

    private:
        void SetButtonGroup();
        QMap<int, QLabel*> m_mapLabel;//(label Id <--> QLabel)
        Ui::CFavoriteProgramsPanelWidget *ui;
        QButtonGroup m_ButtonGroup;
        Core::CDataConnector *mp_DataConnector;                     //!< Global data container
        DataManager::CDataProgramList* mp_ProgramList;
        bool m_ProcessRunning;
        Dashboard::CDashboardDateTimeWidget *mp_wdgtDateTime;
        int m_LastSelectedButtonId;
        QDateTime m_ProgramEndDateTime;
        QStringList m_FavProgramIDs;
        QString m_NewSelectedProgramId;
        QDateTime m_EndDateTime;

    };
}
#endif // FAVORITEPROGRAMSPANELWIDGET_H
