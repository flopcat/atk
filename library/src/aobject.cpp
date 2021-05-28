#include <assert.h>
#include <iostream>
#include "atk/aobject.h"

AObject::AObject(AObject *owner)
    : canary(0xfedbeef), parent_(owner), children(), objectName_()
{
    setObjectName("Object");
    aboutToDelete = new ASignal<>(this);
    aboutToDeleteObject = new ASignal<AObject*>(this);
    deleted = new ASignal<>(nullptr);
    objectDeleted = new ASignal<AObject*>(nullptr);

    if (owner == nullptr)
        return;

    owner->children.insert(this);
}

AObject::~AObject()
{
    checkCanary();
    if (canary != 0xfedbeef)
        return;

    aboutToDelete->send();
    aboutToDeleteObject->send(this);

    bereave();
    while (!things.empty())
        delete *things.begin();
    things.clear();

    orphan();
    deleted->send();
    objectDeleted->send(this);
    delete deleted;
    delete objectDeleted;
    canary = 0;
}

void AObject::checkCanary()
{
    assert(canary == 0xfedbeef);
}

void AObject::bereave()
{
    while (!children.empty())
        delete *children.begin();
    children.clear();
}

void AObject::orphan()
{
    if (parent_)
        parent_->children.erase(this);
    parent_ = nullptr;
}

void AObject::giveThing(AThing *thing)
{
    assert(thing);
    if (thing->owner() != this) {
        std::cerr << "Attepted to give possession to foreign object." << std::endl;
        return;
    }
    things.insert(thing);
    std::cout << "added thing to object" << std::endl;
}

void AObject::takeThing(AThing *thing)
{
    assert(thing);
    if (thing->owner() != this) {
        std::cerr << "Attempted to take away possession of unowned object." << std::endl;
        return;
    }
    things.erase(thing);
    std::cout << "took thing from object" << std::endl;
}

AObject *AObject::parent() const { return parent_; }

std::string AObject::objectName() const { return objectName_; }

void AObject::setObjectName(const std::string &name)
{
    objectName_ = name;
}

void AObject::setParent(AObject *l)
{
    if (l == parent_)
        return;

    orphan();
    if (l) {
        parent_ = l;
        parent_->children.insert(this);
    }
}

AThing::AThing(AObject *owner) : owner_(owner), canary(0xfedbeef)
{
    if (owner == NULL)
        return;
    owner->giveThing(this);
}

AThing::~AThing() {
    checkCanary();
    if (owner_ != NULL) {
        owner_->takeThing(this);
    }
    canary = 0;
}

void AThing::checkCanary()
{
    assert(canary == 0xfedbeef);
}
