#ifndef TRANSITIONTYPEENUM_H
#define TRANSITIONTYPEENUM_H
#include <QEvent>
#include <QAbstractTransition>

#include "Scheduler/Include/TPEvent.h"

namespace Scheduler{
const QEvent::Type TPSMEventType = (QEvent::Type)QEvent::registerEventType(QEvent::User + 1);
class IState;



class EventTransition: public QAbstractTransition
{

public:
    EventTransition(const TPTransition_t& transition, QState* m_pSource, QState* pTarget = nullptr);

protected:
    bool eventTest(QEvent *e) Q_DECL_OVERRIDE;

    void onTransition(QEvent* e) override;

private:
    QState* m_pSource;
    QState* m_pTarget;
    TPTransition_t m_value;
};
}

#endif // TRANSITIONTYPEENUM_H
