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
    static QString GetSelectedProgramId();
    static bool GetProgramPaused();
    static void SetProgramPaused(bool pauseFlag);
    static void UnselectProgram();
    static void SetStationList(const QStringList& list);

    static bool CheckIfCanEdit(const QString& name="", int CheckType=0);  // type 0: No Id 1:Program Name 2: Reagent Name 3: Station 4:ReagentGroup
public slots:
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of slot OnProgramSelected
     */
    /****************************************************************************/
    void OnProgramSelected(QString&, QList<QString>&);
private:
    static QString m_selectedProgramId;
    static QString m_strMsgUnselect;
    static QString m_strWarning;
    static QString m_strCancel;
    static QString m_strYes;
    static QString SELECTED_PROGRAM_NAME;
    static bool m_programIsPaused;
    static QStringList m_StationList;
    static Core::CDataConnector *p_StaticDataConnector;
    Core::CDataConnector *mp_DataConnector;
};

} // end namespace Core

#endif // CORE_GLOBALHELPER_H

