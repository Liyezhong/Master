/****************************************************************************/
/*! \file GlobalHelper.h
 *
 *  \brief GlobalHelper definition.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2014-01-27
 *   $Author:  $ Abe Yang
 *
 *  \b Company:
 *
 *       Leica Microsystems Ltd. Shanghai.
 *
 *  (C) Copyright 2012 by Leica Microsystems Shanghai. All rights reserved.
 *  This is unpublished proprietary source code of Leica. The copyright notice
 *  does not evidence any actual or intended publication.
 *
 */

#ifndef CORE_GLOBALHELPER_H
#define CORE_GLOBALHELPER_H

#include <QObject>

namespace Core {
class CDataConnector;
/****************************************************************************/
/**
 * \brief This class supplies some helpful utilities.
 */
/****************************************************************************/
class CGlobalHelper : public QObject
{
    Q_OBJECT

public:
    CGlobalHelper(Core::CDataConnector *p_DataConnector);
    virtual ~CGlobalHelper();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function CheckSelectedProgram
     *
     *  \param bRevertSelectProgram
     *  \param operatedProgramID
     *
     *  \return from CheckSelectedProgram
     */
    /****************************************************************************/
    static bool CheckSelectedProgram(bool& bRevertSelectProgram,
                              QString operatedProgramID = "");
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function TimeToString
     *
     *  \param seconds = int type parameter
     *  \param showSeconds =  bool type parameter
     *
     *  \return from TimeToString
     */
    /****************************************************************************/
    static QString TimeToString(int seconds, bool showSeconds=false) ;
    static void RetranslateUI();

public slots:
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of slot OnProgramSelected
     */
    /****************************************************************************/
    void OnProgramSelected(QString&, QList<QString>&);
private:
    static QString m_selectedProgramId;       ///<  Definition/Declaration of variable m_selectedProgramId
    static QString m_strMsgUnselect;       ///<  Definition/Declaration of variable m_strMsgUnselect
    static QString m_strWarning;       ///<  Definition/Declaration of variable m_strWarning
    static QString m_strCancel;       ///<  Definition/Declaration of variable m_strCancel
    static QString m_strYes;       ///<  Definition/Declaration of variable m_strYes
    static QString SELECTED_PROGRAM_NAME;       ///<  Definition/Declaration of variable SELECTED_PROGRAM_NAME
    Core::CDataConnector *mp_DataConnector;       ///<  Definition/Declaration of variable mp_DataConnector
};

} // end namespace Core

#endif // CORE_GLOBALHELPER_H

