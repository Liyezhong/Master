/****************************************************************************/
/*! \file Display.h
 *
 *  \brief  CDisplay class definition
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2013-02-04
 *   $Author:  $ Soumya. D
 *
 *  \b Company:
 *
 *       Leica Biosystems Nussloch GmbH.
 *
 *  (C) Copyright 2010 by Leica Biosystems Nussloch GmbH. All rights reserved.
 *  This is unpublished proprietary source code of Leica. The copyright notice
 *  does not evidence any actual or intended publication.
 *
 */
/****************************************************************************/

#ifndef DIAGNOSTICS_DISPLAY_H
#define DIAGNOSTICS_DISPLAY_H

//#include <QWidget>
//#include <QTranslator>
//#include <QEvent>

#include "Core/Include/ServiceDefines.h"
#include "Diagnostics/Include/BasicColor/BasicColorTestDlg.h"

namespace Diagnostics {

namespace Ui {
class CDisplay;
}

/**********************************************************************************/
/**
 * \brief This widget provides option to perform various tests for display module
 */
/*********************************************************************************/
class CDisplay : public QWidget
{
    Q_OBJECT

public:
    explicit CDisplay(QWidget *p_Parent = 0);
    ~CDisplay();

protected:
    void changeEvent(QEvent *p_Event);

private:
    Ui::CDisplay *mp_Ui;        //!< User Interface
private:
    CBasicColorTestDlg *mp_ColorTestDialog;

signals:
    /****************************************************************************/
    /*!
     *  \brief Set gui tab click status
     *  \iparam Enable = flag of enable
     */
    /****************************************************************************/

    void SetGUITabEnable(bool Enable);

    /**********************************************************************************/
    /**
     * \brief Signal is emitted for basic color test
     */
    /*********************************************************************************/
    void BasicColorTest();    

    /****************************************************************************/
    /*!
     *  \brief Signal emitted for module(factory) test
     *  \iparam Names = name of module
     *  \iparam TestCaseList = list of test case
     */
    /****************************************************************************/
    void BeginModuleTest(Service::ModuleNames_t Names, const QList<Service::ModuleTestCaseID> &TestCaseList);

private slots:
    void RetranslateUI();

public slots:
    void OnBasicColorTest();
    void OnFactoryBasicColorTest();

public:
    void DoBasicColorTest(); //!< Perform color test
};

} // end namespace Diagnostics


#endif // DIAGNOSTICS_DISPLAY_H

