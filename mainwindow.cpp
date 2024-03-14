#include "mainwindow.h"
#include "ui_mainwindow.h"

using namespace std;
using namespace cv;


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    camera = hikcamera::getInstance();
    camera->initialize();
    camera->connect();

    ui->src_label->setScaledContents(true);
    ui->dst_label->setScaledContents(true);

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(getOneFrame()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::test()
{
    QString imgpath = ui -> lineEdit ->text();
    QImage *src = new QImage(imgpath);
    src -> scaled(ui->src_label->size(), Qt::KeepAspectRatio,Qt::SmoothTransformation);
    ui -> src_label ->setScaledContents(true);
    ui -> src_label -> setPixmap(QPixmap::fromImage(*src));

    cv::Mat dst = cv::imread(imgpath.toStdString());
    imshow("dst",dst);
    waitKey(0);
    cvtColor(dst,dst,CV_BGR2RGB);
    QImage dst2(dst.data, dst.cols, dst.rows, dst.step, QImage::Format_RGB888);
    //dst2 -> scaled(ui->src_label->size(), Qt::KeepAspectRatio,Qt::SmoothTransformation);
    ui -> dst_label ->setScaledContents(true);
    ui -> dst_label -> setPixmap(QPixmap::fromImage(*src));

}

void MainWindow::saveImg()
{
    cv::Mat img_save;
    camera->startGrabbing(img_save);
    string filename = "I:/shotPhoto/666"  + to_string(index++) + ".jpg";
    imwrite(filename,img_save);
    img_save.release();

}

void MainWindow::saveVedio()
{
    Mat frame;
    int frame_width = 1280;
    int frame_height = 720;
    int fps = 30;
    int duration = 10; // 录制时间，单位为秒
     time_t start_time = time(nullptr);
    // 创建VideoWriter对象来保存视频
    string filename = "I:/shotPhoto/output.avi"; // 修改为保存的路径
    VideoWriter video(filename, CV_FOURCC('M', 'J', 'P', 'G'), fps, Size(frame_width, frame_height));
    while (difftime(time(nullptr), start_time) < duration)
        {
            camera->startGrabbing(frame);
            // 写入帧到视频文件
            video.write(frame);
            frame.release();
        }
    video.release();
}

void MainWindow::predict_frame()
{



}


void MainWindow::on_start_btn_clicked()
{
    camera->startStreaming((HWND)ui->src_label->winId());//开始按键：开始取流
}

void MainWindow::getOneFrame()
{
    cv::Mat grabImg;
    camera->startGrabbing(grabImg);
    cv::Mat dst;
    //QImage qgrabImg = QImage((const unsigned char*)(dst.data), dst.cols, dst.rows, dst.cols*dst.channels(), QImage::Format_RGB888);   // Mat为BGR颜色模式;
    cvtColor(grabImg,dst,CV_BGR2GRAY);
    QImage qgrabImg;
        if (dst.channels() > 1)
        {
            qgrabImg = QImage((const unsigned char*)(dst.data), dst.cols, dst.rows, dst.cols*dst.channels(), QImage::Format_RGB888);
        }
        else
        {
            qgrabImg = QImage((const unsigned char*)(dst.data), dst.cols, dst.rows, QImage::Format_Indexed8);
        }
    qgrabImg = qgrabImg.rgbSwapped();
    ui->dst_label->setPixmap(QPixmap::fromImage(qgrabImg));
}

void MainWindow::on_live_btn_clicked()
{
    timer->start(100);
}

void MainWindow::on_close_btn_clicked()
{    
    camera->stopStreaming();
    QWidget::close();
}

void MainWindow::on_shot_btn_clicked()
{
    saveImg();
}

void MainWindow::on_vedio_btn_clicked()
{
    saveVedio();
}
