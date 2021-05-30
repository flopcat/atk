#include <iostream>
#include <algorithm>
#include <atk/utf8.h>
#include <atk/aapplication.h>
#include <windows.h>

#define WM_IDLE_PROCESS (WM_APP+1)

AApplication *aApp = NULL;

AApplication::AApplication(int argc, const wchar_t *argv[])
    : AObject(NULL)
{
    programName_ = narrow(std::wstring(argv[0]));
    for (int i = 1; i < argc; i++)
        args_.push_back(narrow(argv[i]));
    handle_ = GetModuleHandle(0);
    defaultShowMethod_ = SW_SHOWNORMAL;
    init();
}

AApplication::AApplication(int argc, const char *argv[])
    : AObject(NULL)
{
    programName_ = argv[0];
    args_.assign(argv + 1, argv + argc);
    handle_ = GetModuleHandle(0);
    defaultShowMethod_ = SW_SHOWNORMAL;
    init();
}

AApplication::AApplication(HINSTANCE hInstance, int nCmdShow)
    : AObject(NULL)
{
    //FIXME: parse program arguments
    handle_ = hInstance;
    defaultShowMethod_ = nCmdShow;
    init();
}

int AApplication::run()
{
    //FIXME: each thread should have its own message pump
    MSG msg;
    processPendingFunctions();
    while (GetMessage(&msg, NULL, 0, 0)) {
        if (msg.message == WM_IDLE_PROCESS) {
            std::cout << "processing pending functions" << std::endl;
            processPendingFunctions();
        } else {
            //std::cout << "dispatching message" << std::endl;
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        //std::cout << "waiting for message" << std::endl;
    }
    return msg.wParam;
}

void AApplication::quit()
{
    std::cout << "quit called" << std::endl;
    aboutToQuit->send();
    PostQuitMessage(0);
}

void AApplication::addPendingFunction(ASlot<> *func)
{
    if (nextPendingIndex == 0)
        PostMessage(0, WM_IDLE_PROCESS, 0, 0);

    AObject *funcOwner = func->owner();
    pendingFunctions.insert({nextPendingIndex, func});
    pendingIndexByOwner.insert({funcOwner, nextPendingIndex});
    if (funcOwner && destroyNotices.count(funcOwner) == 0) {
        // add a destroy notice that will remove all its pending functions
        ASlot<> *notice = new ASlot<>(funcOwner, [funcOwner, this]() {
            typedef std::multimap<AObject*, int> IndexByOwner;
            std::pair<IndexByOwner::const_iterator, IndexByOwner::const_iterator> toDelete = pendingIndexByOwner.equal_range(funcOwner);
            while (toDelete.first != toDelete.second) {
                delete pendingFunctions.at(toDelete.first->second);
                pendingFunctions.erase(toDelete.first->second);
            }
            pendingIndexByOwner.erase(funcOwner);
            destroyNotices.erase(funcOwner);
        });
        funcOwner->aboutToDelete->connect(notice);
        destroyNotices.insert({funcOwner, notice});
    }
    nextPendingIndex++;
}

void AApplication::init()
{
    if (!aApp)
        aApp = this;
    setObjectName("Application Object");

    shouldQuit = false;
    aboutToQuit = new ASignal<>(this);
    nextPendingIndex = 0;
}

bool AApplication::processPendingFunctions()
{
    if (pendingFunctions.empty())
        return false;

    std::cout << "processing pending functions" << std::endl;
    
    // Actually, a peculiar thing may happen here if calling a
    // pendingFunction results in the deletion of the supposed
    // function, or a different pending function.
    // To prevent pendingIndexByOwner from ever losing sync with
    // pendingFunctions, we clear them *after* every function
    // has been invoked... at a time when the arrays will not be
    // modified by the application.
    // Beware! This only works in a single-thread environment.
    for (auto it = pendingFunctions.begin(); it != pendingFunctions.end(); it++)
        it->second->operator ()();
    for (auto it = pendingFunctions.begin(); it != pendingFunctions.end(); it++)
        delete it->second;
    pendingFunctions.clear();
    pendingIndexByOwner.clear();
    // Now that all functions have been called, we can purge all
    // deletion notices for any parent objects of our pending
    // functions.
    for (auto it = destroyNotices.begin(); it != destroyNotices.end(); it++)
        delete it->second;
    destroyNotices.clear();
    nextPendingIndex = 0;
    return true;
}
