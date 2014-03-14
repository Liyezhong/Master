/****************************************************************************/
/*! \file USBKeyValidator.h
 *
 *  \brief
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2013-04-09
 *   $Author:  $ Soumya. D
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

#ifndef CORE_USBKEYVALIDATOR_H
#define CORE_USBKEYVALIDATOR_H

#include <QWidget>
#include <KeyBoard/Include/KeyBoard.h>
#include "Core/Include/Startup.h"
#include "MainMenu/Include/MessageDlg.h"

namespace Core {
    class CStartup;
}

namespace Core {

/****************************************************************************/
/**
 * \brief This class handles the USB Key Validation.
 */
/****************************************************************************/
class CUSBKeyValidator : public QWidget
{
    Q_OBJECT

public:
    CUSBKeyValidator(Core::CStartup* StartUp);
    ~CUSBKeyValidator();

private:
    KeyBoard::CKeyBoard *mp_KeyBoardWidget;
    QString m_EnteredPin;
    QString m_ServiceKey;
    QString m_ManufacturingKey;
    Core::CStartup *mp_StartUp;
    MainMenu::CMessageDlg *mp_MessageDlg;

    void ConnectKeyBoardSignalSlots();
    void DisconnectKeyBoardSignalSlots();

private slots:
    void OnOkClicked(QString EnteredString);
    void OnESCClicked();

};

}   // end of namespace Core


#endif // CORE_USBKEYVALIDATOR_H
