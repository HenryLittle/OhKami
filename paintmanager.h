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
    StrokeType strokeType;
    StrokeMode strokeMode;
    PaintManager(QImage *image, StrokeMode strokeMode = STM_FILL);
    void setStrokeMode(StrokeMode strokeMode);
    Stroke initStroke();
    void setBackground(QColor background);
    // paint APIs
    QRectF paintTablet(const QTabletEvent &tablet);
    QRectF eraseTablet(const QTabletEvent &tablet, QColor backgroundColor);
    void paintTouch(QRectF rect);
    void paintEllipse(const QRectF &rect);
    void eraseEllipse(const QRectF &rect);
    void paintRect(QRectF rect);
    void eraseRect(QRectF rect);
    void paintLine(QPointF pos1, QPointF pos2);
    void eraseLine(QPointF pos1, QPointF pos2);

private:
    QImage *image;
    QPainter *painter;
    QPen qpen;
    QBrush qbrush;
    qreal brushSize;
    void updatePainterSetting();
};

#endif // PAINTMANAGER_H
