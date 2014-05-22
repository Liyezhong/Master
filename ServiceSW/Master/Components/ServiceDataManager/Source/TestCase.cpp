#include <QDebug>
#include "ServiceDataManager/Include/TestCase.h"

namespace DataManager {

CTestCase::CTestCase(QString ModuleName, QString CaseName, QString Status):
    m_ModuleName(ModuleName),
    m_CaseName(CaseName),
    m_Status(Status)
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

QString CTestCase::GenReport()
{
    QString StrReport;
    StrReport = m_CaseName + QString(":\n");
    StrReport.append(QString("\tName: %1\n").arg(m_CaseName));
    StrReport.append(QString("\tResult: %1\n").arg(m_Status));

    QMap<QString, QString>::iterator itr = m_Parameters.begin();
    for (; itr != m_Parameters.end(); ++itr) {
        StrReport.append(QString("\t%1 : %2\n").arg(itr.key()).arg(itr.value()));
    }

    itr = m_Results.begin();

    for (; itr != m_Results.end(); ++itr) {
        StrReport.append(QString("\t%1 : %2\n").arg(itr.key()).arg(itr.value()));
    }

    return StrReport;
}

void CTestCase::AddResult(const QString& Key, const QString& Value)
{
    m_Results.insert(Key, Value);
}

void CTestCase::AddParameter(const QString& ParamName, const QString& ParamValue)
{
    m_Parameters.insert(ParamName, ParamValue);
}

} //end of namespace Datamanager

