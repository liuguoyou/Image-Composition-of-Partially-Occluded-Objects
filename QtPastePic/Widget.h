#pragma once


#include <QWidget>
#include <QtGui>
#include <QtWidgets/QMainWindow>
#include "ui_QtPastePic.h"
#include<QGraphicsScene>  
#include<QGraphicsView>                   //graphicsview��
#include<opencv2\imgproc\imgproc.hpp>
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>  //opencv����
#include <qfiledialog.h>                //getopenfilename ������
#include <qlabel.h>                     //label��
#include<qapplication.h>
#include "QtPastePic.h"


//class QtPastePic : public QMainWindow
//{
//	Q_OBJECT
//
//private:
//	QPixmap * pix;
//	int action;          //����(�Ŵ�,��С,�ƶ�...)
//	int pixW;            //ͼƬ��
//	int pixH;            //ͼƬ��
//
//	QRect Paint;         //�滭����
//
//	float ratio;                //����
//	QPoint offset;              //һ�ε�ͼƬƫ��ֵ
//	QPoint Alloffset;           //��ƫ��
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
//	void wheelEvent(QWheelEvent* e);     //��껬���¼�
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
