//
// Pool simulation before reconstruction
//
#include "simlib.h"
#include <vector>
#include <algorithm>

#define GROUP_SIZE 32
#define Norm(a, b) (std::max(0.0, Normal(a, b)))

Store Pool("Pool capacity", 634);
Store Booths("Booths", 8);
Facility Reception("Reception");
Queue boothQueue("Booth queue");

// 6-9 morning groups
int MorningGroups = 2;
// 9-15 no groups
int AfternoonNoGroups = 6;
// 15-20 evening groups
int EveningGroups = 5;
// total people got in
int people_got_in = 0;

Histogram TimeInPool("Time spent in the pool", 0, 600, 20);
Histogram SwimmerInterrupted("Amount of swimmer's interruptions", 0, 1, 10);

/**
 * Visitor class
 */
class Visitor : public Process
{
private:
    double Entry;
    bool isGroup;
    bool skipBooth = false;
    bool isProfesional = false;
    int interrupted = 0;

public:
    Visitor(bool isGroup = false);

    void Behavior() override;

    void setSkipBooth();
    void interrupt();
    void changinRoom(int boothTime, int hallTime);
};

/**
 * SwimManager class
 *
 * It is used to interrupt professional swimmers when an interruptor comes.
 */
class SwimManager
{
private:
    std::vector<Visitor *> swimmerList;

public:
    void addSwimmer(Visitor *swimmer)
    {
        swimmerList.push_back(swimmer);
    }

    bool isSwimmerInList(const Visitor *swimmer) const
    {
        return std::find(swimmerList.begin(), swimmerList.end(), swimmer) != swimmerList.end();
    }

    void removeSwimmer(const Visitor *swimmer)
    {
        auto it = std::remove(swimmerList.begin(), swimmerList.end(), swimmer);
        swimmerList.erase(it, swimmerList.end());
    }

    size_t getSwimmerCount() const
    {
        return swimmerList.size();
    }

    std::vector<Visitor *>::const_iterator begin() const
    {
        return swimmerList.begin();
    }

    std::vector<Visitor *>::const_iterator end() const
    {
        return swimmerList.end();
    }
};

SwimManager swimManager;

/**
 * VisitorWantsBooth class
 *
 * It is used to wait maximum 2 minutes for a booth.
 */
class VisitorWantsBooth : public Process
{
private:
    Visitor *visitor;

public:
    VisitorWantsBooth(Visitor *visitor) : visitor(visitor) {}

    void Behavior()
    {
        Wait(120);
        visitor->setSkipBooth();
        visitor->Activate();
    }
};

Visitor::Visitor(bool isGroup) : isGroup(isGroup) {}

void Visitor::Behavior()
{
    Entry = Time;
    if (!isGroup)
    {
        if (Pool.Free() >= 1)
        {
            Enter(Pool, 1);
            people_got_in++;
        }
        else
        {
            Cancel();
        }

        if (Random() < 0.30)
        {
            Seize(Reception);
            Wait(Exponential(90));
            Release(Reception);
        }
    }

    //////////////////////
    // in changing room //
    //////////////////////

    changinRoom(240, 180);
    skipBooth = false;

    ////////////////
    // near pools //
    ////////////////

    if (isGroup)
    {
        Wait(3600);
    }
    else
    {
        double swimmerType = Random();
        if (swimmerType < 0.04)
        {
            for (const auto swimmer : swimManager)
            {
                swimmer->interrupt();
            }
            goto normal;
        }
        else if (swimmerType < 0.14) // 10%
        {
            isProfesional = true;
            swimManager.addSwimmer(this);
            Wait(Norm(3600, 600));
            swimManager.removeSwimmer(this);
        }
        else
        {
        normal:
            Wait(Norm(2400, 600));
            Wait(Norm(1800, 300));
        }
    }

    //////////////////////
    // in changing room //
    //////////////////////

    Wait(Exponential(300));

    changinRoom(360, 300);

    Leave(Pool, 1);

    TimeInPool(Time - Entry);
    // professional swimmers interruption statistics
    if (isProfesional)
    {
        SwimmerInterrupted(interrupted);
    }
}

void Visitor::setSkipBooth()
{
    skipBooth = true;
}

void Visitor::interrupt()
{
    interrupted++;
}

void Visitor::changinRoom(int boothTime, int hallTime)
{
    if (Random() < 0.40)
    {
        if (Booths.Full()) // no free booth
        {
            Into(boothQueue);
            VisitorWantsBooth *vwb = new VisitorWantsBooth(this);
            vwb->Activate();
            Passivate();
            // visitor was activated before 2 minutes passed
            if (!skipBooth)
            {
                vwb->Cancel();
            }
        }
        if (!skipBooth)
        {
            Enter(Booths, 1);
            Wait(Norm(boothTime, 60));
            Leave(Booths, 1);
            while (boothQueue.Length() > 0)
            {
                Visitor *v = (Visitor *)boothQueue.GetFirst();
                if (!(v->skipBooth))
                { // old ones, which decided to leave the queue, are skipped
                    v->Activate();
                    break;
                }
            }
        }
    }
    else
    {
        skipBooth = true;
    }
    if (skipBooth)
    {
        Wait(Norm(hallTime, 60));
    }
}

/**
 * Group class
 *
 * It is used to generate groups of visitors.
 */
class Group : public Process
{
    void Behavior()
    {
        if (Pool.Free() >= GROUP_SIZE)
        {
            Enter(Pool, GROUP_SIZE);
            for (int i = 0; i < GROUP_SIZE; i++)
            {
                (new Visitor(true))->Activate();
                people_got_in++;
            }
        }
    }
};

/**
 * Hour class
 *
 * It is used to simulate hours in the pool and managing groups.
 */
class Hour : public Process
{
    void Behavior()
    {
        if (MorningGroups > 0)
        {
            MorningGroups--;
            Wait(3600);
            (new Group)->Activate();
            (new Hour)->Activate();
        }
        else if (AfternoonNoGroups > 0)
        {
            AfternoonNoGroups--;
            Wait(3600);
            (new Hour)->Activate();
        }
        else if (EveningGroups > 0)
        {
            EveningGroups--;
            Wait(3600);
            (new Group)->Activate();
            (new Hour)->Activate();
        }
    }
};

/**
 * VisitorGenerator class
 *
 * It is used to generate visitors (not in groups).
 */
class VisitorGenerator : public Event
{
    void Behavior()
    {
        (new Visitor(false))->Activate();
        double d = Exponential(60);
        Activate(Time + d);
    }
};

int main()
{
    SetOutput("pool.out");
    Init(0, 54000); // 6-21
    (new VisitorGenerator)->Activate();
    (new Hour)->Activate();
    (new Group)->Activate();
    Run();
    Pool.Output();
    Print("People got in: %d\n\n", people_got_in);
    Booths.Output();
    TimeInPool.Output();
    SwimmerInterrupted.Output();
    Reception.Output();
    return 0;
}
