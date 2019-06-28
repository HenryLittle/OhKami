#ifndef LAYER_H
#define LAYER_H
#include <QWidget>
#include <QtWidgets>
#include "stroke.h"

enum BlendingMode{
    NORMAL
};

class Layer
{
public:
    QVector<Stroke> data;
    bool visible;
    BlendingMode blendingMode;
    void addStroke(QVector<QRectF> stroke) {
        data.append(Stroke(stroke));
    }
};

#endif // LAYER_H
