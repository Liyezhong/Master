/****************************************************************************/
/*! \file RackGripColorDlg.h
 *
 *  \brief Include file for class CRackGripColorDlg.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2011-09-28
 *   $Author:  $ M.Scherer, C.Adaragunchi1234
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

#ifndef PROGRAMS_RACKGRIPCOLORDLG_H
#define PROGRAMS_RACKGRIPCOLORDLG_H

#include "HimalayaDataContainer/Containers/Programs/Include/DataProgramList.h"
#include "MainMenu/Include/DialogFrame.h"
#include "MainMenu/Include/MainWindow.h"
#include <QButtonGroup>
#include <QMap>

namespace Programs {

namespace Ui {
    class CRackGripColorDlg;
}

/****************************************************************************/
/**
 * \brief Dialog for the selection of the rack color of a  program
 */
/****************************************************************************/
class CRackGripColorDlg : public MainMenu::CDialogFrame
{
    Q_OBJECT

private:
    void UpdateButtons();
    void RetranslateUI();
    Ui::CRackGripColorDlg *mp_Ui;                   //!< User interface
    QButtonGroup m_ButtonGroup;                     //!< Grouping the color buttons
    DataManager::CDataProgramList m_ProgramList;    //!< Local copy of all  programs
    static QStringList m_Colors;                    //!< String list of a colors avilable
    DataManager::CProgram *mp_Program;              //!< Program to be modified
    MainMenu::CMainWindow *mp_MainWindow;           //!< Reference to main window

    //!< Used to send Updated Color given to Program
    //!< Two objects are created because- if an existing program color is replaced by the current program, then
    //!< that program has to be updated to master
    DataManager::CProgram m_Program[2];             //!< Array consisting of two program objects.

    QMap<QString, DataManager::CProgram> m_Programs; //!< Assigns colors to programs
    bool m_ColorReplaced;                            //!< True if Color of program got replaced
    bool m_ProcessRunning;                           //!< Process running state
    DataManager::CProgram m_ProgramClone;            //!< Program object

public:
    explicit CRackGripColorDlg(QWidget *p_Parent = NULL, MainMenu::CMainWindow *p_MainWindow = NULL);
    ~CRackGripColorDlg();
    bool Init(DataManager::CDataProgramList *p_ProgramList, DataManager::CProgram *p_Program);
    void SetSaveButton(QString ButtonText);

private slots:
    void OnSave();
    void OnCancel();
    void OnButtonGroup(int Id);
    void OnProcessStateChanged();

protected:
    void changeEvent(QEvent *p_Event);

signals:
    /****************************************************************************/
    /*!
     *  \brief This signal is emitted when a program is updated.
     *
     *  \iparam Program = Selected Program.
     */
    /****************************************************************************/
    void UpdateProgram(DataManager::CProgram &Program);

    /****************************************************************************/
    /*!
     *  \brief This signal is emitted when the color of one program is assigned to
     *         the selected program.
     *
     *  \iparam Program = Program to be updated.
     *  \iparam ColorReplaced = True if color is changed else False
     */
    /****************************************************************************/
    void UpdateProgramColor(DataManager::CProgram &Program, bool ColorReplaced);

private:
    void EnableColorButtons(bool Enable);
};

} // end namespace Programs

#endif // PROGRAMS_RACKGRIPCOLORDLG_H
