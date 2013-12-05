#include "scripteventbase.h"

class InstantaneousEvent : public AbstractDelayedScriptEvent
{
    DECLARE_CLASS(InstantaneousEvent, AbstractDelayedScriptEvent);

public:
    InstantaneousEvent()
    {
        countMin = 1;
        countMax = 1;
        evenDistribution = false;
    }

    virtual void ParseFromXML(const QDomElement &root)
    {
        BaseClass::ParseFromXML(root);

        countMin = XMLParseInt(XMLFirstChildElementNoRecursion(root, "count_min"), countMin);
        countMax = XMLParseInt(XMLFirstChildElementNoRecursion(root, "count_max"), countMax);
        evenDistribution = XMLParseBoolean(XMLFirstChildElementNoRecursion(root, "even_distribution"), evenDistribution);

        countMax = MAX(countMin, countMax);
    }

    virtual bool ShouldFire() const
    {
        return BaseClass::ShouldFire();
    }

    virtual void OnSimulate(float frametime)
    {
        if (ShouldFire())
        {
            FireEvent();
        }

        SimulateChildren(frametime);
    }

    virtual void FireEvent()
    {
        BaseClass::FireEvent();

        int count = qrand(countMax - countMin) + countMin;

        for (int i = 0; i < count; i++)
        {
            FOREACH_QLIST(GetChildren(), IScriptEvent*, e)
            {
                if (e->ShouldFire())
                {
                    // TODO: refactor, remove hacky cast
                    AbstractEntityScriptEvent *eEntityEvent = dynamic_cast< AbstractEntityScriptEvent* >(e);

                    if (eEntityEvent != nullptr)
                        eEntityEvent->FireEventEvenDistribution(i, count);
                    else
                        e->FireEvent();
                }
            }
            FOREACH_QLIST_END;
        }
    }

    virtual bool IsComplete() const
    {
        return BaseClass::IsComplete();
    }

private:
    int countMin;
    int countMax;

    bool evenDistribution;
};

REGISTER_SCRIPT_EVENT(InstantaneousEvent, instantaneous);


