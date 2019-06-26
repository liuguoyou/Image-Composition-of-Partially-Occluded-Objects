#pragma once


#include <QWidget>
#include <QtGui>
#include <QtWidgets/QMainWindow>
#include "ui_QtPastePic.h"
#include<QGraphicsScene>  
#include<QGraphicsView>                   //graphicsview类
#include<opencv2\imgproc\imgproc.hpp>
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>  //opencv申明
#include <qfiledialog.h>                //getopenfilename 类申明
#include <qlabel.h>                     //label类
#include<qapplication.h>
#include "QtPastePic.h"


//class QtPastePic : public QMainWindow
//{
//	Q_OBJECT
//
//private:
//	QPixmap * pix;
//	int action;          //动作(放大,缩小,移动...)
//	int pixW;            //图片宽
//	int pixH;            //图片高
//
//	QRect Paint;         //绘画区域
//
//	float ratio;                //比例
//	QPoint offset;              //一次的图片偏移值
//	QPoint Alloffset;           //总偏移
//	QLabel label;
//
//	QPushButton  BigButton;
//	QPushButton  LittleButton;
//	QPushButton  LiftButton;
//	QPushButton  RightButton;
//	QPushButton  UpButton;
//	QPushButton  DownButton;
//
//	void AddComboItem(QComboBox* cmbo);
//	bool event(QEvent * event);
//	void wheelEvent(QWheelEvent* e);     //鼠标滑轮事件
//private slots:
//	void    on_little_clicked();
//	void    on_big_clicked();
//	void    on_up_clicked();
//	void    on_down_clicked();
//	void    on_left_clicked();
//	void    on_right_clicked();
//
//
//	void paintEvent(QPaintEvent *event);
//public:
//	//explicit QtPastePic();
//
//	enum  Type {
//		None = 0,
//		Amplification,
//		Shrink,
//		Lift,
//		Right,
//		Up,
//		Down,
//		Move
//	};
//
//};
