#include "graphicwindow.h"

GraphicWindow::GraphicWindow(Service& service, const std::string& cuisine, QWidget* parent)
    : QWidget(parent), service(service), cuisine(cuisine) {
    setWindowTitle(QString::fromStdString("Graphical View - " + cuisine));
    resize(600, 400);
    setAttribute(Qt::WA_DeleteOnClose);
    service.getRepo().attach(this); // Subscribe to data
}

GraphicWindow::~GraphicWindow() {
    service.getRepo().detach(this); // Unsubscribe to prevent crashes
}

void GraphicWindow::update() {
    this->repaint();
}

void GraphicWindow::paintEvent(QPaintEvent* event) {
    QPainter painter(this);
    auto recipes = service.getRepo().getRecipes();
    
    int x = 20, y = 40;
    for (const auto& r : recipes) {
        if (r.getCuisine() != cuisine) continue;

        int minutes = r.getPrepTime();
        int size = minutes;
        
        if (size < 20) size = 20;
        if (size > 150) size = 150;
        
        painter.setPen(Qt::white); // Make text readable on dark background
        painter.drawText(x, y - 5, QString::fromStdString(r.getName()));
        
        painter.setPen(Qt::black);
        if (minutes <= 100) painter.setBrush(Qt::blue);
        else painter.setBrush(Qt::red);

        // Parity check for shapes
        if (r.getName().length() % 2 == 0) {
            painter.drawRect(x, y, size, size);
        } else {
            painter.drawEllipse(x, y, size, size);
        }
        
        x += size + 20; 
        if (x > this->width() - 100) { x = 20; y += 180; } // Wrap to new line
    }
}
