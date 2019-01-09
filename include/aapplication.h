#ifndef AAPPLICATION_H
#define AAPPLICATION_H

#include <aobject.h>
#include <atypes.h>
#include <set>
#include <windows.h>

class AApplication;
extern AApplication *aApp;

class AApplication : public AObject
{
public:
    AApplication(int argc, const wchar_t *argv[]);
    AApplication(int argc, const char *argv[]);
    AApplication(HINSTANCE hInstance, int nCmdShow);

    int run();
    void quit();

    // NOTE: slot will be deleted after it is called, so don't pass
    // it one of your long-lived slots.
    void addPendingFunction(ASlot<>* func);

    const std::string &programName() { return programName_; }
    const AStringList &args() {  return args_; }
    HINSTANCE handle() { return handle_; }
    int defaultShowMethod() { return defaultShowMethod_; }

    ASignal<> *aboutToQuit;

private:
    void init();
    bool processPendingFunctions();

    std::string programName_;
    AStringList args_;
    HINSTANCE handle_;
    int defaultShowMethod_;
    volatile bool shouldQuit;

    std::map<int, ASlot<>*> pendingFunctions;
    std::multimap<AObject*, int> pendingIndexByOwner;
    std::map<AObject*, ASlot<>*> destroyNotices;
    int nextPendingIndex;
};

#endif // AAPPLICATION_H
