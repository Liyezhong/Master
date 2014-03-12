/****************************************************************************/
/*! \file Update.h
 *
 *  \brief Update definition.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2011-06-22
 *   $Author:  $ M.Scherer
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

#ifndef BASICSCREENS_UPDATE_H
#define BASICSCREENS_UPDATE_H

#include "MainMenu/Include/WaitDialog.h"
#include <QWidget>

namespace BasicScreens {

namespace Ui {
    class CUpdate;
}

/****************************************************************************/
/**
 * \brief The device update process is started via this class.
 */
/****************************************************************************/
class CUpdate : public QWidget
{
    Q_OBJECT

public:
    explicit CUpdate(QWidget *p_Parent = 0);
    virtual ~CUpdate();

signals:
/****************************************************************************/
/**
 * \brief Sends the update command to the network layer
 *
 * \iparam Name = Command name
 * \iparam ParamList = Parameter string list
 */
/****************************************************************************/
    void ExecSending(const QString Name, const QStringList &ParamList);

private:
    Ui::CUpdate *mp_Ui;                 //!< User interface
    MainMenu::CWaitDialog *mp_Dialog;   //!< Small widget with a business indicator

private slots:
    void ExecDialog();
};

} // end namespace BasicScreens

#endif // BASICSCREENS_UPDATE_H
