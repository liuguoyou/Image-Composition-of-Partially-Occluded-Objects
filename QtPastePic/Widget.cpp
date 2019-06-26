#include "widget.h"
#include "PastePic.h"




bool QtPastePic::event(QEvent * event)
{
	static bool press = false;
	static QPoint PreDot;

	if (event->type() == QEvent::MouseButtonPress)
	{
		QMouseEvent *mouse = dynamic_cast<QMouseEvent* >(event);

		//判断鼠标是否是左键按下,且鼠标位置是否在绘画区域
		if (mouse->button() == Qt::LeftButton &&Paint.contains(mouse->pos()))
		{
			press = true;
			//QApplication::setOverrideCursor(Qt::OpenHandCursor); //设置鼠标样式
			this->setCursor(Qt::OpenHandCursor);
			PreDot = mouse->pos();
		}

	}
	else if (event->type() == QEvent::MouseButtonRelease)
	{
		QMouseEvent *mouse = dynamic_cast<QMouseEvent* >(event);

		//判断鼠标是否是左键释放,且之前是在绘画区域
		if (mouse->button() == Qt::LeftButton && press)
		{
			//QApplication::setOverrideCursor(Qt::ArrowCursor); //改回鼠标样式
			this->setCursor((Qt::ArrowCursor));
			press = false;
		}
	}

	if (event->type() == QEvent::MouseMove)              //移动图片
	{
		if (press)
		{
			QMouseEvent *mouse = dynamic_cast<QMouseEvent* >(event);

			offset.setX(mouse->x() - PreDot.x());
			offset.setY(mouse->y() - PreDot.y());
			PreDot = mouse->pos();
			action = QtPastePic::Move;
			this->update();
		}
	}
	return QWidget::event(event);
}

void QtPastePic::wheelEvent(QWheelEvent* event)     //鼠标滑轮事件
{
	if (event->delta()>0) {      //上滑,缩小

		action = QtPastePic::Shrink;
		this->update();

	}
	else {                    //下滑,放大
		action = QtPastePic::Amplification;
		this->update();
	}

	event->accept();
}



void QtPastePic::paintEvent(QPaintEvent *event)
{

	QPainter painter(this);

	int NowW = ratio * pixW;
	int NowH = ratio * pixH;

	//根据img修改pix
	QPixmap target_pix_temp;
	target_pix_temp = QPixmap::fromImage(target_img);
	target_pix = &target_pix_temp;


	QPixmap pix_temp;
	pix_temp= QPixmap::fromImage(mask_img);
	pix= &pix_temp;

	//test

	//QPixmap pix_t; pix_t.load("C:/Users/txh/Desktop/test.png");
	//pix = &pix_t;





	if (action == QtPastePic::Amplification)           //缩小
	{
		ratio -= 0.1*ratio;
		if (ratio<0.018)
			ratio = 0.01;

		/*显示比例*/
		//QString str;
		//str.sprintf("%.0f%", ratio * 100);
		//label.setText(str);
	}
	else  if (action == QtPastePic::Shrink)           //放大
	{

		ratio += 0.1*ratio;
		if (ratio>4.5)
			ratio = 5.000;

		/*显示比例*/
		//QString str;
		//str.sprintf("%.0f%", ratio * 100);
		//label.setText(str);
	}
	if (action == QtPastePic::Amplification || action == QtPastePic::Shrink)      //更新图片
	{

		NowW = ratio * pixW;
		NowH = ratio * pixH;

		//QMatrix 缩放
		QMatrix matrix;
		matrix.scale(ratio, ratio);
		//source_img = source_img.transformed(matrix);

		source_img = source_img.scaled(NowW, NowH);

		//0328
		//source_pic_label = new QLabel();
		//source_pic_label->setPixmap(QPixmap::fromImage(source_img));
		//source_pic_label->resize(QSize(source_img.width(), source_img.height()));
		//ui.source_pic_show->setWidget(source_pic_label);



		//mask_img QMatrix 缩放
		QMatrix mask_matrix;
		mask_matrix.scale(ratio, ratio);
		//mask_img = mask_img.transformed(mask_matrix);
		mask_img = mask_img.scaled(NowW, NowH);

		QPixmap temp_pix = QPixmap::fromImage(mask_img);
		pix = &temp_pix;
		*pix = pix->scaled(NowW, NowH, Qt::KeepAspectRatio);

		//算offset？
		//int offsetx = Alloffset.x() + pixW/2-NowW/2;
		//Alloffset.setX(offsetx);

		//int offsety = Alloffset.y() + pixH / 2 - NowH/2;
		//Alloffset.setY(offsety);

		action = QtPastePic::None;

	}

	if (action == QtPastePic::Move)                    //移动
	{

		QPixmap pix_temp;
		pix_temp = QPixmap::fromImage(source_img);
		pix = &pix_temp;


		int offsetx = Alloffset.x() + offset.x();
		Alloffset.setX(offsetx);

		int offsety = Alloffset.y() + offset.y();
		Alloffset.setY(offsety);
		action = QtPastePic::None;
	}

	if (abs(Alloffset.x()) >= (Paint.width() / 2 + NowW / 2 - 20))    //限制X偏移值
	{
		if (Alloffset.x()>0)
			Alloffset.setX(Paint.width() / 2 + NowW / 2 - 20);
		else
			Alloffset.setX(-Paint.width() / 2 + -NowW / 2 + 20);
	}
	if (abs(Alloffset.y()) >= (Paint.height() / 2 + NowH / 2 - 62))    //限制Y偏移值
	{
		if (Alloffset.y()>0)
			Alloffset.setY(Paint.height() / 2 + NowH / 2 - 62);
		else
			Alloffset.setY(-Paint.height() / 2 + -NowH / 2 + 62);

	}

	int x = Paint.width() / 2 + Alloffset.x() - NowW / 2;
	if (x<0)
		x = 0;

	int y = Paint.height() / 2 + Alloffset.y() - NowH / 2;
	if (y<0)
		y = 0;

	int  sx = NowW / 2 - Paint.width() / 2 - Alloffset.x();
	if (sx<0)
		sx = 0;

	int  sy = NowH / 2 - Paint.height() / 2 - Alloffset.y();
	if (sy<0)
		sy = 0;

	int w = (NowW - sx)>Paint.width() ? Paint.width() : (NowW - sx);
	if (w>(Paint.width() - x))
		w = Paint.width() - x;

	int h = (NowH - sy)>Paint.height() ? Paint.height() : (NowH - sy);
	if (h>(Paint.height() - y))
		h = Paint.height() - y;

	painter.drawRect(Paint.x() - 1, Paint.y() - 1, Paint.width() + 1, Paint.height() + 1); //画框
	//painter.drawPixmap(Paint.x(), Paint.y(), *target_pix, sx, sy, w, h);  //绘画图形
	//painter.drawPixmap(x + Paint.x(), y + Paint.y(),  *pix, 0, 0, w, h);
	//通过把pixmap的一部分复制到绘制设备中，在(x, y)绘制一个像素映射。
	//(x, y)指定了要被绘制的绘制设备的左上点。(sx, sy)指定了要被绘制的pixmap中的左上点。默认为(0, 0)。
	//(sw, sh)指定了要被绘制的pixmap的大小。默认(-1, -1)，意思是一直到像素映射的右下
	//x
	//Paint( int x, int y, int w, int h )
	//Paint.x()   Paint.y() 在
	//y
	//
	//sx
	//sy

	//x=Paint.height() / 2 - NowH / 2
	painter.drawPixmap(-targetW / 2 + Paint.width() / 2 +Paint.x(), Paint.height() / 2 - targetH / 2+Paint.y(), *target_pix, 0, 0,-1,-1);  //绘画图形
	painter.drawPixmap(x + Paint.x(), y + Paint.y(), *pix, sx, sy,-1,-1);

	
}

void  QtPastePic::on_little_clicked()
{
	action = QtPastePic::Amplification;
	this->update();
}

void  QtPastePic::on_big_clicked()
{
	action = QtPastePic::Shrink;
	this->update();
}
void QtPastePic::on_up_clicked()
{
	action = QtPastePic::Move;
	offset.setX(0);
	offset.setY(-20);

	this->update();
}
void QtPastePic::on_down_clicked()
{
	action = QtPastePic::Move;
	offset.setX(0);
	offset.setY(20);
	this->update();
}
void QtPastePic::on_left_clicked()
{
	action = QtPastePic::Move;
	offset.setX(-20);
	offset.setY(0);

	this->update();
}
void QtPastePic::on_right_clicked()
{
	action = QtPastePic::Move;
	offset.setX(20);
	offset.setY(0);

	this->update();
}

void QtPastePic::on_mask_clicked()
{
	PastePic pastepic;
	Mat source;
	Mat target;

	cvtColor(source_pic, source, CV_RGB2BGR);
	cvtColor(source_alpha_pic, target, CV_RGB2BGR);
	source_mask_pic = pastepic.Cutout(source, target);
	cvtColor(source_mask_pic, source_mask_pic, CV_BGRA2RGBA);

	mask_img = QImage((const unsigned char*)(source_mask_pic.data), source_mask_pic.cols, source_mask_pic.rows, QImage::Format_RGBA8888);
	//mask_img = mask_img.convertToFormat(QImage::Format_RGBA8888_Premultiplied, Qt::NoFormatConversion);
	//QPixmap pix_t; 
	//pix_t.load("C:/Users/txh/Desktop/test.png");
	//pix = &pix_t;

	//this->update();
}

void QtPastePic::on_output_clicked()
{
	PastePic pastepic;

	//转图的格式
	Mat source;
	Mat source_alpha;
	Mat target;
	vector<Mat> target_alphas_bgr;
	Mat depth;

	cvtColor(source_pic, source, CV_RGB2BGR);
	cvtColor(source_alpha_pic, source_alpha, CV_RGB2GRAY);
	cvtColor(target_pic, target, CV_RGB2BGR);
	cvtColor(depth_pic, depth, CV_RGB2GRAY);
	int i = 0;
	for (vector<Mat>::iterator target_alpha_iter = target_alphas.begin(); target_alpha_iter != target_alphas.end(); target_alpha_iter++)
	{
		//imshow("target_alpha_iter"+to_string(target_alphas.size()), *target_alpha_iter);
		Mat target_alpha_bgr;
		cvtColor(*target_alpha_iter, target_alpha_bgr, CV_RGB2GRAY);
		target_alphas_bgr.push_back(target_alpha_bgr);

	}
	//waitKey();

	//normal
	Mat normal_alpha = Mat::zeros(target.rows, target.cols, CV_8UC1);
	Mat normal_output_pic = pastepic.SourceLocMaskPasteImage(source, target, source_alpha, target_alphas_bgr, normal_alpha, "C:/Users/txh/Desktop/test_t8.png",  Alloffset.y()+int((targetH-ratio*pixH)/2), Alloffset.x() + int((targetW - ratio * pixW) / 2), ratio);
	normal_output_pic=normal_output_pic*255.0;
	imwrite("C:/Users/txh/Desktop/test_normal.png", normal_output_pic);
	//no test

	output_pic = pastepic.SourceLocMaskPasteImage(source, target, source_alpha, target_alphas_bgr, depth, "C:/Users/txh/Desktop/test_t8.png",  Alloffset.y()+int((targetH-ratio*pixH)/2), Alloffset.x() + int((targetW - ratio * pixW) / 2), ratio);
	//imshow(to_string(output_pic.channels()), output_pic);

	output_pic = output_pic * 255.0;

	//只能先存再读。。。。为啥
	imwrite("C:/Users/txh/Desktop/test_f.png", output_pic);
	output_pic = imread("C:/Users/txh/Desktop/test_f.png");

	cvtColor(output_pic, output_pic, CV_BGR2RGB);
	output_img = QImage((const unsigned char*)(output_pic.data), output_pic.cols, output_pic.rows, QImage::Format_RGB888);
	

	//在下框中显示
	output_label = new QLabel();
	output_label->setPixmap(QPixmap::fromImage(output_img));
	output_label->resize(QSize(output_img.width(), output_img.height()));
	ui.output_show->setWidget(output_label);

}