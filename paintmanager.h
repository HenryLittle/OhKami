#ifndef PAINTMANAGER_H
#define PAINTMANAGER_H
#include <iostream>
#include <QPainter>
#include <QWidget>
#include <QtWidgets>
#include "config.h"
#include "layer.h"

enum PaintMode{
    FILL,
    OUTLINE,
    FILL_OUTLINE
};

// provide basic drawing APIs
// handles QBrush & QPen & mbrush
class PaintManager
{
public:
    PaintManager(QImage *image, PaintMode paintMode = FILL);
    void setPaintMode(PaintMode paintMode);
    // paint APIs
    QRectF paintTablet(const QTabletEvent &tablet);
    void paintTouch(QRectF rect);
    void paintStroke(const QRectF &rect);

private:
    QImage *image;
    PaintMode paintMode;
    QPainter *painter;
    QPen qpen;
    QBrush qbrush;
    qreal brushSize;
    void updatePainterSetting();
};

#endif // PAINTMANAGER_H
