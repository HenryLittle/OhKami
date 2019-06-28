#ifndef PAINTMANAGER_H
#define PAINTMANAGER_H
#include <iostream>
#include <QPainter>
#include <QWidget>
#include <QtWidgets>
#include "config.h"
#include "layer.h"

// provide basic drawing APIs
// handles QBrush & QPen & mbrush
class PaintManager
{
public:
    PaintManager(QImage *image, StrokeMode strokeMode = STM_FILL);
    void setPaintMode(StrokeMode strokeMode);
    Stroke initStroke();
    void setBackground(QColor background);
    // paint APIs
    QRectF paintTablet(const QTabletEvent &tablet);
    QRectF eraseTablet(const QTabletEvent &tablet, QColor backgroundColor);
    void paintTouch(QRectF rect);
    void paintEllipse(const QRectF &rect);
    void eraseEllipse(const QRectF &rect);
private:
    QImage *image;
    StrokeType strokeType;
    StrokeMode strokeMode;
    QPainter *painter;
    QPen qpen;
    QBrush qbrush;
    qreal brushSize;
    void updatePainterSetting();
};

#endif // PAINTMANAGER_H
