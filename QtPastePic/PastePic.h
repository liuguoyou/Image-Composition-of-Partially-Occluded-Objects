#pragma once

//#include "saliency/saliency.h"
#include <cstdio>
//#include "commun.h"
#include <opencv2\opencv.hpp>
#include <vector>
#include <queue>
#include <set>
#include "PastePic.h"

using namespace cv;
using namespace std;

class PastePic {
private:
	//void PasteImage(Mat&source_pic, Mat&target_pic, Mat&source_alpha_pic, Mat&target_alpha_pic, char * save_path, int ox, int oy);
	double ObjDepth(Mat obj_mask, Mat depth_pic, int col, int row);
	double ObjDepth(Mat obj_mask, Mat depth_pic);
	double ObjDepthFoot(Mat obj_mask, Mat depth_pic, vector<Mat> target_alphas, int col, int row, int area_row, int area_col);
	double ObjDepthFoot(Mat obj_mask, Mat depth_pic, int area_row, int area_col);
	void countLoc(double source_depth, double target_depth, Mat target_alpha, Mat& loc_alpha);
	Mat SourceLocMask(float resize_ratio, Mat source_alpha, vector<Mat> target_alphas, Mat target_depth_pic, int col, int row);
	Mat sourceMaskInTarget(Mat source_alpha, vector<Mat> target_alphas, int row_loc, int col_loc);
	Mat PasteImage(Mat&source_pic, Mat&target_pic, Mat&source_alpha_pic, Mat&target_alpha_pic, char * save_path, int ox, int oy);
public:
	Mat Cutout(Mat source_pic, Mat source_alpha);
	Mat SourceLocMaskPasteImage(Mat source_pic, Mat target_pic, Mat source_alpha_pic, vector<Mat> target_alphas, Mat depth_pic, char * save_path, int ox, int oy, float ratio);
	Mat target_pic;
	Mat source_pic;
	vector<Mat> target_alphas;
	Mat depth_pic;
	Mat source_alpha_pic;
	Mat target_alpha_pic;
	int loc_row;
	int loc_col;

};
