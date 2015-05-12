#ifndef BOTTLECHECKSTATUSDLG_H
#define BOTTLECHECKSTATUSDLG_H

#include "MainMenu/Include/DialogFrame.h"
#include "MainMenu/Include/BaseTable.h"
#include "Settings/Include/BottleCheckStatusModel.h"

namespace Core
{
    class CDataConnector;
}

namespace Settings {

namespace Ui {
    class CBottleCheckStatusDlg;
}

class CBottleCheckStatusDlg : public MainMenu::CDialogFrame
{
    Q_OBJECT
    
public:
    explicit CBottleCheckStatusDlg(QWidget *parent, Core::CDataConnector*);
    ~CBottleCheckStatusDlg();
    void UpdateCurrentStationStatus(const QString& stationID, DataManager::BottleCheckStatusType_t bottleCheckStatusType);
protected:
    void changeEvent(QEvent *p_Event);
private slots:
    void OnClose();
private:
    void RetranslateUI();
    void Inilialize();
    void UpdateStationNotProcess(const QString& stationID);
    Ui::CBottleCheckStatusDlg *ui;
    MainMenu::CBaseTable *mp_TableWidget;
    Settings::CBottleCheckStatusModel m_BottleCheckStatusModel;
    QString m_Started;
    QString m_Waiting;
    QString m_Empty;
    QString m_Passed;
    QString m_Blockage;
    QString m_Checking;
    QString m_WillNotProcess;
    QString m_BuildPressureFailed;
    QString m_Failed;
    QMap<DataManager::BottleCheckStatusType_t, QString> m_BottleCheckStatusMap;
    QMap<QString, int> m_StationIDRowMap;
    QMap<QString, QString> m_StationStatusMap;

};
}
#endif // BOTTLECHECKSTATUSDLG_H
