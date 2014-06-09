/****************************************************************************/
/*! \file TestCase.cpp
 *
 *  \brief Implementation file for class CTestCase.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2014-5-21
 *   $Author:  $ Dixiong.li
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
#include <QDebug>
#include "ServiceDataManager/Include/TestCase.h"

namespace DataManager {

CTestCase::CTestCase(QString ModuleName, QString CaseName, bool Status):
    m_ModuleName(ModuleName),
    m_CaseName(CaseName),
    m_Status(false)
{

}

CTestCase::~CTestCase()
{
    m_Parameters.clear();
    m_Results.clear();
}

QString CTestCase::GetParameter(const QString& ParamName)
{
    QMap<QString, QString>::iterator itr = m_Parameters.find(ParamName);
    if (itr != m_Parameters.end()) {
        return itr.value();
    }
    else {
        qDebug()<<"CTestCase:GetValue failed the key is "<<ParamName;
        return QString("");
    }
}

void CTestCase::SetParameter(const QString& ParamName, const QString& ParamValue)
{
    m_Parameters[ParamName] = ParamValue;
}

QString CTestCase::GenReport()
{
    QString StrReport;
    QString StrResult;
    if (m_Status) {
        StrResult = "Pass";
    }
    else {
        StrResult = "Fail";
    }
    StrReport = m_CaseName + QString(":\n");
    StrReport.append(QString("\tName: %1\n").arg(m_CaseName));
    StrReport.append(QString("\tResult: %1\n").arg(StrResult));

    QMap<QString, QString>::iterator itr = m_Parameters.begin();
    for (; itr != m_Parameters.end(); ++itr) {
        StrReport.append(QString("\t%1 : %2\n").arg(itr.key()).arg(itr.value()));
    }

    StrReport.append("\n");
    itr = m_Results.begin();

    for (; itr != m_Results.end(); ++itr) {
        StrReport.append(QString("\t%1 : %2\n").arg(itr.key()).arg(itr.value()));
    }

    return StrReport;
}

void CTestCase::AddResult(const QString& Key, const QString& Value)
{
    m_Results[Key] = Value;
}

QMap<QString, QString>& CTestCase::GetResult()
{
    return m_Results;
}

} //end of namespace Datamanager

