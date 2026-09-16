#pragma once
#include <QWidget>
#include <QPainter>
#include "observer.h"
#include "service.h"

class GraphicWindow : public QWidget, public Observer {
    Q_OBJECT
private:
    Service& service;
    std::string cuisine;
protected:
    void paintEvent(QPaintEvent* event) override;
public:
    GraphicWindow(Service& service, const std::string& cuisine, QWidget* parent = nullptr);
    ~GraphicWindow();
    void update() override;
};
