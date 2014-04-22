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

/****************************************************************************/
/*!
 *  \brief  Definition/Declaration of class Dashboard::CFavoriteProgramsPanelWidget
 */
/****************************************************************************/
    class CFavoriteProgramsPanelWidget : public QWidget
    {
        Q_OBJECT

    public:
        /****************************************************************************/
        /*!
         *  \brief  Definition/Declaration of function CFavoriteProgramsPanelWidget
         *
         *  \return from CFavoriteProgramsPanelWidget
         */
        /****************************************************************************/
        explicit CFavoriteProgramsPanelWidget(QWidget *parent = 0);
        ~CFavoriteProgramsPanelWidget();
        /****************************************************************************/
        /*!
         *  \brief  Definition/Declaration of function SetPtrToMainWindow
         *
         *  \return from SetPtrToMainWindow
         */
        /****************************************************************************/
        void SetPtrToMainWindow(MainMenu::CMainWindow *p_MainWindow, Core::CDataConnector *p_DataConnector);
        static QString SELECTED_PROGRAM_NAME;       ///<  Definition/Declaration of variable SELECTED_PROGRAM_NAME

    public slots:
        /****************************************************************************/
        /*!
         *  \brief  Definition/Declaration of slot UpdateReagentList
         */
        /****************************************************************************/
        void AddItemsToFavoritePanel(bool bOnlyAddCleaningProgram);
    private slots:
        /****************************************************************************/
        /*!
         *  \brief  Definition/Declaration of slot UpdateReagentList
         */
        /****************************************************************************/
        void OnSelectDateTime(const QDateTime&);
        /****************************************************************************/
        /*!
         *  \brief  Definition/Declaration of slot UpdateReagentList
         */
        /****************************************************************************/
        void OnEndTimeButtonClicked();
        /****************************************************************************/
        /*!
         *  \brief  Definition/Declaration of slot UpdateReagentList
         */
        /****************************************************************************/
        void OnProcessStateChanged();
        /****************************************************************************/
        /*!
         *  \brief  Definition/Declaration of slot UpdateReagentList
         */
        /****************************************************************************/
        void ProgramSelected(QString& programId, int asapEndTime, bool bProgramStartReady);
        /****************************************************************************/
        /*!
         *  \brief  Definition/Declaration of slot UpdateReagentList
         */
        /****************************************************************************/
        void UndoProgramSelection();
        /****************************************************************************/
        /*!
         *  \brief  Definition/Declaration of slot UpdateReagentList
         */
        /****************************************************************************/
        void UpdateProgram(DataManager::CProgram &Program);
    signals:
        /****************************************************************************/
        /*!
         *  \brief  Definition/Declaration of signal PrepareSelectedProgramChecking
         */
        /****************************************************************************/
        void PrepareSelectedProgramChecking(const QString& selectedProgramId);
        /****************************************************************************/
        /*!
         *  \brief  Definition/Declaration of signal OnSelectEndDateTime
         */
        /****************************************************************************/
        void OnSelectEndDateTime(const QDateTime &);

    private:
        /****************************************************************************/
        /*!
         *  \brief  Definition/Declaration of function SetButtonGroup
         *
         *  \return from SetButtonGroup
         */
        /****************************************************************************/
        void SetButtonGroup();
        QMap<int, QLabel*> m_mapLabel;//(label Id <--> QLabel)       ///<  Definition/Declaration of variable m_mapLabel
        Ui::CFavoriteProgramsPanelWidget *ui;       ///<  Definition/Declaration of variable ui
        QButtonGroup m_ButtonGroup;       ///<  Definition/Declaration of variable m_ButtonGroup
        Core::CDataConnector *mp_DataConnector;                     //!< Global data container
        DataManager::CDataProgramList* mp_ProgramList;       ///<  Definition/Declaration of variable mp_ProgramList
        bool m_ProcessRunning;       ///<  Definition/Declaration of variable m_ProcessRunning
        Dashboard::CDashboardDateTimeWidget *mp_wdgtDateTime;       ///<  Definition/Declaration of variable mp_wdgtDateTime
        int m_LastSelectedButtonId;       ///<  Definition/Declaration of variable m_LastSelectedButtonId
        QDateTime m_ProgramEndDateTime;       ///<  Definition/Declaration of variable m_ProgramEndDateTime
        QStringList m_FavProgramIDs;       ///<  Definition/Declaration of variable m_FavProgramIDs
        QString m_NewSelectedProgramId;       ///<  Definition/Declaration of variable m_NewSelectedProgramId
        QDateTime m_EndDateTime;       ///<  Definition/Declaration of variable m_EndDateTime

    };
}
#endif // FAVORITEPROGRAMSPANELWIDGET_H
