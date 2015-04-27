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
#include <QXmlStreamReader>
#include <QHash>

using namespace EventHandler;

namespace Scheduler {

typedef struct{
    QString StationID;
    QString ReagentID;
}ReagentStationList_t;

/****************************************************************************/
/**
 * \brief Test class for XmlConfigFile class.
 */
/****************************************************************************/
class TestErrHandlingMap: public QObject {
    Q_OBJECT

private:
    QString GetSafeReagentType(const QString& Scenario);
    bool ReadReagentList(const QString& FileList);

    QSharedPointer<EventScenarioErrXMLInfo> m_pESEXMLInfo;
    QSharedPointer<EventXMLInfo> m_pEventXMLInfo;
    QList<ReagentStationList_t>      m_ReagentStationList;
    void GetSafeReagentStation(const QString& ReagentID, ReagentStationList_t& reagentStation);

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

void TestErrHandlingMap::GetSafeReagentStation(const QString& ReagentID, ReagentStationList_t& reagentStation)
{
    ReagentStationList_t ReagentStation;
    for(int i = 0; i < m_ReagentStationList.size(); ++i)
    {
        ReagentStation = m_ReagentStationList.at(i);
        if ( (ReagentID == "RG1") || (ReagentID == "RG5") || (ReagentID == "RG6") || (ReagentID == "RG3") )
        {
            if(ReagentID == ReagentStation.ReagentID)
            {
                reagentStation = ReagentStation;
                return;
            }
        }
    }
}

bool TestErrHandlingMap::ReadReagentList(const QString& FileList)
{
    m_ReagentStationList.clear();
    QString XMLFile = FileList;
    QSharedPointer<QXmlStreamReader> pXMLReader;
    QFile xmlFile(FileList);
    if (xmlFile.exists())
    {
        if (xmlFile.open(QIODevice::ReadOnly)){
            pXMLReader = QSharedPointer<QXmlStreamReader>(new QXmlStreamReader(xmlFile.readAll()));
        }
        else{
            return false;
        }
    }
    else{
        return false;
    }
    if(pXMLReader == NULL)
    {
        return false;
    }
    if(!pXMLReader->atEnd())
    {
        pXMLReader->readNextStartElement();
        while (pXMLReader->name() != "Reagents" || !pXMLReader->isEndElement())
        {
            pXMLReader->readNextStartElement();
            if (pXMLReader->name() == "Reagent" && !pXMLReader->isEndElement())
            {
                QString StationId = pXMLReader->attributes().value("StationID").toString();
                QString ReagentGroup = pXMLReader->attributes().value("ReagentGroup").toString();
                if(!StationId.isEmpty())
                {
                    ReagentStationList_t reagentStation;
                    reagentStation.StationID = StationId;
                    reagentStation.ReagentID = ReagentGroup;

                    m_ReagentStationList.push_back(reagentStation);
                }
            }
        }
    }

    return true;
}

QString TestErrHandlingMap::GetSafeReagentType(const QString& Scenario)
{
    ReagentStationList_t reagentStation;
    QString SafeReagentType = "";
    quint32 scenario = Scenario.toInt();
    QString ReagentID;

    if(200 == scenario)
    {
        reagentStation = m_ReagentStationList.at(0);//Program_first_position";
    }
    else if(scenario >= 211 && scenario <= 221)
    {
        GetSafeReagentStation("RG1", reagentStation);
    }
    else if(scenario >= 221 && scenario <= 247)
    {
        GetSafeReagentStation("RG3", reagentStation);
    }
    else if(scenario >= 251 && scenario <= 271)
    {
        GetSafeReagentStation("RG5", reagentStation);
    }
    else if(scenario >= 272 && scenario <= 277)
    {
        GetSafeReagentStation("RG6", reagentStation);
    }

    SafeReagentType = reagentStation.StationID + "_" + reagentStation.ReagentID;
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
    stream << "#EventId; ScenarioId; ErrorId; Type; ResponseId; SafeReagentStationGroup;" << "\n";

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
                stream << ";  SafeReagentStationGroup:/;\n";
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
                        stream << ";  SafeReagentStationGroup:" << GetSafeReagentType(Scenario) << ";\n";
                    }
                    else
                    {
                        stream << ";  SafeReagentStationGroup:/;\n";
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

    ret = false;
    QString ReagentFilePath = "../Settings/ReagentStations.xml";
    ret = ReadReagentList(ReagentFilePath);
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
