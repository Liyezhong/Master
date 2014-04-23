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
         *  \param parent = QWidget type parameter
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
         *  \param p_MainWindow = MainMenu::CMainWindow type parameter
         *  \param p_DataConnector =  Core::CDataConnector type parameter
         *
         *  \return from SetPtrToMainWindow
         */
        /****************************************************************************/
        void SetPtrToMainWindow(MainMenu::CMainWindow *p_MainWindow, Core::CDataConnector *p_DataConnector);
        static QString SELECTED_PROGRAM_NAME;       ///<  Definition/Declaration of variable SELECTED_PROGRAM_NAME

    public slots:
        /****************************************************************************/
        /*!
         *  \brief  Definition/Declaration of AddItemsToFavoritePanel
         */
        /****************************************************************************/
        void AddItemsToFavoritePanel(bool bOnlyAddCleaningProgram);
    private slots:
        void OnResetFocus(bool reset);
        /****************************************************************************/
        /*!
         *  \brief  Definition/Declaration of OnEndTimeButtonClicked
         */
        /****************************************************************************/
        void OnEndTimeButtonClicked();
        /****************************************************************************/
        /*!
         *  \brief  Definition/Declaration of OnProcessStateChanged
         */
        /****************************************************************************/
        void OnProcessStateChanged();
        /****************************************************************************/
        /*!
         *  \brief  Definition/Declaration of ProgramSelected
         */
        /****************************************************************************/
        void ProgramSelected(QString& programId, int asapEndTime, bool bProgramStartReady);
        /****************************************************************************/
        /*!
         *  \brief  Definition/Declaration of UndoProgramSelection
         */
        /****************************************************************************/
        void UndoProgramSelection();
        /****************************************************************************/
        /*!
         *  \brief  Definition/Declaration of UpdateProgram
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
        void RequstAsapDateTime();
        void SendAsapDateTime(int);
    private:
        /****************************************************************************/
        /*!
         *  \brief  Definition/Declaration of function SetButtonGroup
         *
         *  \return from SetButtonGroup
         */
        /****************************************************************************/
        void SetButtonGroup();
        QMap<int, QLabel*> m_mapLabel;///< (label Id <--> QLabel)
        Ui::CFavoriteProgramsPanelWidget *ui;       ///<  Definition/Declaration of variable ui
        QButtonGroup m_ButtonGroup;       ///<  Definition/Declaration of variable m_ButtonGroup
        Core::CDataConnector *mp_DataConnector;                     //!< Global data container
	int m_LastCanBeSelectedButtonId;
        QDateTime m_lastSetAsapDatetime;
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

