#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_QtPastePic.h"
#include<QGraphicsScene>  
#include<QGraphicsView>                   //graphicsview��
#include<opencv2\imgproc\imgproc.hpp>
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>  //opencv����
#include <qfiledialog.h>                //getopenfilename ������
#include <qlabel.h>                     //label��
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

	//��Ӳۺ���
private slots:
	void on_source_pic_load_button_clicked();
	//����ʽon_�ؼ���_clicked����������QT��Ĭ����ɺ����Ͱ�ť���������ӣ���������������Ļ���ȥ�����źŲۺ���
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

	int action;          //����(�Ŵ�,��С,�ƶ�...)
	int pixW;            //sourceͼƬ��
	int pixH;            //sourceͼƬ��
	int targetW;            //targetͼƬ��
	int targetH;            //targetͼƬ��

	QRect Paint;         //�滭����

	float ratio;                //����
	QPoint offset;              //һ�ε�ͼƬƫ��ֵ
	QPoint Alloffset;           //��ƫ��
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
	void wheelEvent(QWheelEvent* e);     //��껬���¼�

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
