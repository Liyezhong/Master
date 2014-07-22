/****************************************************************************/
/*! \file Oven.h
 *
 *  \brief Declaration of Oven configuration class.
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

#ifndef SYSTEMTRACKING_OVEN_H
#define SYSTEMTRACKING_OVEN_H

#include <QWidget>

#include "Core/Include/ServiceGUIConnector.h"

namespace SystemTracking {

namespace Ui {
class COven;
}

class COven : public QWidget
{
    Q_OBJECT
    
public:
    /****************************************************************************/
    /**
     * \brief  Default constructor.
     * \iparam p_DataConnector = service Gui connector, parent = parent of widget
     */
    /****************************************************************************/
    explicit COven(Core::CServiceGUIConnector &DataConnector,
                   QWidget *parent = 0);

    /****************************************************************************/
    /**
     * \brief Destructor.
     */
    /****************************************************************************/
    ~COven(void);

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
     *  \brief  slot for modify Oven.
     */
    /****************************************************************************/
    void ModifyOven(void);

    /****************************************************************************/
    /*!
     *  \brief  slot for modify Heater.
     */
    /****************************************************************************/
    void ModifyHeater(void);

    /****************************************************************************/
    /*!
     *  \brief  slot for modify Cover Sensor.
     */
    /****************************************************************************/
    void ModifyCoverSensor(void);

    /****************************************************************************/
    /*!
     *  \brief  slot for finalize configuration.
     */
    /****************************************************************************/
    void OnFinalizeConfiguration();

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

    Ui::COven *mp_Ui;                                    //!< User Interface
    Core::CServiceGUIConnector *mp_DataConnector;        //!< Global data connector
    MainMenu::CMessageDlg *mp_MessageDlg;                //!< Information dialog
    ServiceDataManager::CModuleDataList *mp_ModuleList;  //!< Current list of modules
};


} // namespace SystemTracking

#endif // SYSTEMTRACKING_OVEN_H
