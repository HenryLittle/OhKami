#include <QtWidgets>

#include "paintmanager.h"

PaintManager::PaintManager(QImage *image, PaintMode paintMode) {
    this->painter = new QPainter();
    this->paintMode = paintMode;
    this->image = image;
    // for test only
    setPaintMode(FILL);
    this->qbrush = QBrush(QColor("grey"));
    this->brushSize = 150.0;
}

void PaintManager::setPaintMode(PaintMode paintMode) {
    this->paintMode = paintMode;
    updatePainterSetting();
}

void PaintManager::updatePainterSetting() {
    switch(paintMode) {
        case FILL:
            painter->setPen(Qt::NoPen);
            painter->setBrush(qbrush);
            break;
        case OUTLINE:
            painter->setPen(qpen);
            painter->setBrush(Qt::NoBrush);
            break;
        case FILL_OUTLINE:
            painter->setPen(qpen);
            painter->setBrush(qbrush);
            break;
    }
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

void PaintManager::paintTouch(QRectF rect) {
    painter->begin(image);
    painter->setPen(Qt::NoPen);
    updatePainterSetting();
    painter->drawEllipse(rect);
    painter->end();
}

// for render the whole canvas
void PaintManager::paintStroke(const QRectF &rect) {
    painter->begin(image);
    updatePainterSetting();
    painter->drawEllipse(rect);
    painter->end();
}
