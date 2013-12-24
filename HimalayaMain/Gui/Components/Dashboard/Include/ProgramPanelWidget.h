#ifndef PROGRAMPANELWIDGET_H
#define PROGRAMPANELWIDGET_H

#include "MainMenu/Include/PanelFrame.h"
#include <QButtonGroup>
#include "HimalayaDataContainer/Helper/Include/Global.h"
#include <QDateTime>

class QDateTime;
namespace MainMenu
{
    class CMainWindow ;
    class CMessageDlg;
}

namespace Core
{
    class CDataConnector;
}

namespace DataManager
{
    class CHimalayaUserSettings;
    class CDataProgramList;
}

namespace Dashboard {

namespace Ui {
    class CProgramPanelWidget;
}

class CProgramPanelWidget : public MainMenu::CPanelFrame
{
    Q_OBJECT
    
public:
    explicit CProgramPanelWidget(QWidget *parent = 0);
    ~CProgramPanelWidget();

    void SetPtrToMainWindow(MainMenu::CMainWindow *p_MainWindow, Core::CDataConnector *p_DataConnector);

signals:
    void AddItemsToFavoritePanel(bool bOnlyAddCleaningProgram);
    void PrepareSelectedProgramChecking(const QString& selectedProgramId);
    void OnSelectEndDateTime(const QDateTime &);
    void ProgramSelected(QString& ProgramId, int asapEndTime);
protected:
    void changeEvent(QEvent *p_Event);

private slots:
    void OnButtonClicked(int whichBtn);
    void OnProgramSelected(QString& ProgramId, int asapEndTime);
    void SelectEndDateTime(const QDateTime &);
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();

private:
    void RetranslateUI();
    void CheckPreConditionsToRunProgram();

    Ui::CProgramPanelWidget *ui;
    QButtonGroup m_btnGroup;                                    //!< Button Group
    DataManager::ProgramActionType_t m_ProgramNextAction;       //!< Program Next Action
    bool m_IsResumeRun;
    QString m_SelectedProgramId;
    Core::CDataConnector *mp_DataConnector;                     //!< Global data container
    DataManager::CHimalayaUserSettings* m_pUserSetting;
    QDateTime m_EndDateTime;
    MainMenu::CMessageDlg   *mp_MessageDlg;                      //!< Message Dialogue
    DataManager::CDataProgramList *mp_ProgramList;
    QString m_strWarning;
    QString m_strNotStartRMSOFF;
    QString m_strOK;

};

}// end namespace Dashboard

#endif // PROGRAMPANELWIDGET_H
