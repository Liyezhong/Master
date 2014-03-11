/****************************************************************************/
/*! \file TestProtocols.h
 *
 *  \brief TestProtocols definition.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2011-07-08
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

#ifndef BASICSCREENS_TESTPROTCOLS_H
#define BASICSCREENS_TESTPROTCOLS_H

#include "MainMenu/Include/BaseTable.h"
#include "MainMenu/Include/TextDialog.h"
#include <QStandardItemModel>
#include <QWidget>

namespace BasicScreens {

namespace Ui {
    class CTestProtocols;
}

/****************************************************************************/
/**
 * \brief This class includes a selection tables for test protocols.
 */
/****************************************************************************/
class CTestProtocols : public QWidget
{
    Q_OBJECT

public:
    explicit CTestProtocols(QWidget *p_Parent = 0);
    virtual ~CTestProtocols();

private:
    Ui::CTestProtocols *mp_Ui;              //!< User interface
    MainMenu::CBaseTable *mp_TableWidget;   //!< Table listing the test protocols
    MainMenu::CTextDialog *mp_Dialog;       //!< Text view displaying the log file
    QStandardItemModel m_Model;             //!< Model for the table view

private slots:
    void ExecDialog();
};

} // end namespace BasicScreens

#endif // BASICSCREENS_TESTPROTCOLS_H
