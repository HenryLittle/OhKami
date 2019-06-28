#ifndef STROKE_H
#define STROKE_H
#include <QWidget>
#include <QtWidgets>

class Stroke
{
public:
    Stroke(QVector<QRectF> stroke) {
        data = stroke;
    }
    QVector<QRectF> data;
};

#endif // STROKE_H
