/****************************************************************************/
/*! \file DlgSelectTestOptions.h
 *
 *  \brief
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2014-05-12
 *   $Author:  $ Abe Yang
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

#ifndef DLGSELECTTESTOPTIONS_H
#define DLGSELECTTESTOPTIONS_H

#include <QWidget>

class QButtonGroup;

namespace MainMenu {

namespace Ui {
    class CDlgSelectTestOptions;
}

class CDlgSelectTestOptions : public QWidget
{
    Q_OBJECT

public:
    explicit CDlgSelectTestOptions(QWidget *parent = 0);
    ~CDlgSelectTestOptions(void);
    void EnableNextButton(bool bEnable);
protected:
    void changeEvent(QEvent *p_Event);

signals:
    void RadioBtnSelected(int radioBtnIndex);

private:
    Ui::CDlgSelectTestOptions *ui;
    QButtonGroup* m_pButtonGroup;
};

} // namespace MainMenu

#endif // DLGSELECTTESTOPTIONS_H
