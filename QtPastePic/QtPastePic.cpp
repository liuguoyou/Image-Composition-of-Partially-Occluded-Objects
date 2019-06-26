#include "QtPastePic.h"
#include <qbuffer.h>
using namespace cv;
using namespace std;

QtPastePic::QtPastePic(QWidget *parent)
	:
	QMainWindow(parent),
	Paint(20, 62, 550, 380),	//�滭����
	Alloffset(0, 0)//,   //��ƫ��
	//label("100%", this)

{
	ratio = 1.0;             //��ʼ��ͼƬ���ű���
	action = QtPastePic::None;
	pixW = 985;            //����ͼƬ�ߴ�Ϊ985*740
	pixH = 740;

	ui.setupUi(this);
}



QtPastePic::~QtPastePic()
{
	//��������
}


void QtPastePic::on_target_pic_load_button_clicked()
{
	QString filename;
	filename = QFileDialog::getOpenFileName(this,
		tr("ѡ��ͼ��"),
		"",
		tr("Images (*.png *.bmp *.jpg *.tif *.GIF )"));
	if (filename.isEmpty())
	{
		return;
	}
	else
	{
		//���target mask
		target_alphas.clear();

		string str = filename.toStdString();  // ��filenameת��Ϊstring���ͣ�
		target_pic = imread(str);
		targetH = target_pic.rows;
		targetW = target_pic.cols;
		cvtColor(target_pic, target_pic, CV_BGR2RGB);
		target_img = QImage((const unsigned char*)(target_pic.data), target_pic.cols, target_pic.rows, QImage::Format_RGB888);
		
		//���¿�����ʾ
		target_pic_label = new QLabel();

		QImage newImg = target_img.scaled(ui.target_pic_show->width()*0.9, ui.target_pic_show->height()*0.9);
		target_pic_label->setPixmap(QPixmap::fromImage(newImg));

		//target_pic_label->setPixmap(QPixmap::fromImage(target_img));
		//target_pic_label->resize(QSize(target_img.width(), target_img.height()));
		ui.target_pic_show->setWidget(target_pic_label);

		//��QRect����ʾ
		QPixmap temp_pix = QPixmap::fromImage(target_img);
		target_pix = &temp_pix;

	}

}

void QtPastePic::on_source_pic_load_button_clicked()
{
	QString filename;
	filename = QFileDialog::getOpenFileName(this,
		tr("ѡ��ͼ��"),
		"",
		tr("Images (*.png *.bmp *.jpg *.tif *.GIF )"));
	if (filename.isEmpty())
	{
		return;
	}
	else
	{

		QImage loadedImage;
		if (!loadedImage.load(filename))
			return;
		pixW = loadedImage.width();
		pixH = loadedImage.height();


		string str = filename.toStdString();  // ��filenameת��Ϊstring���ͣ�
		source_pic = imread(str);

		//mat to QImage
		Mat source_pic_for_img;
		source_pic.copyTo(source_pic_for_img);	

		cvtColor(source_pic, source_pic, CV_BGR2RGB);
		//cv::resize(source_pic_for_img, source_pic_for_img, Size(pixW, pixH));
		source_img = QImage((const unsigned char*)(source_pic.data), source_pic.cols, source_pic.rows, QImage::Format_RGB888);

		//���¿�����ʾ
		source_pic_label = new QLabel();

		QImage newImg = source_img.scaled(ui.source_pic_show->width()*0.9, ui.source_pic_show->height()*0.9);
		source_pic_label->setPixmap(QPixmap::fromImage(newImg));

		//source_pic_label->setPixmap(QPixmap::fromImage(source_img));
		//source_pic_label->resize(QSize(source_img.width(), source_img.height()));

		ui.source_pic_show->setWidget(source_pic_label);

		//��QRect����ʾ
	/*	QPixmap temp_pix = QPixmap::fromImage(source_img);
		pix = &temp_pix;*/
		//makeBgTransparent();


	}
}

void QtPastePic::on_source_mask_load_button_clicked()
{
	QString filename;
	filename = QFileDialog::getOpenFileName(this,
		tr("ѡ��ͼ��"),
		"",
		tr("Images (*.png *.bmp *.jpg *.tif *.GIF )"));
	if (filename.isEmpty())
	{
		return;
	}
	else
	{

		string str = filename.toStdString();  // ��filenameת��Ϊstring���ͣ�
		source_alpha_pic = imread(str);
		//pixH = source_alpha_pic.rows;
		//pixW = source_alpha_pic.cols;

		//mat to QImage
		cvtColor(source_alpha_pic, source_alpha_pic, CV_BGR2RGB);
		//cv::resize(source_alpha_for_img, source_alpha_for_img, Size(pixW, pixH));
		source_alpha_img = QImage((const unsigned char*)(source_alpha_pic.data), source_alpha_pic.cols, source_alpha_pic.rows, QImage::Format_RGB888);

		//����͸����
		//cvtColor(source_alpha_pic, source_alpha_pic, CV_BGRA2RGBA);
		//source_alpha_img = QImage((const unsigned char*)(source_alpha_pic.data), source_alpha_pic.cols, source_alpha_pic.rows, QImage::Format_RGBA8888);
		//source_alpha_img = source_alpha_img.convertToFormat(QImage::Format_RGBA8888_Premultiplied, Qt::NoFormatConversion);


		//���¿�����ʾ
		source_alpha_label = new QLabel();

		QImage newImg = source_alpha_img.scaled(ui.source_mask_show->width()*0.9, ui.source_mask_show->height()*0.9);
		source_alpha_label->setPixmap(QPixmap::fromImage(newImg));

		//source_alpha_label->setPixmap(QPixmap::fromImage(source_alpha_img));
		//source_alpha_label->resize(QSize(source_alpha_img.width(), source_alpha_img.height()));
		ui.source_mask_show->setWidget(source_alpha_label);

		//������͸��
		////temp_pix.fill(Qt::transparent);
		////QPainter p(&temp_pix);
		update();

	}

}


void QtPastePic::on_target_mask_load_button_clicked()
{
	QString filename;
	filename = QFileDialog::getOpenFileName(this,
		tr("ѡ��ͼ��"),
		"",
		tr("Images (*.png *.bmp *.jpg *.tif *.GIF )"));
	if (filename.isEmpty())
	{
		return;
	}
	else
	{

		string str = filename.toStdString();  // ��filenameת��Ϊstring���ͣ�
		target_alpha_pic = imread(str);

		

		//pixH = target_alpha_pic.rows;
		//pixW = target_alpha_pic.cols;
		cvtColor(target_alpha_pic, target_alpha_pic, CV_BGR2RGB);
		target_alpha_img = QImage((const unsigned char*)(target_alpha_pic.data), target_alpha_pic.cols, target_alpha_pic.rows, QImage::Format_RGB888);

		target_alphas.push_back(target_alpha_pic);

		//���¿�����ʾ
		target_alpha_label = new QLabel();

		QImage newImg = target_alpha_img.scaled(ui.target_mask_show->width()*0.9, ui.target_mask_show->height()*0.9);
		target_alpha_label->setPixmap(QPixmap::fromImage(newImg));

		//target_alpha_label->setPixmap(QPixmap::fromImage(target_alpha_img));
		//target_alpha_label->resize(QSize(target_alpha_img.width(), target_alpha_img.height()));

		ui.target_mask_show->setWidget(target_alpha_label);


	}

}

void QtPastePic::on_target_depth_load_button_clicked()
{
	QString filename;
	filename = QFileDialog::getOpenFileName(this,
		tr("Img"),
		"",
		tr("Images (*.png *.bmp *.jpg *.tif *.GIF )"));
	if (filename.isEmpty())
	{
		return;
	}
	else
	{

		string str = filename.toStdString();  // ��filenameת��Ϊstring���ͣ�
		depth_pic = imread(str);
		//pixH = depth_pic.rows;
		//pixW = depth_pic.cols;
		cvtColor(depth_pic, depth_pic, CV_BGR2RGB);
		depth_img = QImage((const unsigned char*)(depth_pic.data), depth_pic.cols, depth_pic.rows, QImage::Format_RGB888);

		//���¿�����ʾ
		depth_label = new QLabel();

		QImage newImg = depth_img.scaled(ui.target_depth_show->width()*0.9, ui.target_depth_show->height()*0.9);
		depth_label->setPixmap(QPixmap::fromImage(newImg));

		//depth_label->setPixmap(QPixmap::fromImage(depth_img));
		//depth_label->resize(QSize(depth_img.width(), depth_img.height()));

		ui.target_depth_show->setWidget(depth_label);

	}

}

