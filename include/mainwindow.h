#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

signals:

public slots:
    void clearErrorLabel();

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void onActionLoad();
    void onActionSave();

private:
    Ui::MainWindow *ui;
    QTimer *errorTimer;
};
#endif // MAINWINDOW_H
