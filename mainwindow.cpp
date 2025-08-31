#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "openGLWidget.h"
#include <QSurfaceFormat>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    QSurfaceFormat format;
    format.setVersion(3, 3);
    format.setProfile(QSurfaceFormat::CoreProfile);
    QSurfaceFormat::setDefaultFormat(format);
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}
