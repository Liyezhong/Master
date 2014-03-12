/****************************************************************************/
/*! \file Troubleshooting.h
 *
 *  \brief Troubleshooting definition.
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

#ifndef BASICSCREENS_TROUBLESHOOTING_H
#define BASICSCREENS_TROUBLESHOOTING_H

#include "BasicScreens/Include/TroublesList.h"
#include "MainMenu/Include/BaseTable.h"
#include <QStandardItemModel>

namespace BasicScreens {

namespace Ui {
    class CTroubleshooting;
}

/****************************************************************************/
/**
 * \brief This class offers a selection table for error log files.
 */
/****************************************************************************/
class CTroubleshooting : public QWidget
{
    Q_OBJECT

public:
    explicit CTroubleshooting(QWidget *p_Parent = 0);
    virtual ~CTroubleshooting();

private:
    Ui::CTroubleshooting *mp_Ui;            //!< User interface
    MainMenu::CBaseTable *mp_TableWidget;   //!< Table listing the logfiles
    BasicScreens::CTroublesList *mp_Dialog; //!< Widget holding the different errors
    QStandardItemModel m_Model;             //!< Model for the table view

private slots:
    void ExecDialog();
};

} // end namespace BasicScreens

#endif // BASICSCREENS_TROUBLESHOOTING_H
