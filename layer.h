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
};

#endif // LAYER_H
