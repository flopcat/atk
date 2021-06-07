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
    // FIXME: destruction order may be random!
    typedef std::unordered_set<AObject *> AObjectSet;
    typedef std::unordered_set<AThing *> AThingSet;

    // Create an AObject with a specified owner.  `owner` should be set to the
    // parent object, or nullptr if it is without a parent.  For example,
    // objects created on the stack should set their parent to nullptr.
    AObject(AObject *owner);
    // Destroy all things attached to the object, and remove itself from the
    // parent object.  Calls orphan, which will remove itself from the parent
    // object's children container.
    virtual ~AObject();

    // Check that the canary still equals its magic value.  asserts() that it
    // is set.  Canaries are used to avoid double frees in the destructor, and
    // are zeroed on the last line of the destructor.  You can call this at any
    // time.
    void checkCanary();

    // Delete every child and clear the child array.
    void bereave();
    // Orphan this object by removing it from its parent's children set and
    // clearing the parent field.  It could also be called declareIndependence.
    void orphan();

    // Give possession of an AThing to this object, after the AThings's owner
    // field has been assigned to this object, usually in the constructor of
    // AThing itself.  Will complain on cerr if you didn't know what you're
    // doing.
    void giveThing(AThing *thing);
    // Take possession of an AThing away from this object.  Usually called by
    // the destructor of an AThing.  Will complain on cerr if it doesn't own
    // what you're trying to take away.
    void takeThing(AThing *thing);

    // Returns who owns us
    AObject* parent() const;
    // Returns the name associated with this object
    std::string objectName() const;

    // Set the name associated with the object.  Do this in the constructor of
    // your object, or else you will inherit whoever called it last (usually
    // your super class).
    void setObjectName(const std::string &name);
    // Re-parent this object to belong to someone else.  This will temporarily
    // declare itself an orphan before attaching to the new parent.
    void setParent(AObject *l);

    // Fired in the destructor, but before the children are deleted.
    ASignal<> *aboutToDelete;
    // Fired in the destructor, but before the children are deleted.
    ASignal<AObject*> *aboutToDeleteObject;
    // Fired in the destructor, after the children are deleted.
    ASignal<> *deleted;
    // Fired in the destructor, after the children are deleted.
    ASignal<AObject*> *objectDeleted;

private:
    int canary;
    AObject* parent_;
    AObjectSet children;
    AThingSet things;
    std::string objectName_;
    std::string className_;

};

// AThing represents items that belong to an object, but have no need of
// signals related to their construction and destruction, such as properties,
// signals, slots or so forth.  They are meant to be more permanently attached
// to an AObject, compared to other AObjects.  AThing should only ever be
// destroyed by the AObject destructor.
class AThing
{
public:
    // Construct AThing with specified owner.  Associates this AThing with the
    // parent through the parent's giveThing method.
    explicit AThing(AObject *owner);
    // Destroy AThing.  Disassociates from the parent by calling the parent's
    // takeThing method.
    virtual ~AThing();

    // Return the object who owns us.
    AObject *owner() const { return owner_; }

    // Perform a runtime check of the canary
    void checkCanary();

private:
    int canary;
    AObject *owner_;
    friend AObject;
};

// ASignal<event arguments> represents the sender side of an event.  ASignals
// are used as a handy way of triggering a callback to somewhere in the program
// without the sender and receiver needing to know anything about each other.
// ASignals are a 1 to many relationship and can be sent to any number of
// receivers.
template <typename... Args>
class ASignal : public AThing {
public:
    // Map between an incrementing index with each associated slot.  We use a
    // map to preserve insertion order.
    typedef std::map<int, ASlot<Args...>*> SlotMap;

    // Create an ASignal with the specified owner (nullptr if no owner).
    explicit ASignal(AObject *owner) : AThing(owner), slots(), nextSlot(0) {}
    // Destroy the ASignal.  Disconnects from everyone listening to us.
    virtual ~ASignal() {
        disconnectAll();
    }

    // Fire the event with the specified arguments.  Every associated slot is
    // invoked in connection order.
    void send(Args... args) {
        if (slots.empty())
            return;
        for (auto &slotPair : slots)
            slotPair.second->operator ()(args...);
    }

    // Connect a slot to this signal.  Inserts the slot into the next available
    // index in the slot map, and requests that the slot associate on its side
    // as well.
    void connect(ASlot<Args...> *slot) {
        std::cout << "adding connection" << std::endl;
        int slotIndex = nextSlot++;
        slots.insert({slotIndex, slot});
        slot->attach(this, slotIndex);
    }

    // Disconnect a slot from this signal.  Requests that the slot also remove
    // the signal on their end as well.  This means you want to call disconnect
    // on the signal side rather than the slot side.
    void disconnect(ASlot<Args...> *slot) {
        if (slots.erase(slot->slotId(this)) > 0)
            slot->detach(this);
    }

    // Disconnects every slot from this signal.  Asks every associated slot to
    // detach from this signal and clears the slot map.
    void disconnectAll() {
        for (auto &slotPair : slots)
            slotPair.second->detach(this);
        slots.clear();
    }

private:
    SlotMap slots;
    int nextSlot;
};

// ASlot<event arguments> represents the receiver side of an event.  ASlots are
// used as a handy way of triggering a callback to somewhere in the program
// without the sender and receiver needing to know anything about each other.
// ASlots are a many to 1 relationship and can be invoked by any number of
// senders.
template <typename... Args>
class ASlot : public AThing {
public:
    // Map between associated signals to each signal's slot index of this slot.
    // We track the signal's slot index so the signal can quickly erase us.
    typedef std::map<ASignal<Args...>*, int> ConnectionMap;

    // Create an ASlot with a specified context (that is to say, owner) with a
    // specified callback function.  As usual, context can be nullptr if the
    // ASlot is created on the stack.
    explicit ASlot(AObject *context, std::function<void(Args...)> func) : AThing(context), callback(func) {}

    // Create an ASlot with a specified context and a callback to one of the
    // context's member functions.  The specified context must not be nullptr.
    template<class T>
    ASlot(T *context, void (T::*func)(Args...)) : AThing(context) {
        callback = [=](Args... args) { (context->*func)(args...); };
    }

    // Destroys the ASlot.  Asks every associated signal to disconnect from
    // this slot.
    virtual ~ASlot() {
        checkCanary();
        if (connections.empty())
            return;
        while (!connections.empty())
            connections.begin()->first->disconnect(this);
        connections.clear();
    }

    // Invoke the slot's callback with the specified arguments.
    void operator ()(Args... args) {
        callback(args...);
    }

protected:
    // Attach a signal to this slot.  Inserts the signal into the connection
    // map, with the signal's slot index of this slot.
    void attach(ASignal<Args...> *signal, int slotIndex) {
        connections.insert({signal, slotIndex});
    }

    // Detach a signal from this slot.  Removes the signal from the connection
    // map, with the signal's slot index of this slot.
    void detach(ASignal<Args...> *signal) {
        connections.erase(signal);
    }

    // Returns the specified signal's slot index of this slot, or -1 if no such
    // signal was found.
    int slotId(ASignal<Args...> *signal) const {
        auto where = connections.find(signal);
        if (where == connections.end())
            return -1;
        return where->second;
    }

private:
    std::function<void(Args...)> callback;
    ConnectionMap connections;

    friend ASignal<Args...>;
};

#endif // AOBJECT_H
