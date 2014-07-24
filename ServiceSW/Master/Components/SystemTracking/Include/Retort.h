/****************************************************************************/
/*! \file Retort.h
 *
 *  \brief Declaration of Retort configuration class.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2013-05-28
 *   $Author:  $ R.Wu
 *
 *  \b Company:
 *
 *       Leica Biosystems R&D Center Shanghai.
 *
 *  (C) Copyright 2010 by LBS R&D Center Shanghai. All rights reserved.
 *  This is unpublished proprietary source code of Leica. The copyright notice
 *  does not evidence any actual or intended publication.
 *
 */
/****************************************************************************/

#ifndef SYSTEMTRACKING_RETORT_H
#define SYSTEMTRACKING_RETORT_H

#include <QWidget>

#include "Core/Include/ServiceGUIConnector.h"

namespace SystemTracking {

namespace Ui {
class CRetort;
}

/****************************************************************************/
/**
 * \brief This is the widget displaying to modify Retort config.
 */
/****************************************************************************/
class CRetort : public QWidget
{
    Q_OBJECT
    
public:
    /****************************************************************************/
    /**
     * \brief  Default constructor.
     * \iparam DataConnector = service Gui connector
     * \iparam p_Parent = parent of widget
     */
    /****************************************************************************/
    explicit CRetort(Core::CServiceGUIConnector &DataConnector,
                     QWidget *p_Parent = 0);

    /****************************************************************************/
    /**
     * \brief Destructor.
     */
    /****************************************************************************/
    ~CRetort(void);

Q_SIGNALS:
    /****************************************************************************/
    /*!
     *  \brief  signal for module list changed.
     */
    /****************************************************************************/
    void ModuleListChanged(void);

public Q_SLOTS:
    /****************************************************************************/
    /*!
     *  \brief  slot for update module.
     *  \param  Module = module object
     */
    /****************************************************************************/
    void UpdateModule(ServiceDataManager::CModule &Module);

    /****************************************************************************/
    /*!
     *  \brief  slot for update sub module.
     *  \param  SubModule = sub module object
     */
    /****************************************************************************/
    void UpdateSubModule(ServiceDataManager::CSubModule &SubModule);

    /****************************************************************************/
    /*!
     *  \brief  slot for modify Retort.
     */
    /****************************************************************************/
    void ModifyRetort(void);

    /****************************************************************************/
    /*!
     *  \brief  slot for modify Heater.
     */
    /****************************************************************************/
    void ModifyHeater(void);

    /****************************************************************************/
    /*!
     *  \brief  slot for modify Lid lock.
     */
    /****************************************************************************/
    void ModifyLidLock(void);

    /****************************************************************************/
    /*!
     *  \brief  slot for modify Level sensor.
     */
    /****************************************************************************/
    void ModifyLevelSensor(void);

    /****************************************************************************/
    /*!
     *  \brief  slot for finalize configuration.
     */
    /****************************************************************************/
    void OnFinalizeConfiguration(void);

    /****************************************************************************/
    /*!
     *  \brief  call slot when current tab changed.
     *  \param  Index = tab index
     */
    /****************************************************************************/
    void CurrentTabChanged(int Index);

    /****************************************************************************/
    /*!
     *  \brief  slot for confirm module configuration.
     */
    /****************************************************************************/
    void ConfirmModuleConfiguration();

private:
    /****************************************************************************/
    /*!
     *  \brief  To modify sub module info.
     *  \param  ModuleName    = the module name
     *  \param  SubMoudleName = the sub module name
     */
    /****************************************************************************/
    void ModifySubModule(const QString &ModuleName,
                         const QString &SubModuleName);

    /****************************************************************************/
    /*!
     *  \brief  Pop's up to confirm module configuration.
     *  \param  Text = the message to confirm string.
     */
    /****************************************************************************/
    void ConfirmModuleConfiguration(QString& Text);

    /****************************************************************************/
    /*!
     *  \brief  reset message box.
     */
    /****************************************************************************/
    void ResetMessageBox();

    Ui::CRetort *mp_Ui;                                  //!< User Interface
    Core::CServiceGUIConnector *mp_DateConnector;        //!< Global data connector
    MainMenu::CMessageDlg *mp_MessageDlg;                //!< Information dialog
    ServiceDataManager::CModuleDataList *mp_ModuleList;  //!< Current list of modules
};


} // namespace SystemTracking
#endif // SYSTEMTRACKING_RETORT_H
