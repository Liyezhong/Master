/****************************************************************************/
/*! \file ErrorLogging.h
 *
 *  \brief ErrorLogging definition.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2011-06-29
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

#ifndef BASICSCREENS_ERRORLOGGING_H
#define BASICSCREENS_ERRORLOGGING_H

#include "MainMenu/Include/BaseTable.h"
#include "MainMenu/Include/TextDialog.h"
#include <QStandardItemModel>
#include <QWidget>

namespace BasicScreens {

namespace Ui {
    class CErrorLogging;
}

/****************************************************************************/
/**
 * \brief This widget allows the user to read error logs.
 */
/****************************************************************************/
class CErrorLogging : public QWidget
{
    Q_OBJECT

public:
    explicit CErrorLogging(QWidget *p_Parent = 0);
    virtual ~CErrorLogging();

private:
    Ui::CErrorLogging *mp_Ui;               //!< User interface
    MainMenu::CBaseTable *mp_TableWidget;   //!< Table displaying logfiles
    MainMenu::CTextDialog *mp_Dialog;       //!< Text view in the dialog
    QStandardItemModel m_Model;             //!< Model for the table view

private slots:
    void ExecDialog();
};

} // end namespace BasicScreens

#endif // BASICSCREENS_ERRORLOGGING_H
