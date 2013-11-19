#include "scripteventbase.h"

class ContinuousEvent : public AbstractDelayedScriptEvent
{
    DECLARE_CLASS(ContinuousEvent, AbstractDelayedScriptEvent);

    enum DistributionMode_e
    {
        DM_ASCENDING = 0,
        DM_DESCENDING,
        DM_RANDOM,
    };

public:
    ContinuousEvent()
    {
        endTime = 0.0f;
        nextFireTime = 0.0f;

        intervalMin = 0.0f;
        intervalMax = 0.0f;

        countMax = -1;
        spawnCount = 0;
        distributionMode = 0;
    }

    virtual void Init(IGameContext *gameContext, IScriptEvent *root, IScriptEvent *parent)
    {
        BaseClass::Init(gameContext, root, parent);

        if (endTime > 0.0f)
        {
            endTime += GetStartTime();
        }

        RandomNextFireTime();
        nextFireTime += GetStartTime();
    }

    virtual void ParseFromXML(const QDomElement &root)
    {
        BaseClass::ParseFromXML(root);

        endTime = XMLParseFloat(XMLFirstChildElementNoRecursion(root, "duration"), endTime);
        intervalMin = XMLParseFloat(XMLFirstChildElementNoRecursion(root, "interval_min"), intervalMin);
        intervalMax = XMLParseFloat(XMLFirstChildElementNoRecursion(root, "interval_max"), intervalMax);
        countMax = XMLParseInt(XMLFirstChildElementNoRecursion(root, "count_max"), countMax);
        distributionMode = XMLParseInt(XMLFirstChildElementNoRecursion(root, "distribution_mode"), distributionMode);

        if (distributionMode == DM_RANDOM)
        {
            for (int i = 0; i < countMax; i++)
                spawnedSlots.push_back(i);

            for (int i = 0; i < countMax; i++)
                qSwap(spawnedSlots[qrand(countMax - 1)], spawnedSlots[qrand(countMax - 1)]);
        }
    }

    virtual bool ShouldFire() const
    {
        return false;
    }

    virtual void OnSimulate(float frametime)
    {
        Q_ASSERT(countMax > 0 || endTime > 0.0f);

        if (nextFireTime < GetGameContext()->GetGameTime())
        {
            FireEvent();

            spawnCount++;

            RandomNextFireTime();
        }

        SimulateChildren(frametime);
    }

    virtual void FireEvent()
    {
        FOREACH_QLIST(GetChildren(), IScriptEvent*, e)
        {
            // TODO: refactor, remove hacky cast
            AbstractEntityScriptEvent *eEntityEvent = dynamic_cast< AbstractEntityScriptEvent* >(e);

            if (countMax > 0 && eEntityEvent != NULL)
            {
                int slot = spawnCount;

                switch (distributionMode)
                {
                case DM_DESCENDING:
                    slot = (countMax - 1) - slot;
                    break;
                case DM_RANDOM:
                    slot = spawnedSlots.takeAt(qrand(spawnedSlots.size() - 1));
                    break;
                }

                eEntityEvent->FireEventEvenDistribution(slot, countMax);
            }
            else
                e->FireEvent();
        }
        FOREACH_QLIST_END;
    }

    virtual bool IsComplete() const
    {
        return countMax > 0 && spawnCount >= countMax
                || endTime > 0.0f && GetGameContext()->GetGameTime() > endTime;
    }

private:
    void RandomNextFireTime()
    {
        nextFireTime = GetGameContext()->GetGameTime()
                + qlerp(qfrand(), intervalMin, intervalMax);
    }

    float endTime;
    float nextFireTime;

    float intervalMin;
    float intervalMax;

    int countMax;
    int spawnCount;
    int distributionMode;
    QList<int> spawnedSlots;
};

REGISTER_SCRIPT_EVENT(ContinuousEvent, continuous);

