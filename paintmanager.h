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
    Qt::BrushStyle brushStyle;
    PaintManager(QImage *image, StrokeMode strokeMode = STM_FILL);
    void setStrokeMode(StrokeMode strokeMode);
    Stroke initStroke();
    void setBackground(QColor background);
    void setBrushStyle(Qt::BrushStyle style) {
        brushStyle = style;
    }
    void cacheState() {
        penColorCache = qpen.color();
        brushColorCache = qbrush.color();
        smCache = strokeMode;
        stCache = strokeType;
        btCache = brushStyle;
    }
    void restoreState() {
        qpen.setColor(penColorCache);
        qbrush.setColor(brushColorCache);
        strokeMode = smCache;
        strokeType = stCache;
        brushStyle = btCache;
    }


    // paint APIs

    QColor getPenColor() {return qpen.color();}
    QColor getBrushColor() {return qbrush.color();}
    void setPenColor(QColor color) {qpen.setColor(color);}
    void setBrushColor(QColor color) {qbrush.setColor(color);}
    void setPenWidth(int width){qpen.setWidth((width));}
    void setBrushWidth(int width){brushSize = static_cast<qreal>(width);}
    void setBrush(const QBrush &brush);

    QRectF paintTablet(const QTabletEvent &tablet);
    QRectF eraseTablet(const QTabletEvent &tablet, QColor backgroundColor);
    QRectF paintMouse(const QPointF &pos);
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
    QColor backgroundColor;
    // do not use explicitly
    QColor penColorCache;
    QColor brushColorCache;
    StrokeMode smCache;
    StrokeType stCache;
    Qt::BrushStyle btCache;
};

#endif // PAINTMANAGER_H
