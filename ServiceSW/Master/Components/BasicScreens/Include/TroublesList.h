/****************************************************************************/
/*! \file TroublesList.h
 *
 *  \brief TroublesList definition.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2011-07-05
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

#ifndef BASICSCREENS_TROUBLESLIST_H
#define BASICSCREENS_TROUBLESLIST_H

#include "MainMenu/Include/BaseTable.h"
#include "MainMenu/Include/DialogFrame.h"
#include "MainMenu/Include/TextDialog.h"
#include <QStandardItemModel>

namespace BasicScreens {

namespace Ui {
    class CTroublesList;
}

/****************************************************************************/
/**
 * \brief This widget displays the most recent entries of an error logfile.
 */
/****************************************************************************/
class CTroublesList : public MainMenu::CDialogFrame
{
    Q_OBJECT

public:
    explicit CTroublesList(QWidget *p_Parent = 0);
    virtual ~CTroublesList();
    void AddItem(QString Timestamp, QString Origin, QString Id, QString Description, QString PossibleCauses);

private:
    Ui::CTroublesList *mp_Ui;               //!< User interface
    MainMenu::CBaseTable *mp_TableWidget;   //!< Table listing the errors
    MainMenu::CTextDialog *mp_Dialog;   //!< Text containg possible error causes
    QStandardItemModel m_Model;             //!< Model for the table view

private slots:
    void ExecDialog();
};

} // end namespace BasicScreens

#endif // BASICSCREENS_TROUBLESLIST_H
