#include <QtWidgets>

#include "mainwindow.h"
#include "canvas.h"
#include "stroke.h"

MainWindow::MainWindow() {
    canvas = new CanvasManager();
    setCentralWidget(canvas);

    createActions();
    createMenus();

    setWindowTitle(tr("OhKami!"));
    resize(500, 500);
}



void MainWindow::closeEvent(QCloseEvent *event) {
    if (maybeSave()) {
        event->accept();
    } else {
        event->ignore();
    }
}

void MainWindow::open() {
    if (maybeSave()) {
        QString fileName = QFileDialog::getOpenFileName(this,
                                   tr("Open File"), QDir::currentPath());
        if (!fileName.isEmpty())
            canvas->openImage(fileName);
    }
}

void MainWindow::save() {
    QAction *action = qobject_cast<QAction *>(sender());
    QByteArray fileFormat = action->data().toByteArray();
    saveFile(fileFormat);
}

void MainWindow::about() {
    QMessageBox::about(this, tr("About OhKami!"),
            tr("<p>OhKami is paint program that let you paint.</p>"));
}

void MainWindow::createActions() {
    openAct = new QAction(tr("&Open..."), this);
    openAct->setShortcut(tr("Ctrl+O"));
    connect(openAct, &QAction::triggered, this, &MainWindow::open);

    const QList<QByteArray> imageFormats = QImageWriter::supportedImageFormats();
    for (const QByteArray &format : imageFormats) {
        QString text = tr("%1...").arg(QString(format).toUpper());

        QAction *action = new QAction(text, this);
        action->setData(format);
        connect(action, &QAction::triggered, this, &MainWindow::save);
        saveAsActs.append(action);
    }


    renderAct = new QAction(tr("&Render"), this);
    renderAct->setShortcut(tr("Ctrl+R"));
    connect(renderAct, &QAction::triggered, canvas, &CanvasManager::renderCanvas);

    inputModeAct = new QAction(tr("&ChangeInputMode"), this);
    inputModeAct->setShortcut(tr("Ctrl+I"));
    connect(inputModeAct, &QAction::triggered, canvas, &CanvasManager::setInputMode);

    undoAct = new QAction(tr("&Undo"), this);
    undoAct->setShortcut(tr("Ctrl+Z"));
    connect(undoAct, &QAction::triggered, canvas, &CanvasManager::undo);


    freeAct = new QAction(tr("&Brush"), this);
    freeAct->setShortcut(tr("Ctrl+1"));
    connect(freeAct, &QAction::triggered, canvas, [=](){canvas->setType(ST_FREE);});
    rectAct = new QAction(tr("&Rectangle"), this);
    rectAct->setShortcut(tr("Ctrl+2"));
    connect(rectAct, &QAction::triggered, canvas, [=](){canvas->setType(ST_RECT);});
    ellipseAct = new QAction(tr("&Ellipse"), this);
    ellipseAct->setShortcut(tr("Ctrl+3"));
    connect(ellipseAct, &QAction::triggered, canvas, [=](){canvas->setType(ST_ELLIPS);});
    diamondAct = new QAction(tr("&Diamond"), this);
    diamondAct->setShortcut(tr("Ctrl+4"));
    connect(diamondAct, &QAction::triggered, canvas, [=](){canvas->setType(ST_DIAMOND);});
    lineAct = new QAction(tr("&Line"), this);
    lineAct->setShortcut(tr("Ctrl+5"));
    connect(lineAct, &QAction::triggered, canvas, [=](){canvas->setType(ST_LINE);});
    eraseAct = new QAction(tr("&Eraser"), this);
    eraseAct->setShortcut(tr("Ctrl+6"));
    connect(eraseAct, &QAction::triggered, canvas, [=](){canvas->setType(ST_ERASE);});


    exitAct = new QAction(tr("E&xit"), this);
    exitAct->setShortcut(tr("Ctrl+Q"));
    connect(exitAct, &QAction::triggered, this, &QWidget::close);

    clearScreenAct = new QAction(tr("&Clear Screen"), this);
    clearScreenAct->setShortcut(tr("Ctrl+L"));
    connect(clearScreenAct, &QAction::triggered,
            canvas, &CanvasManager::clearImage);

    aboutAct = new QAction(tr("&About"), this);
    connect(aboutAct, &QAction::triggered, this, &MainWindow::about);

    aboutQtAct = new QAction(tr("About &Qt"), this);
    connect(aboutQtAct, &QAction::triggered, qApp, &QApplication::aboutQt);
}

void MainWindow::createMenus() {
    saveAsMenu = new QMenu(tr("&Save As"), this);
    saveAsMenu->addActions(saveAsActs);

    fileMenu = new QMenu(tr("&File"), this);
    fileMenu->addAction(openAct);
    fileMenu->addMenu(saveAsMenu);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);


    optionMenu = new QMenu(tr("&Options"), this);
    optionMenu->addAction(clearScreenAct);
    optionMenu->addAction(renderAct);
    optionMenu->addAction(inputModeAct);
    optionMenu->addAction(undoAct);
    optionMenu->addSeparator();
    optionMenu->addAction(freeAct);
    optionMenu->addAction(rectAct);
    optionMenu->addAction(ellipseAct);
    optionMenu->addAction(diamondAct);
    optionMenu->addAction(lineAct);
    optionMenu->addAction(eraseAct);


    helpMenu = new QMenu(tr("&Help"), this);
    helpMenu->addAction(aboutAct);
    helpMenu->addAction(aboutQtAct);

    menuBar()->addMenu(fileMenu);
    menuBar()->addMenu(optionMenu);
    menuBar()->addMenu(helpMenu);
}

bool MainWindow::maybeSave() {
    if (canvas->isModified()) {
       QMessageBox::StandardButton ret;
       ret = QMessageBox::warning(this, tr("Scribble"),
                          tr("The image has been modified.\n"
                             "Do you want to save your changes?"),
                          QMessageBox::Save | QMessageBox::Discard
                          | QMessageBox::Cancel);
        if (ret == QMessageBox::Save) {
            return saveFile("png");
        } else if (ret == QMessageBox::Cancel) {
            return false;
        }
    }
    return true;
}

bool MainWindow::saveFile(const QByteArray &fileFormat) {
    QString initialPath = QDir::currentPath() + "/untitled." + fileFormat;

    QString fileName = QFileDialog::getSaveFileName(this, tr("Save As"),
                               initialPath,
                               tr("%1 Files (*.%2);;All Files (*)")
                               .arg(QString::fromLatin1(fileFormat.toUpper()))
                               .arg(QString::fromLatin1(fileFormat)));
    if (fileName.isEmpty()) {
        return false;
    } else {
        return canvas->saveImage(fileName, fileFormat.constData());
    }
}
