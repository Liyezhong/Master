/****************************************************************************/
/*! \file LiquidHoseTestDlg.h
 *
 *  \brief Declaration of System Liquid hose test dialog.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2014-10-21
 *   $Author:  $ Dixiong Li
 *
 *  \b Company:
 *
 *       Leica Biosystems Shanghai.
 *
 *  (C) Copyright 2010 by Leica Biosystems Shanghai. All rights reserved.
 *  This is unpublished proprietary source code of Leica. The copyright notice
 *  does not evidence any actual or intended publication.
 *
 */
/****************************************************************************/
#ifndef DIAGNOSTICS_SYSTEM_LIQUIDHOSETESTDLG_H
#define DIAGNOSTICS_SYSTEM_LIQUIDHOSETESTDLG_H

#include "MainMenu/Include/DialogFrame.h"
#include <QList>
#include <QLabel>

namespace Diagnostics {

namespace Ui {
    class CLiquidHoseTestDlg;
}

namespace System {

class CLiquidHoseTestDlg : public MainMenu::CDialogFrame
{
    Q_OBJECT

    struct LabelGroup {
        LabelGroup(QLabel* label1, QLabel*label2, QLabel*label3) {
            bottleLabel   = label1;
            pressureLabel = label2;
            resultLabel   = label3;
        }
        QLabel* bottleLabel;
        QLabel* pressureLabel;
        QLabel* resultLabel;
    };
public:
    /****************************************************************************/
    /*!
     *  \brief Constructor
     *
     *  \iparam p_Parent = Parent widget
     */
    /****************************************************************************/
    explicit CLiquidHoseTestDlg(QWidget *p_Parent = 0);

    /****************************************************************************/
    /*!
     *  \brief Destructor
     */
    /****************************************************************************/
    virtual ~CLiquidHoseTestDlg();

    void RunTest();

    void AddBottleNumber(int BottleNumber);

private slots:

    void OnAbort();

    void OnOk();
protected:
    /****************************************************************************/
    /*!
     *  \brief Event filter
     *  \iparam p_Object = QObject
     *  \iparam p_Event = event object
     */
    /****************************************************************************/
    bool eventFilter(QObject *p_Object, QEvent *p_Event);

private:
    /****************************************************************************/
    /*!
     *  \brief label init
     */
    /****************************************************************************/
    void InitLableGroup();

    void UpdateLabel(int Index, int BottleNumber, float Pressure);

    bool CreatePressure(float TargetPressure, int TimeOut);

    float GetRecordPressure(int RecordTime);

private:
    Ui::CLiquidHoseTestDlg *mp_Ui;        //!< User interface
    QList<LabelGroup*> m_LabelGroups;     //!< label group
    QList<int> m_BottleNumberList;
    int m_Index;
    bool m_Abort;                         //!< Flag for abort.

};

} // namespace System

} // end namespace Diagnostics

#endif // DIAGNOSTICS_SYSTEM_LIQUIDHOSETESTDLG_H
