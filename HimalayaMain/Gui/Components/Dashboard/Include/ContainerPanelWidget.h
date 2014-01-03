/****************************************************************************/
/*! \file ContainerPanelWidget.h
 *
 *  \brief ContainerPanel Widget Class Definition.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2013-01-03
 *   $Author:  $ Swati Tiwari
 *
 *  \b Company:
 *
 *       Leica Microsystems Ltd. Shanghai.
 *
 *  (C) Copyright 2012 by Leica Microsystems Shanghai. All rights reserved.
 *  This is unpublished proprietary source code of Leica. The copyright notice
 *  does not evidence any actual or intended publication.
 *
 */
/****************************************************************************/

#ifndef CONTAINERPANELWIDGET_H
#define CONTAINERPANELWIDGET_H

#include "MainMenu/Include/PanelFrame.h"
#include "MainMenu/Include/MessageDlg.h"
#include "Dashboard/Include/DashboardScene.h"
#include "Dashboard/Include/DashboardComboBox.h"
#include "Dashboard/Include/DashboardDataTypes.h"
#include "HimalayaDataContainer/Containers/Programs/Include/DataProgramList.h"


namespace Dashboard {

class CDashboardProgramStatusWidget;

namespace Ui {
    class CContainerPanelWidget;
}


/****************************************************************************/
/**
 * \brief This is the widget for container panel widget.
 */
/****************************************************************************/
class CContainerPanelWidget : public MainMenu::CPanelFrame
{
    Q_OBJECT

private:
    Ui::CContainerPanelWidget *mp_Ui;                                //!< User interface
    MainMenu::CMainWindow *mp_MainWindow;                           //!< Reference to MainWindow.
    Core::CDataConnector *mp_DataConnector;                     //!< Global data container
    CDashboardScene *mp_DashboardScene;                         //!< Displays the station items
    DataManager::CDataProgramList *mp_ProgramList;
    QStringList m_FavProgramIDs;

    CDashboardProgramStatusWidget *mp_ProgramStatusWidget;
    bool m_UserRoleChanged;                                     //!< Flag to Verify the Change in User Role
    MainMenu::CMessageDlg   *mp_MessageDlg;                      //!< Message Dialogue
    int m_ParaffinStepIndex;
    QString m_strProgram;
    QString m_strInformation;
    QString m_strOK, m_strNo;
    QString m_strNotStartExpiredReagent;
    QString m_strStartExpiredReagent;
    QString m_strNeedMeltParaffin;
    DataManager::CHimalayaUserSettings* m_pUserSetting;


    void RetranslateUI();
public:
    explicit CContainerPanelWidget(QWidget *p_Parent = 0);
    ~CContainerPanelWidget();
    void SetPtrToMainWindow(MainMenu::CMainWindow *p_MainWindow, Core::CDataConnector *p_DataConnector);
    void AddItemsToComboBox(bool bOnlyAddCleaningProgram = false);
    void UpdateRetortStatus(DataManager::ContainerStatusType_t retortStatusType);
    static QString m_strWarning;
    static QString m_strYes, m_strCancel;

protected:
    void DrawSeparatorLine();
    void changeEvent(QEvent *p_Event);
    void Initialize();

signals:
    void ProgramAction(const QString& ProgramID, DataManager::ProgramActionType_t ActionType);
    void UpdateDashboardSceneReagentStations(QString& ProgramID);
    void ProgramSelected(QString & ProgramId, QList<QString>& SelectedStationList);
    void OnSelectEndDateTime(const QDateTime &);
public slots:
    void OnProcessStateChanged();
    void OnRetortLockStatusChanged(const MsgClasses::CmdRetortLockStatus& cmd);
    void OnStationSuckDrain(const MsgClasses::CmdStationSuckDrain & cmd);

};

}
#endif // CONTAINERPANELWIDGET_H
