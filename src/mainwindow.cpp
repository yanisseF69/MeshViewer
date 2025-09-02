#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "openGLWidget.h"
#include <QSurfaceFormat>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    QSurfaceFormat format;
    format.setVersion(3, 3);
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setDepthBufferSize(24);
    QSurfaceFormat::setDefaultFormat(format);
    ui->setupUi(this);

    connect(ui->actionLoad, &QAction::triggered, this, &MainWindow::onActionLoad);
    connect(ui->openGLWidget, &OpenGLWidget::verticesChanged, this, [=](unsigned int count) {
        ui->verticesCount->setText(QString::number(count));
    });
    connect(ui->openGLWidget, &OpenGLWidget::trianglesChanged, this, [=](unsigned int count) {
        ui->trianglesCount->setText(QString::number(count));
    });
    connect(ui->wireframeCheck, &QCheckBox::toggled,
            ui->openGLWidget, &OpenGLWidget::setWireframe);


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onActionLoad() {
    QString filename = QFileDialog::getOpenFileName(
        this,
        tr("Open a mesh file"),
        QString(),
        tr("Mesh file (*.off);;All files (*.*)")
        );

    if (!filename.isEmpty()) {
        ui->openGLWidget->loadMesh(filename.toStdString().c_str());
    }
}
