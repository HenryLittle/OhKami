#ifndef FILEMANAGER_H
#define FILEMANAGER_H
#include <QDataStream>
#include <QtWidgets>
#include <iostream>
#include <QFile>

#include "config.h"
#include "layer.h"

// save a QVector<Layer> to a file *.kami
class FileManager
{
public:
    FileManager(const QString &filename);
    void saveKami(const QVector<Layer> &layers);
    QVector<Layer> loadKami();
private:
    const quint16 KAMI_HEADER = 0xCAE1; // Kami
    QFile *file;
};

#endif // FILEMANAGER_H
