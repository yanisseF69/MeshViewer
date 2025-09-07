#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "openGLWidget.h"
#include <QSurfaceFormat>
#include <QFileDialog>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    errorTimer = new QTimer();
    QSurfaceFormat format;
    format.setVersion(3, 3);
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setDepthBufferSize(32);
    QSurfaceFormat::setDefaultFormat(format);
    ui->setupUi(this);

    this->setWindowTitle("Mesh Viewer");

    connect(ui->actionLoad, &QAction::triggered, this, &MainWindow::onActionLoad);
    connect(ui->openGLWidget, &OpenGLWidget::verticesChanged, this, [=](unsigned int count) {
        ui->verticesCount->setText(QString::number(count));
    });
    connect(ui->openGLWidget, &OpenGLWidget::trianglesChanged, this, [=](unsigned int count) {
        ui->trianglesCount->setText(QString::number(count));
    });
    connect(ui->wireframeCheck, &QCheckBox::toggled,
            ui->openGLWidget, &OpenGLWidget::setWireframe);
    connect(errorTimer, SIGNAL(timeout()), this, SLOT(clearErrorLabel()));


}

MainWindow::~MainWindow()
{
    delete ui;
    delete errorTimer;
}

void MainWindow::clearErrorLabel() {
    ui->meshErrorLabel->clear();
}

void MainWindow::onActionLoad() {
    QString filename = QFileDialog::getOpenFileName(
        this,
        tr("Open a mesh file"),
        QString(),
        tr("Mesh file (*.txt *.obj *.off);;All files (*.*)")
        );

    if (!filename.isEmpty()) {
        int ok = ui->openGLWidget->loadMesh(filename.toStdString().c_str());
        switch(ok) {
        case MeshError::FORMAT:
            errorTimer->setSingleShot(true);
            errorTimer->start(5000);
            ui->meshErrorLabel->setText("Wrong file format, please check the selected file.");
            break;
        case MeshError::READ:
            errorTimer->setSingleShot(true);
            errorTimer->start(5000);
            ui->meshErrorLabel->setText("Error while reading the mesh file, please check the selected file.");
            break;
        case MeshError::UNKNOWN:
            errorTimer->setSingleShot(true);
            errorTimer->start(5000);
            ui->meshErrorLabel->setText("Unknown error, please try again and check the selected file.");
            break;
        default:
            break;
        }
    }
}
