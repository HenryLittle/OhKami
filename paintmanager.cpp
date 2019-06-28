#include <QtWidgets>

#include "paintmanager.h"

PaintManager::PaintManager(QImage *image, StrokeMode paintMode) {
    this->painter = new QPainter();
    this->strokeMode = paintMode;
    this->image = image;
    // for test only
    setPaintMode(STM_FILL);
    this->strokeType = ST_FREE;
    this->qbrush = QBrush(QColor("grey"));
    this->brushSize = 150.0;
}

void PaintManager::setPaintMode(StrokeMode paintMode) {
    this->strokeMode = paintMode;
    updatePainterSetting();
}

void PaintManager::updatePainterSetting() {
    switch(strokeMode) {
        case STM_FILL:
            painter->setPen(Qt::NoPen);
            painter->setBrush(qbrush);
            break;
        case STM_OUTLINE:
            painter->setPen(qpen);
            painter->setBrush(Qt::NoBrush);
            break;
        case STM_FILL_OUTLINE:
            painter->setPen(qpen);
            painter->setBrush(qbrush);
            break;
    }
}

void PaintManager::setBackground(QColor background) {
    painter->setBackground(QBrush(background));
}

// for real time rendering
QRectF PaintManager::paintTablet(const QTabletEvent &tablet) {
    qreal len = (brushSize - 5.0) * tablet.pressure();
    QRectF rect = QRectF(tablet.posF().x() - len / 2, tablet.posF().y() - len / 2, len, len);
    painter->begin(image);
    updatePainterSetting();
    painter->drawEllipse(rect);
    painter->end();
    return rect;
}

QRectF PaintManager::eraseTablet(const QTabletEvent &tablet, QColor backgroundColor) {
    qreal len = (brushSize - 5.0) * tablet.pressure();
    QRectF rect = QRectF(tablet.posF().x() - len / 2, tablet.posF().y() - len / 2, len, len);
    painter->begin(image);
    QBrush tempBrush = qbrush;
    qbrush = QBrush(backgroundColor);
    updatePainterSetting();
    painter->drawEllipse(rect);
    painter->end();
    qbrush = tempBrush;
    return rect;
}

void PaintManager::paintTouch(QRectF rect) {
    painter->begin(image);
    painter->setPen(Qt::NoPen);
    updatePainterSetting();
    painter->drawEllipse(rect);
    painter->end();
}

// for render the whole canvas
void PaintManager::paintEllipse(const QRectF &rect) {
    painter->begin(image);
    updatePainterSetting();
    painter->drawEllipse(rect);
    painter->end();
}

void PaintManager::eraseEllipse(const QRectF &rect) {
    painter->begin(image);
    QBrush tempBrush = qbrush;
    qbrush = painter->background();
    updatePainterSetting();
    painter->drawEllipse(rect);
    painter->end();
    qbrush = tempBrush;
    painter->end();
}

Stroke PaintManager::initStroke() {
    Stroke stroke = Stroke();
    stroke.fillColor = qbrush.color();
    stroke.outlineColor = qpen.color();
    stroke.mode = strokeMode;
    stroke.type = strokeType;
    return stroke;
}
