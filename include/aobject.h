#ifndef AOBJECT_H
#define AOBJECT_H

#include <iostream>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <functional>

class AObject;
class AThing;

template <typename... Args>
class ASignal;

template <typename... Args>
class ASlot;

class AObject
{
public:
    typedef std::unordered_set<AObject *> AObjectSet;
    typedef std::unordered_set<AThing *> AThingSet;

    AObject(AObject *owner);
    virtual ~AObject();

    void checkCanary();

    void bereave();
    void orphan();
    void giveThing(AThing *thing);

    AObject* parent() const;
    std::string objectName() const;

    void setObjectName(const std::string &name);
    void setParent(AObject *l);

    ASignal<> *aboutToDelete;
    ASignal<AObject*> *aboutToDeleteObject;
    ASignal<> *deleted;
    ASignal<AObject*> *objectDeleted;

    void takeThing(AThing *thing);
private:
    int canary;
    AObject* parent_;
    AObjectSet children;
    AThingSet things;
    std::string objectName_;
    std::string className_;

};

class AThing
{
    // AThing -- represents items that belong to an object, but
    // have no need of signals related to their construction and
    // destruction, such as properties, signals, slots or so.
    // Things should only ever destroyed by the AObject destructor.
public:
    explicit AThing(AObject *owner);

    AObject *owner() const { return owner_; }
    virtual ~AThing();

    void checkCanary();

private:
    int canary;
    AObject *owner_;
    friend AObject;
};


template <typename... Args>
class ASignal : public AThing {
public:
    typedef std::map<int, ASlot<Args...>*> SlotMap;

    explicit ASignal(AObject *owner) : AThing(owner), slots(), nextSlot(0) {}
    virtual ~ASignal() {
        disconnectAll();
    }

    void send(Args... args) {
        if (slots.empty())
            return;
        for (auto &slotPair : slots)
            slotPair.second->operator ()(args...);
    }

    void connect(ASlot<Args...> *slot) {
        std::cout << "adding connection" << std::endl;
        int slotIndex = nextSlot++;
        slots.insert({slotIndex, slot});
        slot->attach(this, slotIndex);
    }

    void disconnect(ASlot<Args...> *slot) {
        if (slots.erase(slot->slotId(this)) > 0)
            slot->detach(this);
    }

    void disconnectAll() {
        for (auto &slotPair : slots)
            slotPair.second->detach(this);
        slots.clear();
    }

private:
    SlotMap slots;
    int nextSlot;
};

template <typename... Args>
class ASlot : public AThing {
public:
    explicit ASlot(AObject *context, std::function<void(Args...)> func) : AThing(context), callback(func) {}

    template<class T>
    ASlot(T *context, void (T::*func)(Args...)) : AThing(context) {
        callback = [=](Args... args) { (context->*func)(args...); };
    }

    virtual ~ASlot() {
        checkCanary();
        std::cout << connections.size();
        if (connections.empty())
            return;
        while (!connections.empty())
            connections.begin()->first->disconnect(this);
        connections.clear();
    }

    void operator ()(Args... args) {
        callback(args...);
    }

protected:
    void attach(ASignal<Args...> *signal, int slotIndex) {
        connections.insert({signal, slotIndex});
    }

    void detach(ASignal<Args...> *signal) {
        std::cout << "erasing connection" << std::endl;
        connections.erase(signal);
    }

    int slotId(ASignal<Args...> *signal) const {
        auto where = connections.find(signal);
        if (where == connections.end())
            return -1;
        return where->second;
    }

private:
    std::function<void(Args...)> callback;
    std::map<ASignal<Args...>*, int> connections;

    friend ASignal<Args...>;
};

#endif // AOBJECT_H
