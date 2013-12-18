#ifndef CDASHBOARDWIDGET2_H
#define CDASHBOARDWIDGET2_H

#include <QWidget>
#include <QDateTime>

namespace Core
{
    class CDataConnector;
}

namespace MsgClasses
{
    class CmdProgramSelectedReply;
}

namespace MainMenu
{
    class CMainWindow;
    class CMessageDlg;
}

namespace DataManager
{
    class CDataProgramList;
    class CProgram;
    class CHimalayaUserSettings;
}

namespace Dashboard {

namespace Ui {
    class CDashboardWidget;
}

class CDashboardWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit CDashboardWidget(Core::CDataConnector *p_DataConnector,
                               MainMenu::CMainWindow *p_Parent = NULL);
    ~CDashboardWidget();

protected:
    void changeEvent(QEvent *p_Event);

private:
    bool IsParaffinInProgram(const DataManager::CProgram* p_Program);
    int GetASAPTime(int, int, int, bool&);
    void RetranslateUI();
    Ui::CDashboardWidget *ui;
    Core::CDataConnector *mp_DataConnector;          //!< Data object
    MainMenu::CMainWindow *mp_MainWindow;           //!< Reference to main window.
    DataManager::CDataProgramList *mp_ProgramList;
    QString m_NewSelectedProgramId;
    int m_ParaffinStepIndex;
    DataManager::CHimalayaUserSettings* m_pUserSetting;
    MainMenu::CMessageDlg   *mp_MessageDlg;                      //!< Message Dialogue
    QString m_strCheckSafeReagent;
    QString m_strNotFoundStation;
    QString m_strCheckEmptyStation;
    static QString m_SelectedProgramId;
    QList<QString> m_StationList;
    int m_TimeProposed;
    QDateTime m_EndDateTime;

public slots:
    void OnUnselectProgram();


private slots:
    void PrepareSelectedProgramChecking(const QString& selectedProgramId);
    void OnProgramSelectedReply(const MsgClasses::CmdProgramSelectedReply& cmd);
    void OnSelectEndDateTime(const QDateTime&);

 signals:
    void AddItemsToFavoritePanel(bool bOnlyAddCleaningProgram = false);
    void ProgramSelected(QString & ProgramId, QList<QString>& SelectedStationList);
    void ProgramSelected(QString & ProgramId, int asapEndTime);
    void UpdateSelectedStationList(QList<QString>&);

};

} // end namespace Dashboard

#endif // CDASHBOARDWIDGET2_H
