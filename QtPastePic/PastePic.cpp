//
////#include "saliency/saliency.h"
//#include <cstdio>
////#include "commun.h"
//#include <opencv2\opencv.hpp>
//#include <vector>
//#include <queue>
//#include <set>
#include "PastePic.h"

//using namespace cv;
//using namespace std;



bool kernelJudge(Mat pic, int x, int y) {
	for (int i = -2; i<3; i++)
	{
		for (int j = -2; j<3; j++)
		{
			if (x + i<pic.rows && y + j<pic.cols)
			{
				//blue
				if (int(pic.at<Vec3b>(x + i, y + j)[0]) == 255 && int(pic.at<Vec3b>(x + i, y + j)[1]) == 0 && int(pic.at<Vec3b>(x + i, y + j)[2]) == 0)
				{
					//cout<<0<<endl;
					return false;
				}
			}
		}
	}
	//red bg
	return true;	//accurate

}



bool boundaryJudge(Mat pic, int x, int y, int d) {
	bool blue = false;
	bool red = false;
	if (x<d || y<d || x >= pic.rows - d || y >= pic.cols - d)
	{
		return false;
	}
	for (int i = -d; i<d + 1; i++)
	{
		for (int j = -d; j<d + 1; j++)
		{
			if (int(pic.at<Vec3b>(x + i, y + j)[0]) == 0 && int(pic.at<Vec3b>(x + i, y + j)[1]) == 0 && int(pic.at<Vec3b>(x + i, y + j)[2]) == 255)
			{
				red = true;
			}
			if (int(pic.at<Vec3b>(x + i, y + j)[0]) == 255 && int(pic.at<Vec3b>(x + i, y + j)[1]) == 0 && int(pic.at<Vec3b>(x + i, y + j)[2]) == 0)
			{
				blue = true;
			}
		}
	}

	return (red == true && blue == true);	//accurate
}




Mat PastePic::PasteImage(Mat&source_pic, Mat&target_pic, Mat&source_alpha_pic, Mat&target_alpha_pic, char * save_path, int ox, int oy) {
	source_pic.convertTo(source_pic, CV_32F, 1.0 / 255.0);
	target_pic.convertTo(target_pic, CV_32F, 1.0 / 255.0);
	source_alpha_pic.convertTo(source_alpha_pic, CV_32FC1, 1.0 / 255.0);
	target_alpha_pic.convertTo(target_alpha_pic, CV_32FC1, 1.0 / 255.0);
	Mat composite, source_temp;
	target_pic.copyTo(composite);
	source_pic.copyTo(source_temp);
	//int ox=0,oy=0;
	for (int j = 0; j<source_alpha_pic.rows; j++)
	{
		for (int i = 0; i<source_alpha_pic.cols; i++)
		{
			if (j + ox<0 || j + ox >= target_pic.rows || i + oy<0 || i + oy >= target_pic.cols)
			{
				continue;
			}

			float target_alpha = target_alpha_pic.at<float>(j + ox, i + oy);
			float source_alpha = source_alpha_pic.at<float>(j, i);
			source_temp.at<Vec3f>(j, i) = source_alpha * source_pic.at<Vec3f>(j, i) + (1 - source_alpha)*target_pic.at<Vec3f>(ox + j, oy + i);
			composite.at<Vec3f>(ox + j, oy + i) = target_alpha * target_pic.at<Vec3f>(ox + j, oy + i) + (1 - target_alpha)*source_temp.at<Vec3f>(j, i);

		}
	}
	//imshow("composite", composite);
	//imwrite(save_path, composite*255.0);
	return composite;
}


//双边滤波
double BiFilter(Mat source_pic, Mat alpha_pic, int x, int y, int d, float sigma_color, float sigma_space) {
	int radius;
	float gauss_color_coeff = -0.5 / (sigma_color*sigma_color);
	float gauss_space_coeff = -0.5 / (sigma_space*sigma_space);
	alpha_pic.convertTo(alpha_pic, CV_32FC1);
	imshow("alpha_pic", alpha_pic / 255.0);
	//waitKey();
	if (d <= 0)	//如果调用双边滤波函数时给出的滤波内核大小小于等于0
		radius = cvRound(sigma_space*1.5);		//根据σd来自动给出内核的半径
	else			//否则得到内核的半径
		radius = d / 2;
	radius = MAX(radius, 1);		//保证内核半径大于1
	d = radius * 2 + 1;		//重新得到内核大小尺寸
	vector<float> color_weight(d*d);
	vector<float> space_weight(d*d);
	vector<float> bi_weight(d*d);
	vector<float> alpha(d*d);
	vector<float> molecule(d*d);
	float all_weight = 0, all_molecule = 0;
	int i = -radius; int maxk = 0;
	for (; i <= radius; i++)
	{
		for (int j = -radius; j <= radius; j++)
		{
			float color_diff = 0;
			float r = std::sqrt((float)i*i + (float)j*j);		//距离范数
			if (r > radius || i + x<0 || i + x >= alpha_pic.rows || j + y<0 || j + y >= alpha_pic.cols)	//如果距离大于内核半径，则抛弃该值
				continue;

			space_weight[maxk] = (float)std::exp(r*r*gauss_space_coeff);	//高斯公式

			for (int c = 0; c<3; c++)
			{
				color_diff += float(source_pic.at<Vec3f>(x + i, y + j)[c] - source_pic.Mat::at<Vec3f>(x, y)[c]);
			}
			color_weight[maxk] = (float)std::exp(color_diff*color_diff*gauss_color_coeff);
			bi_weight[maxk] = space_weight[maxk] * color_weight[maxk];
			alpha[maxk] = float(alpha_pic.at<float>(x + i, y + j));
			molecule[maxk] = alpha[maxk] * bi_weight[maxk];
			all_molecule += molecule[maxk];
			all_weight += bi_weight[maxk];
			//std::cout<<"alpha"<<alpha[maxk]<<"bi_weight"<<bi_weight[maxk]<<endl;
			maxk++;
		}
	}
	float final_alpha = all_molecule / all_weight;
	//std::cout<<final_alpha-alpha_pic.at<float>(x,y)<<endl;
	return final_alpha;
}


//ac to bi
void acAlphaPic(Mat ac_pic, char *path) {
	ac_pic.convertTo(ac_pic, CV_8UC3);
	Mat ac_alpha_pic(ac_pic.rows, ac_pic.cols, CV_8UC1, Scalar(0, 0, 0));
	for (int i = 0; i<ac_pic.rows; i++)
	{
		for (int j = 0; j<ac_pic.cols; j++)
		{
			//blue
			if (int(ac_pic.at<Vec3b>(i, j)[0]) == 255 && int(ac_pic.at<Vec3b>(i, j)[1]) == 0 && int(ac_pic.at<Vec3b>(i, j)[2]) == 0)
			{
				ac_alpha_pic.at<uchar>(i, j) = 255;
			}
			//red
			else
			{
				ac_alpha_pic.at<uchar>(i, j) = 0;
			}
		}
	}
	imshow("ac_alpha_pic", ac_alpha_pic);
	imwrite(path, ac_alpha_pic);
}

//双边滤波
void boundaryBiFilter(Mat ac_pic, Mat alpha_pic, Mat source_pic, int r_boundary, int d_filter, char * target_path)
{
	Mat target_alpha_pic;
	source_pic.copyTo(target_alpha_pic);
	for (int x = r_boundary; x<ac_pic.rows - r_boundary; x++)
	{
		for (int y = r_boundary; y<ac_pic.cols - r_boundary; y++)
		{
			//change boundary
			if (boundaryJudge(ac_pic, x, y, r_boundary))
			{
				if (x - (d_filter + 1) / 2>0 && x - (d_filter + 1) / 2<alpha_pic.rows && y - (d_filter + 1) / 2>0 && y - (d_filter + 1) / 2<alpha_pic.cols)
				{
					target_alpha_pic.at<float>(x, y) = BiFilter(source_pic, alpha_pic, x, y, d_filter, 70, 70);
					//后两个参数：double sigma_color, double sigma_space
				}
			}
		}
	}
	imshow("target_alpha_pic", target_alpha_pic / 255.0);
	imwrite(target_path, target_alpha_pic);
}

void fliterSegWithBi(Mat seg_pic, Mat ac_pic, char *output_path)
{
	Mat output_pic;
	seg_pic.copyTo(output_pic);
	for (int i = 0; i<seg_pic.rows; i++)
	{
		for (int j = 0; j<seg_pic.cols; j++)
		{
			if (ac_pic.at<uchar>(i, j) == 0)
			{
				//cout<<i<<" "<<j<<endl;
				output_pic.at<float>(i, j) = 0.0;
			}
		}
	}
	imshow("output_pic", output_pic / 255.0);
	imwrite(output_path, output_pic);
}

void Unnoising(Mat pic, int max_r, char *save_path)
{
	Mat unnoise_pic;
	pic.copyTo(unnoise_pic);
	int pic_rows = pic.rows;
	int pic_cols = pic.cols;
	for (int i = max_r; i<pic_rows - max_r; i++)
	{
		for (int j = max_r; j<pic_cols - max_r; j++)
		{
			for (int r = max_r; r>0; r--)
			{
				bool all_red = true;

				for (int x = -r; x <= r; x++)
				{
					int y1 = sqrt(r*r - x * x);
					int y2 = -sqrt(r*r - x * x);
					if (pic.at<Vec3b>(i + x, j + y1)[0] == 255 || pic.at<Vec3b>(i + x, j + y2)[0] == 255)
					{
						all_red = false;
						break;
					}
				}

				if (all_red)
				{
					//以当前半径r为圆，内部全部置为红色
					//cout<<i<<" "<<j<<endl;
					for (int x = -r; x <= r; x++)
					{
						for (int y = -r; y <= r; y++)
						{
							if (x*x + y * y<r*r)
							{
								//cout<<i+x<<" "<<j+y<<endl;
								//cout<<"rgb"<<int(unnoise_pic.at<Vec3b>(i+x,j+y)[0])<<" "<<int(unnoise_pic.at<Vec3b>(i+x,j+y)[1])<<" "<<int(unnoise_pic.at<Vec3b>(i+x,j+y)[2])<<endl;
								unnoise_pic.at<Vec3b>(i + x, j + y)[0] = uchar(0);
								unnoise_pic.at<Vec3b>(i + x, j + y)[1] = uchar(0);
								unnoise_pic.at<Vec3b>(i + x, j + y)[2] = uchar(255);
								//cout<<"new rgb"<<int(unnoise_pic.at<Vec3b>(i+x,j+y)[0])<<" "<<int(unnoise_pic.at<Vec3b>(i+x,j+y)[1])<<" "<<int(unnoise_pic.at<Vec3b>(i+x,j+y)[2])<<endl;
							}
						}
					}
					break;
				}
			}
		}
	}
	imshow("pic", unnoise_pic);
	imwrite(save_path, unnoise_pic);
}

void pinkConvert(Mat pink_alpha, char* ac_pic_path, char* alpha_pic_path)
{
	//pink:128 128 192
	Mat ac_pic, alpha_pic;
	pink_alpha.convertTo(pink_alpha, CV_8UC3);
	pink_alpha.copyTo(ac_pic);
	pink_alpha.copyTo(alpha_pic);
	for (int i = 0; i<pink_alpha.rows; i++)
	{
		for (int j = 0; j<pink_alpha.cols; j++)
		{
			//pink
			if (pink_alpha.at<Vec3b>(i, j)[2]>0)
			{
				ac_pic.at<Vec3b>(i, j)[0] = 255;
				ac_pic.at<Vec3b>(i, j)[1] = 0;
				ac_pic.at<Vec3b>(i, j)[2] = 0;

				alpha_pic.at<Vec3b>(i, j)[0] = 255;
				alpha_pic.at<Vec3b>(i, j)[1] = 255;
				alpha_pic.at<Vec3b>(i, j)[2] = 255;
				//cout<<int(pink_alpha.at<Vec3b>(i,j)[0])<<" "<<int(pink_alpha.at<Vec3b>(i,j)[1])<<" "<<int(pink_alpha.at<Vec3b>(i,j)[2])<<endl;
			}
			else
			{
				ac_pic.at<Vec3b>(i, j)[0] = 0;
				ac_pic.at<Vec3b>(i, j)[1] = 0;
				ac_pic.at<Vec3b>(i, j)[2] = 255;

				alpha_pic.at<Vec3b>(i, j)[0] = 0;
				alpha_pic.at<Vec3b>(i, j)[1] = 0;
				alpha_pic.at<Vec3b>(i, j)[2] = 0;
			}
		}
	}
	imshow("ac_pic", ac_pic);
	imshow("alpha_pic", alpha_pic);
	//waitKey();
}




double ObjDepth(Mat obj_mask, Mat depth_pic, int col, int row) {
	obj_mask.convertTo(obj_mask, CV_8UC1);
	depth_pic.convertTo(depth_pic, CV_8UC1);
	int rows_len = obj_mask.rows;
	int cols_len = obj_mask.cols;
	int depth_pic_rows_len = depth_pic.rows;
	int depth_pic_cols_len = depth_pic.cols;
	int N = 0;
	int depth = 0;
	for (int i = 0; i<rows_len; i++)
	{
		for (int j = 0; j<cols_len; j++)
		{
			if (obj_mask.at<uchar>(i, j)>200 && i + row<depth_pic_rows_len && j + col<depth_pic_cols_len)
			{
				depth += depth_pic.at<uchar>(i + row, j + col);
				N++;
			}
		}
	}
	double ans = double(depth) / N;
	cout << "source depth:" << ans << endl;
	return ans;
}



double ObjDepth(Mat obj_mask, Mat depth_pic) {
	obj_mask.convertTo(obj_mask, CV_8UC1);
	depth_pic.convertTo(depth_pic, CV_8UC1);
	int rows_len = obj_mask.rows;
	int cols_len = obj_mask.cols;
	int N = 0;
	int depth = 0;

	for (int i = 0; i<rows_len; i++)
	{
		for (int j = 0; j<cols_len; j++)
		{
			if (obj_mask.at<uchar>(i, j)>200)
			{
				depth += depth_pic.at<uchar>(i, j);
				N++;
			}
		}
	}
	double ans = double(depth) / N;
	cout << "target depth:" << ans << endl;
	return ans;
}


double PastePic::ObjDepthFoot(Mat obj_mask, Mat depth_pic, vector<Mat> target_alphas, int col, int row, int area_row, int area_col) {
	obj_mask.convertTo(obj_mask, CV_8UC1);
	depth_pic.convertTo(depth_pic, CV_8UC1);
	int rows_len = obj_mask.rows;
	int cols_len = obj_mask.cols;
	int depth_pic_rows_len = depth_pic.rows;
	int depth_pic_cols_len = depth_pic.cols;
	int N = 0;
	int depth = 0;
	int bottom_row = 0;
	int bottom_col = 0;

	for (int i = 0; i<rows_len; i++)
	{
		for (int j = 0; j<cols_len; j++)
		{
			if (obj_mask.at<uchar>(i, j)>200 && i + row<depth_pic_rows_len && j + col<depth_pic_cols_len&& i + row>0 && j + col>0)
			{
				bottom_col = bottom_col>j ? bottom_col : j;
				bottom_row = bottom_row>i ? bottom_row : i;
			}
		}
	}

	for (int i = bottom_row; i>0 && i>bottom_row - area_col; i--)
	{
		for (int j = bottom_col; j>0 && j>bottom_col - area_row; j--)
		{
			if (obj_mask.at<uchar>(i, j)>200 && i + row<depth_pic_rows_len && j + col<depth_pic_cols_len && j + col<depth_pic_cols_len&& i + row>0 && j + col>0)
			{
				bool obj = false;
				for (vector<Mat>::iterator target_alpha_iter = target_alphas.begin(); target_alpha_iter != target_alphas.end(); target_alpha_iter++)
				{
					if ((*target_alpha_iter).at<uchar>(i + row, j + col)>250)
					{
						obj = true;
						break;
					}
				}
				if (obj == false)
				{
					depth += depth_pic.at<uchar>(i + row, j + col);
					N++;
				}
			}
		}
	}
	double ans = double(depth) / N;
	cout << "source depth:" << ans << endl;
	return ans;
}



double PastePic::ObjDepthFoot(Mat obj_mask, Mat depth_pic, int area_row, int area_col) {
	obj_mask.convertTo(obj_mask, CV_8UC1);
	depth_pic.convertTo(depth_pic, CV_8UC1);
	int rows_len = obj_mask.rows;
	int cols_len = obj_mask.cols;
	int N = 0;
	int depth = 0;
	int bottom_row = 0;
	int bottom_col = 0;

	int depth_pic_rows_len = depth_pic.rows;
	int depth_pic_cols_len = depth_pic.cols;



	for (int i = 0; i<rows_len; i++)
	{
		for (int j = 0; j<cols_len; j++)
		{
			if (obj_mask.at<uchar>(i, j)>200 && i <depth_pic_rows_len && j <depth_pic_cols_len)
			{
				bottom_col = bottom_col>j ? bottom_col : j;
				bottom_row = bottom_row>i ? bottom_row : i;
			}
		}
	}

	for (int i = bottom_row; i>0 && bottom_row - area_col; i--)
	{
		for (int j = bottom_col; j>0 && bottom_col - area_row; j--)
		{

			if (obj_mask.at<uchar>(i, j)<5)
			{
				depth += depth_pic.at<uchar>(i, j);
				N++;
			}
		}
	}
	double ans = double(depth) / N;
	cout << "target depth:" << ans << endl;
	return ans;
}

void PastePic::countLoc(double source_depth, double target_depth, Mat target_alpha, Mat& loc_alpha)
{
	target_alpha.convertTo(target_alpha, CV_8UC1);
	loc_alpha.convertTo(loc_alpha, CV_8UC1);
	int rows_len = target_alpha.rows;
	int cols_len = target_alpha.cols;
	if (target_depth<source_depth)
	{
		for (int i = 0; i<rows_len; i++)
		{
			for (int j = 0; j<cols_len; j++)
			{
				if (target_alpha.at<uchar>(i, j)>200)
				{
					loc_alpha.at<uchar>(i, j) = 255;
				}
			}
		}
	}
	//imshow("source_loc_alpha", loc_alpha);
}


Mat PastePic::SourceLocMask(float resize_ratio, Mat source_alpha, vector<Mat> target_alphas, Mat target_depth_pic, int col, int row)
{
	vector<Mat> target_alpha;
	Mat source_alpha_resize;
	int	resize_cols = source_alpha.cols*resize_ratio;
	int	resize_rows = source_alpha.rows*resize_ratio;
	resize(source_alpha, source_alpha_resize, Size(resize_cols, resize_rows), 0, 0, INTER_LINEAR);
	//double source_depth=ObjDepth(source_alpha_resize,target_depth_pic,col,row);
	double source_depth = ObjDepthFoot(source_alpha_resize, target_depth_pic, target_alphas, col, row, 50, 100);
	vector<double> depthes;
	Mat source_loc_alpha = Mat::zeros(target_depth_pic.rows, target_depth_pic.cols, CV_8UC1);
	for (vector<Mat>::iterator target_alpha_iter = target_alphas.begin(); target_alpha_iter != target_alphas.end(); target_alpha_iter++)
	{
		//double target_depth=ObjDepth(*target_alpha_iter,target_depth_pic);
		double target_depth = ObjDepthFoot(*target_alpha_iter, target_depth_pic, 50, 100);
		countLoc(source_depth, target_depth, *target_alpha_iter, source_loc_alpha);
	}

	//imshow("source_loc_alpha", source_loc_alpha);
	return source_loc_alpha;
}

Mat PastePic::sourceMaskInTarget(Mat source_alpha, vector<Mat> target_alphas, int row_loc, int col_loc)
{
	Mat target_alpha1 = target_alphas[0];
	Mat res = Mat::zeros(target_alpha1.rows, target_alpha1.cols, CV_32FC1);
	int source_rows_len = source_alpha.rows;
	int source_cols_len = source_alpha.cols;
	int target_rows_len = target_alpha1.rows;
	int target_cols_len = target_alpha1.cols;

	for (int i = 0; i<source_rows_len && i + row_loc<target_rows_len; i++)
	{
		for (int j = 0; j<source_cols_len && j + col_loc<target_cols_len; j++)
		{
			if (source_alpha.at<uchar>(i, j)>200)
			{
				for (vector<Mat>::iterator target_alpha_iter = target_alphas.begin(); target_alpha_iter != target_alphas.end(); target_alpha_iter++)
				{
					if ((*target_alpha_iter).at<uchar>(i + row_loc, j + col_loc)>250)
					{
						continue;
					}
				}
				res.at<uchar>(i + row_loc, j + col_loc) = 0;

			}
		}
	}
	//imshow("res_alpha", res);
	return res;
}


Mat PastePic::Cutout(Mat source_pic, Mat source_alpha)
{


	source_pic.convertTo(source_pic, CV_8UC3);
	cvtColor(source_alpha, source_alpha, CV_BGR2GRAY);
	int c = source_alpha.channels();
	string s = to_string(c);
	//imshow(s, source_alpha);

	int rows_len = source_pic.rows;
	int cols_len = source_pic.cols;

	Mat cutout_pic = Mat::zeros(rows_len, cols_len, CV_8UC4);

	for (int i = 0; i<rows_len; i++)
	{
		for (int j = 0; j<cols_len; j++)
		{
			//cout<<i<<" "<<j<<endl;
			if (source_alpha.at<uchar>(i, j)<5)
			{
				cutout_pic.at<Vec4b>(i, j)[3] = 0;
			}
			else
			{
				//不透明
				cutout_pic.at<Vec4b>(i, j)[3] = 255;
				for (int channel = 0; channel<3; channel++)
				{
					cutout_pic.at<Vec4b>(i, j)[channel] = source_pic.at<Vec3b>(i, j)[channel];
				}
			}
		}
	}
	//imshow("cutout_pic", cutout_pic);
	imwrite("C:/Users/txh/Desktop/test.png", cutout_pic);
	return cutout_pic;

}


Mat PastePic::SourceLocMaskPasteImage(Mat source_pic, Mat target_pic, Mat source_alpha_pic, vector<Mat> target_alphas, Mat depth_pic, char * save_path, int ox, int oy, float ratio)
{
	//SourceLocMask(float resize_ratio, Mat source_alpha, vector<Mat> target_alphas, Mat target_depth_pic, int col, int row)



	Mat target_alpha_pic = SourceLocMask(ratio, source_alpha_pic, target_alphas, depth_pic, ox, oy);
	int col_size = source_alpha_pic.cols*ratio;
	int row_size = source_alpha_pic.rows*ratio;


	//imshow("target_alpha_pic",target_alpha_pic);

	//acAlphaPic(f_ac,"C:/Users/txh/Desktop/test/2011_004356_alpha1.jpg");
	//Mat f_alpha_pic=imread("C:/Users/txh/Desktop/test/2011_004356_alpha1.jpg",0);

	Mat source_alpha_pic_resize, source_pic_resize;
	resize(source_alpha_pic, source_alpha_pic_resize, Size(col_size, row_size), 0, 0, INTER_LINEAR);
	resize(source_pic, source_pic_resize, Size(col_size, row_size), 0, 0, INTER_LINEAR);
	Mat composite = PasteImage(source_pic_resize, target_pic, source_alpha_pic_resize, target_alpha_pic, "C:/Users/txh/Desktop/test_out.png", ox, oy);
	//imshow("composite_final", composite);
	return composite;
}


//int main(int argc, char * argv[])
//{
//
//	//背景与前景在同一张图片中
//	//二值分割+高斯滤波处理边界
//	Mat source_pic = imread("C:/Users/txh/Desktop/test_sort/2011_005577.jpg", 1);
//	Mat target_pic = imread("C:/Users/txh/Desktop/test_sort/t6.png", 1);
//	Mat source_alpha_pic = imread("C:/Users/txh/Desktop/test_sort/2011_005577_alpha2.png", 0);
//	Mat depth_pic = imread("C:/Users/txh/Desktop/test_sort/2007_009901_depth.jpg", 0);
//	Mat target_alpha1 = imread("C:/Users/txh/Desktop/test_sort/2007_009901_alpha1.png", 0);
//	Mat target_alpha2 = imread("C:/Users/txh/Desktop/test_sort/2007_009901_alpha2.png", 0);
//	vector<Mat> target_alphas;
//	target_alphas.push_back(target_alpha1);
//	target_alphas.push_back(target_alpha2);
//
//	Mat target_alpha_pic = SourceLocMask(1, source_alpha_pic, target_alphas, depth_pic, 50, -40);
//
//	Mat source_alpha_pic_resize, source_pic_resize;
//	resize(source_alpha_pic, source_alpha_pic_resize, Size(source_alpha_pic.cols / 1, source_alpha_pic.rows / 1), 0, 0, INTER_LINEAR);
//	resize(source_pic, source_pic_resize, Size(source_pic.cols / 1, source_pic.rows / 1), 0, 0, INTER_LINEAR);
//	PasteImage5(source_pic_resize, target_pic, source_alpha_pic_resize, target_alpha_pic, "C:/Users/txh/Desktop/test_sort/t8.png", 50, -40);
//
//	waitKey();
//
//	return 0;
//}

