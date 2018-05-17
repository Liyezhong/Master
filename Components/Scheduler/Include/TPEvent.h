#ifndef TPEVENT_H
#define TPEVENT_H

#include <QSharedPointer>
#include <QDateTime>
#include <QStateMachine>
#include "Global/Include/Commands/Command.h"
#include "Scheduler/Commands/Include/CmdSchedulerCommandBase.h"

namespace Scheduler{
class IEventHandler;

typedef enum
{
    Unknown,
    Done,
    Self,
    Error,
    Timeout,
    Load,
    Start
}TPTransition_t;

class TPInternalEventBase: public QObject
{
    Q_OBJECT
public:
    TPInternalEventBase(const QString& sender)
          :m_Sender(sender),
          m_Handled(false)
    {
        m_Timestamp = QDateTime::currentDateTime();
        m_SeqNo++;
    }

//    TPEventType Type() const {return m_Type;}

    QString Sender() const {return m_Sender;}

    void SetHandled() {m_Handled = true;}

    virtual QString toString()
    {
        return QString("Type: %1, Sender: %2, Timestamp: %3, SeqNo: %4").arg(m_Sender).arg(m_Timestamp.toString()).arg(m_SeqNo);
    }

    Q_DISABLE_COPY(TPInternalEventBase);
protected:
//    TPEventType m_Type;
    QString m_Sender;
    bool m_Handled;
    QDateTime m_Timestamp;
    static quint32 m_SeqNo;
};

struct TPSMEvent: public QEvent
{
    TPSMEvent(const TPTransition_t& val, TPInternalEventBase* pData = nullptr)
        : QEvent((QEvent::Type)(QEvent::User + 1)),
          value(val),
          m_pData(pData)
    {}

    TPTransition_t value;
    TPInternalEventBase* m_pData;

    QString toString(){return QString("TPSMEvent- type{%1}transition{%2}").arg(QEvent::type()).arg(value);}
};



template <class T>
class TPCmdEvent: public TPInternalEventBase
{
public:
    TPCmdEvent(const QString& sender, T cmd)
        :TPInternalEventBase(sender)
    {
        m_Data = cmd;
    }

    T Data() {return m_Data;}

    static TPSMEvent* CreateEvent(Scheduler::SchedulerCommandShPtr_t cmd)
    {
        auto object = new TPCmdEvent<Scheduler::SchedulerCommandShPtr_t>(cmd->GetSender(), cmd);
        return new TPSMEvent(TPTransition_t::Self, object);
    }

    static TPSMEvent* CreateEvent(const QString& retortId, Global::CommandShPtr_t cmd)
    {
        auto object = new TPCmdEvent<Global::CommandShPtr_t>(retortId, cmd);
        return new TPSMEvent(TPTransition_t::Self, object);
    }

    Q_DISABLE_COPY(TPCmdEvent)

private:
    T m_Data;
    QEvent* event;
};

}

#endif // TPEVENT_H
