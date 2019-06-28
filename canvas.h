#ifndef CANVAS_H
#define CANVAS_H
#include <QColor>
#include <QImage>
#include <QPoint>
#include <QWidget>
#include <iostream>



class ScribbleArea : public QWidget
{
    Q_OBJECT // required for classes that need signals and slots

public:
    ScribbleArea(QWidget *parent = nullptr);
    bool openImage(const QString &fileName);
    bool saveImage(const QString &fileName, const char *fileFormat);
    bool isModified() const { return modified; }

public slots:
    void clearImage();
    void print();

protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    bool event(QEvent *event) override;

private:
    void resizeImage(QImage *image, const QSize &newSize);

    bool modified;
    QList<QColor> myPenColors;
    QImage image;
};


#endif // CANVAS_H
