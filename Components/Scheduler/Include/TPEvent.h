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
    Initial,
    Done,
    Self,
    Error,
    Timeout,
    Load,
    Start,
    Execute,
    ActionDone,
    SoakingDone
}TPTransition_t;

class TPEventArgsBase: public QObject
{
    Q_OBJECT
public:
    TPEventArgsBase(const QString& sender)
          :m_Sender(sender),
          m_Handled(false)
    {
        m_Timestamp = QDateTime::currentDateTime();
        m_SeqNo++;
    }

    QString Sender() const {return m_Sender;}

    void SetHandled() {m_Handled = true;}

    bool Handled() const{return m_Handled;}

    virtual QString toString()
    {
        return QString("Sender: %1, Timestamp: %2, SeqNo: %3").arg(m_Sender).arg(m_Timestamp.toString()).arg(m_SeqNo);
    }

    Q_DISABLE_COPY(TPEventArgsBase);

protected:
    QString m_Sender;
    bool m_Handled;
    QDateTime m_Timestamp;
    static quint32 m_SeqNo;
};

class TPEvent: public QEvent
{
public:
    TPEvent(const TPTransition_t& val, TPEventArgsBase* pData = nullptr)
        : QEvent((QEvent::Type)(QEvent::User + 1)),
          m_Transition(val),
          m_EventArgs(pData)
    {}

    ~TPEvent()
    {
        if(m_EventArgs != nullptr)
        {
            qDebug() << "~TPEvent()";
            delete m_EventArgs;
            m_EventArgs = nullptr;
        }
    }

    const TPTransition_t& Value(){return m_Transition;}
    TPEventArgsBase* EventArgs()  {return m_EventArgs;}
    QString toString(){return QString("TPSMEvent- type{%1}transition{%2}").arg(QEvent::type()).arg(m_Transition);}

private:
    TPTransition_t m_Transition;
    TPEventArgsBase* m_EventArgs;
};



template <class T>
class TPEventArgs: public TPEventArgsBase
{
public:
    TPEventArgs(const QString& sender, T cmd)
        :TPEventArgsBase(sender)
    {
        m_Data = cmd;
    }

    TPEventArgs(const QString& sender, Global::tRefType ref, T cmd)
        :TPEventArgsBase(sender)
    {
        m_Data = cmd;
        m_Ref = ref;
    }

    T Data() {return m_Data;}
    Global::tRefType Ref() {return m_Ref;}

    static TPEvent* CreateEvent(Scheduler::SchedulerCommandShPtr_t cmd)
    {
        auto object = new TPEventArgs<Scheduler::SchedulerCommandShPtr_t>(cmd->GetSender(), cmd);
        return new TPEvent(TPTransition_t::Self, object);
    }

    static TPEvent* CreateEvent(const QString& retortId, Global::tRefType ref, Global::CommandShPtr_t cmd)
    {
        auto object = new TPEventArgs<Global::CommandShPtr_t>(retortId, ref, cmd);
        return new TPEvent(TPTransition_t::Self, object);
    }

    Q_DISABLE_COPY(TPEventArgs)

private:
    T m_Data;
    Global::tRefType m_Ref;
};

}

#endif // TPEVENT_H
