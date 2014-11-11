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
#include "Diagnostics/Include/DiagnosticMessageDlg.h"
#include <QList>
#include <QLabel>

namespace Diagnostics {

namespace Ui {
    class CLiquidHoseTestDlg;
}

namespace System {

/****************************************************************************/
/*!
*   \brief This class implements the functionality to system Liquid hose test dialog
*/
/****************************************************************************/
class CLiquidHoseTestDlg : public MainMenu::CDialogFrame
{
    Q_OBJECT

    /****************************************************************************/
    /*!
    *   \brief The struct of Label group
    */
    /****************************************************************************/
    struct LabelGroup {
        LabelGroup(QLabel* label1, QLabel*label2, QLabel*label3) {
            bottleLabel   = label1;
            pressureLabel = label2;
            resultLabel   = label3;
        }
        QLabel* bottleLabel;        //!< label of the bottle number.
        QLabel* pressureLabel;      //!< label of the pressure value.
        QLabel* resultLabel;        //!< label of the test result.
    };
public:
    /****************************************************************************/
    /*!
     *  \brief Constructor
     *  \iparam p_Message = The pointer of message dlg.
     *  \iparam p_Parent = Parent widget
     */
    /****************************************************************************/
    explicit CLiquidHoseTestDlg(CDiagnosticMessageDlg* p_MessageDlg, QWidget *p_Parent = 0);

    /****************************************************************************/
    /*!
     *  \brief Destructor
     */
    /****************************************************************************/
    virtual ~CLiquidHoseTestDlg();

    /****************************************************************************/
    /*!
     *  \brief To run this test.
     *  \return true or false.
     */
    /****************************************************************************/
    bool RunTest();

    /****************************************************************************/
    /*!
     *  \brief Add bottle number to test list.
     *  \iparam BottleNumber = the number of the bottle.
     */
    /****************************************************************************/
    void AddBottleNumber(int BottleNumber);

private slots:

    /****************************************************************************/
    /*!
     *  \brief Slot of the Abort.
     */
    /****************************************************************************/
    void OnAbort();

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

    /****************************************************************************/
    /*!
     *  \brief Update label status.
     *  \iparam Index = the label index.
     *  \iparam BottleNumber = the value of the bottle number.
     *  \iparam Pressure     = the value of the pressure.
     */
    /****************************************************************************/
    void UpdateLabel(int Index, int BottleNumber, float Pressure);

    /****************************************************************************/
    /*!
     *  \brief To create pressure and show status to GUI.
     *  \iparam TargetPressure = the target pressure.
     *  \iparam TimeOut        = the sec of the time out.
     *  \return true or false.
     */
    /****************************************************************************/
    bool CreatePressure(float TargetPressure, int TimeOut);

    /****************************************************************************/
    /*!
     *  \brief Get record pressure.
     *  \iparam RecordTime = the record times.
     */
    /****************************************************************************/
    float GetRecordPressure(int RecordTime);

    /****************************************************************************/
    /*!
     *  \brief To show finished dialog.
     *  \iparam RetNum = The number return code.
     */
    /****************************************************************************/
    void ShowFinishDlg(int RetNum);

private:
    Ui::CLiquidHoseTestDlg *mp_Ui;        //!< User interface
    CDiagnosticMessageDlg*  mp_MessageDlg;  //!< The pointer of message dialog.
    QList<LabelGroup*> m_LabelGroups;     //!< label group
    QList<int> m_BottleNumberList;        //!< store the bottle number
    QString    m_MessageTitle;            //!< The message dialog title.
    bool m_Abort;                         //!< Flag for abort.

};

} // namespace System

} // end namespace Diagnostics

#endif // DIAGNOSTICS_SYSTEM_LIQUIDHOSETESTDLG_H
