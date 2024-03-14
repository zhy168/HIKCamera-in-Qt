#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QTimer>
#include <QDebug>
#include "hikcamera.h"
#include "opencv.hpp"
#include "opencv2/opencv.hpp"



namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    hikcamera* camera;
    QTimer* timer;
    int index = 0;

public:
    void test();
    void saveImg();
    void saveVedio();
    void predict_frame();
private slots:
    void on_start_btn_clicked();
    void getOneFrame();
    void on_live_btn_clicked();
    void on_close_btn_clicked();
    void on_shot_btn_clicked();
    void on_vedio_btn_clicked();
};

#endif // MAINWINDOW_H
