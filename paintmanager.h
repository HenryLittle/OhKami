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
    void cacheState() {
        penColorCache = qpen.color();
        brushColorCache = qbrush.color();
        smCache = strokeMode;
        stCache = strokeType;
    }
    void restoreState() {
        qpen.setColor(penColorCache);
        qbrush.setColor(brushColorCache);
        strokeMode = smCache;
        strokeType = stCache;
    }


    // paint APIs

    QColor getPenColor() {return qpen.color();}
    QColor getBrushColor() {return qbrush.color();}
    void setPenColor(QColor color) {qpen.setColor(color);}
    void setBrushColor(QColor color) {qbrush.setColor(color);}


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
    // do not use explicitly
    QColor penColorCache;
    QColor brushColorCache;
    StrokeMode smCache;
    StrokeType stCache;
};

#endif // PAINTMANAGER_H
