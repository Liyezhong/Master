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
    EventTransition(const TPTransition_t& transition, IState* m_pSource, IState* pTarget);

protected:
    bool eventTest(QEvent *e) Q_DECL_OVERRIDE;

    void onTransition(QEvent* e) override;

private:

    TPTransition_t m_value;
    IState* m_pSource;
    IState* m_pTarget;
};
}

#endif // TRANSITIONTYPEENUM_H
