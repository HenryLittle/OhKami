#include <QtWidgets>
#include "canvasmanager.h"

CanvasManager::CanvasManager(QWidget *parent)
     : QWidget(parent) {
    // basic initialization
    setAttribute(Qt::WA_AcceptTouchEvents);
    setAttribute(Qt::WA_StaticContents);
    this->paint = new PaintManager(&image);
    layers.append(Layer());
    currentLayer = 0;
    strokeBegin = false;
    strokeEnd = false;
    backgroundColor = QColor("white");
}

bool CanvasManager::openImage(const QString &filename) {
    QImage loadedImage;
    if (!loadedImage.load(filename))
        return false;

    QSize newSize = loadedImage.size().expandedTo(size());
    resizeImage(&loadedImage, newSize);
    image = loadedImage;
    modified = false;
    update();
    return true;
}

bool CanvasManager::saveImage(const QString &filename, const char *fileFormat) {
    QImage visibleImage = image;
    resizeImage(&visibleImage, size());

    if (visibleImage.save(filename, fileFormat)) {
        modified = false;
        return true;
    } else {
        return false;
    }
}

void CanvasManager::clearImage() {
    // clear the whole image
    image.fill(backgroundColor);
    modified = true;
    update();
}



void CanvasManager::resizeImage(QImage *image, const QSize &newSize)
{
    std::cout<<"size "<<newSize.width()<<" "<<newSize.height()<<std::endl;
    if (image->size() == newSize)
        return;
    std::cout<<"Resizing"<<std::endl;
    QImage newImage(newSize, QImage::Format_RGB32);
    newImage.fill(qRgb(255, 255, 255));
    QPainter painter(&newImage);
    painter.drawImage(QPoint(0, 0), *image);
    *image = newImage;
}


void CanvasManager::updateArea(QRectF rect) {
    modified = true;
    int rad = 2;
    update(rect.toRect().adjusted(-rad,-rad, +rad, +rad));
}

void CanvasManager::renderCanvas() {
    if (layers.empty()) {
        image.fill(backgroundColor);
    } else {
        for (int i = 0; i < layers.length(); i++) {
            const Layer &la = layers.at(i);
            if (!la.data.empty()) {
                for (int j = 0; j < la.data.length(); j++) {
                    const Stroke &st = la.data.at(j);
                    renderStroke(st);
                }
            }
        }
    }
    // render to screen
    update();
}

void CanvasManager::renderStroke(const Stroke &stroke) {
    for (int i = 0; i < stroke.data.length(); i++) {
        paint->paintStroke(stroke.data.at(i));
    }
}

/* Overrided functions */
void CanvasManager::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    const QRect rect = event->rect();
    painter.drawImage(rect.topLeft(), image, rect);
}



void CanvasManager::resizeEvent(QResizeEvent *event)
{
    if (width() > image.width() || height() > image.height()) {
        int newWidth = qMax(width() + 128, image.width());
        int newHeight = qMax(height() + 128, image.height());
        resizeImage(&image, QSize(newWidth, newHeight));
        update();
    }
    QWidget::resizeEvent(event);
}

bool CanvasManager::event(QEvent *event) {
    // handle the events -> strokes
    switch (event->type()) {
    case QEvent::TouchBegin:
    case QEvent::TouchUpdate:
    case QEvent::TouchEnd: {
        const QTouchEvent *touch = static_cast<QTouchEvent *>(event);
        const QList<QTouchEvent::TouchPoint> touchPoints = static_cast<QTouchEvent *>(event)->touchPoints();
        for (const QTouchEvent::TouchPoint &touchPoint : touchPoints) {
            switch (touchPoint.state()) {
            case Qt::TouchPointStationary:
            case Qt::TouchPointReleased:
                // don't do anything if this touch point hasn't moved or has been released
                continue;
            default:
                {
                    QRectF rect = touchPoint.rect();
                    if (rect.isEmpty()) {
                        qreal diameter = MAX_BRUSH_DIAMETER;
                        if (touch->device()->capabilities() & QTouchDevice::Pressure)
                            diameter = MAX_BRUSH_DIAMETER + (MAX_BRUSH_DIAMETER - MAX_BRUSH_DIAMETER) * touchPoint.pressure();
                        rect.setSize(QSizeF(diameter, diameter));
                    }
                    //paint->paintTouch(rect);
                    //updateArea(rect);
                    tempStroke.append(rect);
                }
                break;
            }
        }
        layers[currentLayer].data.append(tempStroke);
        tempStroke.clear();
        break;
    }
        case QEvent::TabletPress:
        case QEvent::TabletMove:
        case QEvent::TabletRelease: {
            const QTabletEvent *tablet = static_cast<QTabletEvent* >(event);
            //std::cout<<"Tablet Pen Pressure "<<tablet->pressure()<<std::endl;
            switch (tablet->pointerType()) {
            case QTabletEvent::Pen:{
                QRectF rect;
                if (tablet->pressure() > 0.0) {
                    rect = paint->paintTablet(*tablet);
                    updateArea(rect);

                }
                if (event->type() == QEvent::TabletPress && !strokeBegin) {
                    std::cout<<"Stroke Begin"<<std::endl;
                    strokeBegin = true;
                    strokeEnd = false;
                    tempStroke.append(rect);
                } else if (event->type() == QEvent::TabletRelease && strokeBegin) {
                    std::cout<<"Stroke End"<<std::endl;
                    strokeBegin = false;
                    strokeEnd = true;
                } else if (!strokeEnd && strokeBegin) {
                    tempStroke.append(rect);
                }
            }
            case QTabletEvent::Eraser:{
                break;
            }
            default:
                    break;
            }

            break;
        }
        default:
            return QWidget::event(event);
        }
    if (strokeEnd) {
        layers[currentLayer].addStroke(tempStroke);
        tempStroke.clear();
        strokeEnd = false; strokeBegin = false;
    }
    return true;
}