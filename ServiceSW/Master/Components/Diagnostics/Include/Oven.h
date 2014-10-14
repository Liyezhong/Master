/****************************************************************************/
/*! \file Oven.h
 *
 *  \brief Declaration of Oven test.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2013-05-27
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

#ifndef DIAGNOSTICS_OVEN_H
#define DIAGNOSTICS_OVEN_H

#include <QWidget>
#include "Diagnostics/Include/DiagnosticMessageDlg.h"
#include "Diagnostics/Include/Oven/OvenHeatingTestEmpty.h"

namespace Diagnostics {

namespace Ui {
class COven;
}

class COven : public QWidget
{
    Q_OBJECT

public:
    explicit COven(QWidget *p_Parent = 0);
    ~COven();

protected:
    void changeEvent(QEvent *p_Event);

private Q_SLOTS:
    void StartHeatingTestEmpty(void);
    void StartCoverSensorTest(void);
    void HeatingTestEmptyClose();

private:
    Ui::COven *ui;
    CDiagnosticMessageDlg *dlg;
    Oven::CHeatingTestEmpty *heatingTestEmpty;

signals:
    void GuiOvenEmptyHeatingTest();
};


} // namespace Diagnostics
#endif // DIAGNOSTICS_OVEN_H
