/****************************************************************************/
/*! \file TestCaseFactory.cpp
 *
 *  \brief Implementation file for class CTestCaseFactory.
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
#include <QtXml/QXmlStreamReader>
#include <QDebug>
#include <QFile>
#include "DataManager/Helper/Include/Helper.h"
#include "ServiceDataManager/Include/TestCaseFactory.h"

namespace DataManager {

CTestCaseFactory CTestCaseFactory::m_TestCaseFactory;

CTestCaseFactory::CTestCaseFactory()
{

}

CTestCaseFactory::~CTestCaseFactory()
{
    QHash<QString, CTestCase*>::iterator itr = m_TestCases.begin();
    for (; itr != m_TestCases.end(); ++itr) {
        delete itr.value();
    }
    m_TestCases.clear();
}

CTestCase* CTestCaseFactory::GetTestCase(const QString& CaseName)
{
    CTestCase* p_TestCase = NULL;
    QHash<QString, CTestCase*>::iterator itr = m_TestCases.find(CaseName);
    if (itr != m_TestCases.end()) {
        p_TestCase = itr.value();
    }
    return p_TestCase;
}

QString CTestCaseFactory::GetParameter(const QString& CaseName, const QString& ParamName)
{
    CTestCase* p_TestCase = GetTestCase(CaseName);
    if (p_TestCase) {
        return p_TestCase->GetParameter(ParamName);
    }
    else {
        qDebug()<<"CTestCaseFactory:GetParameter failed:"<<CaseName;
        return "";
    }
}

QList<CTestCase*> CTestCaseFactory::GetModuleTestCase(const QString& ModuleName)
{
    QList<CTestCase*> TestCaseList;
    QHash<QString, CTestCase*>::iterator itr = m_TestCases.begin();
    for (; itr != m_TestCases.end(); ++itr) {
        CTestCase* p_TestCase = itr.value();
        if (p_TestCase->m_ModuleName == ModuleName) {
            TestCaseList<<p_TestCase;
        }
    }

    return TestCaseList;
}

bool CTestCaseFactory::InitData(QString FileName)
{
    bool Result = true;
    QFile File(FileName);
    if (!QFile::exists(FileName)) {
        return false;
    }
    if (!File.open(QFile::ReadOnly | QFile::Text )) {
        qDebug() << "Open file to read failed " << FileName;
        return false;
    }

    if (!DeserializeContent(File)) {
        qDebug() << " CTestCaseFactory::Read failed for file during deserializing: " << FileName;
        return false;
    }

    //m_FileName = FileName;

    File.close();

    return Result;
}

bool CTestCaseFactory::DeserializeContent(QIODevice& IODevice)
{
    QString ModuleName;
    QXmlStreamReader XmlStreamReader;
    XmlStreamReader.setDevice(&IODevice);

    // look for node <ServiceParameters>
    if (!Helper::ReadNode(XmlStreamReader, "TestCaseConfig")) {
        qDebug() << "DeserializeContent: abort reading. Node not found: TestCaseConfig";
        return false;
    }

    // Read attribute Name
    if (!XmlStreamReader.attributes().hasAttribute("Version")) {
        qDebug() << " attribute TestCaseConfig <Version> is missing => abort reading";
        return false;
    }

    //SetVerion(XmlStreamReader.attributes().value("Version").toString().toInt());

    while (!XmlStreamReader.atEnd())
    {
        (void)XmlStreamReader.readNextStartElement();

        if (XmlStreamReader.isStartElement())
        {
            QXmlStreamAttributes XmlAttributes = XmlStreamReader.attributes();
            if (XmlAttributes.size() == 0)
            {
                ModuleName = XmlStreamReader.name().toString();
            }
            else
            {
                QString CaseName = XmlStreamReader.name().toString();
                CTestCase* p_TestCase = new CTestCase(ModuleName, CaseName);
                for (int i = 0 ; i < XmlAttributes.size(); ++i)
                {
                    QString AttributeName = XmlAttributes[i].name().toString();
                    p_TestCase->SetParameter(AttributeName, XmlAttributes.value(AttributeName).toString());
                }

                qDebug()<<"-------------------------------------Case Name="<<CaseName;

                m_TestCases.insert(CaseName, p_TestCase);
            }
        }
    }

    return true;
}

}// end of namespace DataManager
