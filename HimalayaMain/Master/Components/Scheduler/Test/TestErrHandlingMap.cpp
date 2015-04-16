/****************************************************************************/
/*! \file TestErrHandlingMap.cpp
 *
 *  \brief Implementation file for class TestErrHandlingMap.
 *
 *  $Version:   $ 0.1
 *  $Date:      $ 2015-4-1
 *  $Author:    $ Shufa Li
 *
 *  \b Company:
 *
 *       Leica Biosystems Nussloch GmbH.
 *
 *  (C) Copyright 2013 by Leica Biosystems Nussloch GmbH. All rights reserved.
 *  This is unpublished proprietary source code of Leica. The copyright notice
 *  does not evidence any actual or intended publication.
 *
 */
/****************************************************************************/

#include <QTest> 

#include <Global/Include/Exception.h>
#include <Global/Include/Utils.h>
#include <Scheduler/Include/EventScenarioErrorXMLInfo.h>
#include <EventHandler/Include/EventXMLInfo.h>

using namespace EventHandler;

namespace Scheduler {

/****************************************************************************/
/**
 * \brief Test class for XmlConfigFile class.
 */
/****************************************************************************/
class TestErrHandlingMap: public QObject {
    Q_OBJECT

private:
    QSharedPointer<EventScenarioErrXMLInfo> m_pESEXMLInfo;
    QSharedPointer<EventXMLInfo> m_pEventXMLInfo;

    QString GetSafeReagentType(const QString& Scenario);

private slots:
    /****************************************************************************/
    /**
     * \brief Called before the first testfunction is executed.
     */
    /****************************************************************************/
    void initTestCase();
    /****************************************************************************/
    /**
     * \brief Called before each testfunction is executed.
     */
    /****************************************************************************/
    void init();
    /****************************************************************************/
    /**
     * \brief Called after each testfunction was executed.
     */
    /****************************************************************************/
    void cleanup();
    /****************************************************************************/
    /**
     * \brief Called after last testfunction was executed.
     */
    /****************************************************************************/
    void cleanupTestCase();
    /****************************************************************************/

    void UTAll();

}; // end class TestEventCSVInfo


QString TestErrHandlingMap::GetSafeReagentType(const QString& Scenario)
{
    QString SafeReagentType = "";
    quint32 scenario = Scenario.toInt();
    if(200 == scenario)
    {
        SafeReagentType = "Program_first_position";
    }
    else if(scenario >= 211 && scenario <= 217)
    {
        SafeReagentType = "TissueProtect_Fixation" ;
    }
    else if(scenario >= 221 && scenario <= 247)
    {
        SafeReagentType = "TissueProtect_min_concentration_Dehydration";
    }
    else if(scenario >= 251 && scenario <= 260)
    {
        SafeReagentType = "TissueProtect_clearing";
    }
    else if(scenario >= 271 && scenario <= 277)
    {
        SafeReagentType = "TissueProtect_paraffin";
    }
    return SafeReagentType;
}

void TestErrHandlingMap::UTAll()
{
    QFile file("../Settings/ErrHandlingEventScenario.csv");
    if( !file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        //if the file not exist, create it again
        file.close();
        if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            qDebug() << "Open ErrHandlingEventScenario file error!" << file.errorString();
            file.close();
            QVERIFY(false);
        }
    }
    QTextStream stream(&file);
    stream << "#EventId; ScenarioId; ErrorId; Type; ResponseId; SafeReagentGroup;" << "\n";

    QHash< quint32,QSharedPointer<ESEInfo> > ESEInfoList = m_pESEXMLInfo->GetHashESEInfoList();
    QHash< quint32, QSharedPointer<XMLEvent> > EventConfigList = m_pEventXMLInfo->GetEventList();

    quint32 ErrorID = 0;
    quint32 EventID = 0;
    QString Scenario;

    QHash< quint32,QSharedPointer<ESEInfo> >::iterator iter = ESEInfoList.begin();
    while (iter != ESEInfoList.end())
    {
        EventID = iter.key();
        QHash<QString, quint32> scenarioErrList = iter.value()->GetScenarioErrorList();
        QHash<QString, quint32>::iterator sceErrIter = scenarioErrList.begin();

        int size = scenarioErrList.size();
        if (size == 1 && sceErrIter.key() == "all")
        {
            stream <<"EventId:"<< EventID <<";  ScenarioId:";
            stream << sceErrIter.key();
            ErrorID = sceErrIter.value();
            stream <<";  ErrorId:" << ErrorID;

            QHash< quint32, QSharedPointer<XMLEvent> >::iterator ErrorIter = EventConfigList.find(ErrorID);
            if(ErrorIter != EventConfigList.end())
            {
                if(ErrorIter.value()->GetStep(0))
                {
                    QString Type = ErrorIter.value()->GetStep(0)->GetType();
                    if( "MSG" ==  Type)
                    {
                        stream << ";  Type:MSG;  ResponseId:/";
                    }
                    else
                    {
                        QString Action = ErrorIter.value()->GetStep(0)->GetAction();
                        stream << ";  Type:" << Type << ";  ResponseId:" << Action;
                    }
                }
                stream << ";  SafeReagentGroup:/;\n";
            }
        }
        else
        {
            while(sceErrIter != scenarioErrList.end())
            {
                ErrorID = sceErrIter.value();
                stream <<"EventId:"<< EventID <<";  ScenarioId:";
                Scenario = sceErrIter.key().right(3);
                stream << Scenario;
                stream <<";  ErrorId:" << ErrorID;

                QHash< quint32, QSharedPointer<XMLEvent> >::iterator ErrorIter = EventConfigList.find(ErrorID);
                if(ErrorIter != EventConfigList.end())
                {
                    bool HasSafeReagent = false;
                    int i = 0;
                    while(ErrorIter.value()->GetStep(i) != NULL)
                    {
                        QString Type = ErrorIter.value()->GetStep(i)->GetType();
                        if("MSG" ==  Type && 0 == i)
                        {
                            stream << ";  Type:MSG;  ResponseId:/";
                        }
                        else
                        {
                            QString Action = ErrorIter.value()->GetStep(i)->GetAction();
                            if(0 == i)
                            {
                                stream << ";  Type:" << Type << ";  ResponseId:" << Action;
                            }
                            if("RS_Tissue_Protect" == Action)
                            {
                                HasSafeReagent = true;
                                break;
                            }
                        }
                        i++;
                    }
                    if(HasSafeReagent)
                    {
                        stream << ";  SafeReagentGroup:" << GetSafeReagentType(Scenario) << ";\n";
                    }
                    else
                    {
                        stream << ";  SafeReagentGroup:/;\n";
                    }
                }
                sceErrIter++;
            }
        }
        iter++;
    }

    file.close();
}

/******************************************************************ls**********/
void TestErrHandlingMap::initTestCase()
{
    bool ret = false;
    QString AbsCurrentPath = QCoreApplication::applicationDirPath();
    QString EventScenarioErrorxmlFilePath = AbsCurrentPath + "/../../../Main/Build/Settings/EventScenarioErrorMap.xml";
    m_pESEXMLInfo = QSharedPointer<EventScenarioErrXMLInfo>(new EventScenarioErrXMLInfo(EventScenarioErrorxmlFilePath));
    ret = m_pESEXMLInfo->InitXMLInfo();
    QVERIFY(ret);

    ret = false;
    QString tmp = AbsCurrentPath+ "/../../../Main/Build/Settings/EventConfig.xml";
    QStringList EventConfigXmlFilePath(tmp);
    m_pEventXMLInfo = QSharedPointer<EventXMLInfo>(new EventXMLInfo(EventConfigXmlFilePath));
    ret = m_pEventXMLInfo->InitXMLInfo();
    QVERIFY(ret);
}

/****************************************************************************/
void TestErrHandlingMap::init()
{
}

/****************************************************************************/
void TestErrHandlingMap::cleanup()
{
}

/****************************************************************************/
void TestErrHandlingMap::cleanupTestCase()
{

}

} // end namespace EventHandler

QTEST_MAIN(Scheduler::TestErrHandlingMap)

#include "TestErrHandlingMap.moc"
