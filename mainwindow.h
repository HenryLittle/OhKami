#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QList>
#include <QMainWindow>
#include <QColorDialog>
#include "canvasmanager.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void open();
    void save();
    void about();

private:
    void createActions();
    void createMenus();
    bool maybeSave();
    bool saveFile(const QByteArray &fileFormat);

    CanvasManager *canvas;

    QMenu *saveAsMenu;
    QMenu *fileMenu;
    QMenu *optionMenu;
    QMenu *helpMenu;
    QMenu *toolMenu;

    QAction *openAct;
    QList<QAction *> saveAsActs;
    QAction *exitAct;
    QAction *renderAct;
    QAction *clearScreenAct;
    QAction *aboutAct;
    QAction *aboutQtAct;
    QAction *inputModeAct;
    QAction *undoAct;
    // settings
    QAction *penColorAct;
    QAction *brushColorAct;
    QAction *backgroundColorAct;
    QAction *strokeModeAct;
    // stroke type
    QAction *freeAct;
    QAction *rectAct;
    QAction *ellipseAct;
    QAction *diamondAct;
    QAction *lineAct;
    QAction *eraseAct;
};

#endif
