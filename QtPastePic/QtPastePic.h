#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_QtPastePic.h"
#include<QGraphicsScene>  
#include<QGraphicsView>                   //graphicsview类
#include<opencv2\imgproc\imgproc.hpp>
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>  //opencv申明
#include <qfiledialog.h>                //getopenfilename 类申明
#include <qlabel.h>                     //label类
#include <cstdio>
#include <opencv2\opencv.hpp>
#include <vector>
#include <queue>
#include <set>

using namespace cv;
using namespace std;




class QtPastePic : public QMainWindow
{
	Q_OBJECT

public:
	QtPastePic(QWidget *parent = Q_NULLPTR);
	~QtPastePic();

	//添加槽函数
private slots:
	void on_source_pic_load_button_clicked();
	//按格式on_控件名_clicked命名函数，QT会默认完成函数和按钮动作的链接，如果不这样命名的话就去设置信号槽函数
	void on_target_pic_load_button_clicked();
	void on_source_mask_load_button_clicked();
	void on_target_mask_load_button_clicked();
	void on_target_depth_load_button_clicked();


public:
	Mat target_pic;
	Mat source_pic;
	vector<Mat> target_alphas;
	Mat depth_pic;
	Mat source_alpha_pic;
	Mat target_alpha_pic;
	Mat source_mask_pic;
	Mat output_pic;


private:
	Ui::QtPastePicClass ui;

	
	QLabel *source_pic_label;
	QLabel *target_pic_label;
	QLabel *source_alpha_label;
	QLabel *target_alpha_label;
	QLabel *depth_label;
	QLabel *output_label;


	//void makeBgTransparent();

private:

	QImage target_img;
	QImage source_img;
	QImage source_alpha_img;
	QImage target_alpha_img;
	QImage depth_img;
	QImage output_img;
	QImage mask_img;

	QLabel *source_imageLabel;
	QScrollArea *source_scrollArea;

	QPixmap  *pix;
	QPixmap  *target_pix;

	int action;          //动作(放大,缩小,移动...)
	int pixW;            //source图片宽
	int pixH;            //source图片高
	int targetW;            //target图片宽
	int targetH;            //target图片高

	QRect Paint;         //绘画区域

	float ratio;                //比例
	QPoint offset;              //一次的图片偏移值
	QPoint Alloffset;           //总偏移
	QLabel label;




	QPushButton  BigButton;
	QPushButton  LittleButton;
	QPushButton  LiftButton;
	QPushButton  RightButton;
	QPushButton  UpButton;
	QPushButton  DownButton;
	QPushButton  maskButton;
	QPushButton  outputButton;


	void AddComboItem(QComboBox* cmbo);
	bool event(QEvent * event);
	void wheelEvent(QWheelEvent* e);     //鼠标滑轮事件

private slots:
	void    on_little_clicked();
	void    on_big_clicked();
	void    on_up_clicked();
	void    on_down_clicked();
	void    on_left_clicked();
	void    on_right_clicked();
	void    on_mask_clicked();
	void    on_output_clicked();



	void paintEvent(QPaintEvent *event);
public:
	//explicit QtPastePic();

	enum  Type {
		None = 0,
		Amplification,
		Shrink,
		Lift,
		Right,
		Up,
		Down,
		Move
	};


};
