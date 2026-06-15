#pragma once
#include <memory>
#include "domain.h"
#include "service.h"

class Action {
public:
    virtual ~Action() = default;
    virtual void executeUndo() = 0;
    virtual void executeRedo() = 0;
};

class AddAction : public Action {
public:
    AddAction(Service& svc, const Tutorial& t) : svc(svc), tutorial(t) {}
    void executeUndo() override {
        svc.remove(tutorial.getTitle(), tutorial.getPresenter());
    }
    void executeRedo() override {
        svc.add(tutorial.getTitle(), tutorial.getPresenter(),
            tutorial.getDuration().getMinutes(), tutorial.getDuration().getSeconds(),
            tutorial.getLikes(), tutorial.getLink());
    }
private:
    Service& svc;
    Tutorial tutorial;
};

class RemoveAction : public Action {
public:
    RemoveAction(Service& svc, const Tutorial& t, int originalIndex)
        : svc(svc), tutorial(t), originalIndex(originalIndex) {}
    void executeUndo() override {
        svc.insertAt(originalIndex, tutorial);
    }
    void executeRedo() override {
        svc.remove(tutorial.getTitle(), tutorial.getPresenter());
    }
private:
    Service& svc;
    Tutorial tutorial;
    int originalIndex;
};

class UpdateAction : public Action {
public:
    UpdateAction(Service& svc, const Tutorial& before, const Tutorial& after)
        : svc(svc), before(before), after(after) {}
    void executeUndo() override {
        svc.update(after.getTitle(), after.getPresenter(),
            before.getTitle(), before.getPresenter(),
            before.getDuration().getMinutes(), before.getDuration().getSeconds(),
            before.getLink());
    }
    void executeRedo() override {
        svc.update(before.getTitle(), before.getPresenter(),
            after.getTitle(), after.getPresenter(),
            after.getDuration().getMinutes(), after.getDuration().getSeconds(),
            after.getLink());
    }
private:
    Service& svc;
    Tutorial before;
    Tutorial after;
};
