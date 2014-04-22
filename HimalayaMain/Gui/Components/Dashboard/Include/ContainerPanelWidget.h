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
#include "Dashboard/Include/DashboardDataTypes.h"
#include "HimalayaDataContainer/Containers/Programs/Include/DataProgramList.h"


namespace Dashboard {

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
    DataManager::CDataProgramList *mp_ProgramList;       ///<  Definition/Declaration of variable mp_ProgramList
    QStringList m_FavProgramIDs;       ///<  Definition/Declaration of variable m_FavProgramIDs

    bool m_UserRoleChanged;                                     //!< Flag to Verify the Change in User Role
    MainMenu::CMessageDlg   *mp_MessageDlg;                      //!< Message Dialogue
    int m_ParaffinStepIndex;       ///<  Definition/Declaration of variable m_ParaffinStepIndex
    QString m_strProgram;       ///<  Definition/Declaration of variable m_strProgram
    QString m_strInformation;       ///<  Definition/Declaration of variable m_strInformation
    QString m_strOK;       ///<  Definition/Declaration of variable m_strOK
    QString m_strNo;       ///<  Definition/Declaration of variable m_strNo
    QString m_strNotStartExpiredReagent;       ///<  Definition/Declaration of variable m_strNotStartExpiredReagent
    QString m_strStartExpiredReagent;       ///<  Definition/Declaration of variable m_strStartExpiredReagent
    QString m_strNeedMeltParaffin;       ///<  Definition/Declaration of variable m_strNeedMeltParaffin
    DataManager::CHimalayaUserSettings* m_pUserSetting;       ///<  Definition/Declaration of variable m_pUserSetting


    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function RetranslateUI
     *
     *  \return from RetranslateUI
     */
    /****************************************************************************/
    void RetranslateUI();
public:
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function CContainerPanelWidget
     *
     *  \return from CContainerPanelWidget
     */
    /****************************************************************************/
    explicit CContainerPanelWidget(QWidget *p_Parent = 0);
    ~CContainerPanelWidget();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function SetPtrToMainWindow
     *
     *  \return from SetPtrToMainWindow
     */
    /****************************************************************************/
    void SetPtrToMainWindow(MainMenu::CMainWindow *p_MainWindow, Core::CDataConnector *p_DataConnector);
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function UpdateRetortStatus
     *
     *  \return from UpdateRetortStatus
     */
    /****************************************************************************/
    void UpdateRetortStatus(DataManager::ContainerStatusType_t retortStatusType);
    static QString m_strWarning;       ///<  Definition/Declaration of variable m_strWarning
    static QString m_strYes, m_strCancel;       ///<  Definition/Declaration of variable m_strYes

protected:
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function DrawSeparatorLine
     *
     *  \return from DrawSeparatorLine
     */
    /****************************************************************************/
    void DrawSeparatorLine();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function changeEvent
     *
     *  \return from changeEvent
     */
    /****************************************************************************/
    void changeEvent(QEvent *p_Event);
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function Initialize
     *
     *  \return from Initialize
     */
    /****************************************************************************/
    void Initialize();

signals:
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal ProgramAction
     */
    /****************************************************************************/
    void ProgramAction(const QString& programID, DataManager::ProgramActionType_t actionType);
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal UpdateDashboardSceneReagentStations
     */
    /****************************************************************************/
    void UpdateDashboardSceneReagentStations(QString& ProgramID);
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal ProgramSelected
     */
    /****************************************************************************/
    void ProgramSelected(QString & programId, QList<QString>& selectedStationList);
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal OnSelectEndDateTime
     */
    /****************************************************************************/
    void OnSelectEndDateTime(const QDateTime &);
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal OnInteractStart
     */
    /****************************************************************************/
    void OnInteractStart();
public slots:
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of slot UpdateReagentList
     */
    /****************************************************************************/
    void OnRetortLockStatusChanged(const MsgClasses::CmdLockStatus& cmd);
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of slot UpdateReagentList
     */
    /****************************************************************************/
    void OnStationSuckDrain(const MsgClasses::CmdStationSuckDrain & cmd);

};

}
#endif // CONTAINERPANELWIDGET_H
