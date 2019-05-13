


#include <fstream>
#include <sstream>
#include <iostream>

#include <opencv2/opencv.hpp>
#include <opencv2/dnn/dnn.hpp>
#include <opencv2/dnn.hpp>
#include <opencv2/core/utility.hpp>
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include <opencv2/core/types_c.h>
#include "string_process.h"


using namespace cv;
using namespace dnn;
using namespace std;





//-------------------------
//存储rect 能够在界面上 画边框
std::map<int, std::vector<cv::Rect>> map_rect_;

//ground truth
std::vector< std::vector< string>> vec_gt_;


//define file path 在这里替换成您自己的路径
std::string img_dir_="/home/santiago/MOT16/train/MOT16-04/img1/";
std::string det_file_path_="/home/santiago/MOT16/train/MOT16-04/det/det.txt";
std::string gt_file_path="/home/santiago/MOT16/train/MOT16-04/gt/gt.txt";


std::vector<std::string> split(const std::string& s, char delimiter)
{
   std::vector<std::string> tokens;
   std::string token;
   std::istringstream tokenStream(s);
   while (std::getline(tokenStream, token, delimiter))
   {
      tokens.push_back(token);
   }
   return tokens;
}

void ReadDetFile(const std::string &file)
{
  std::vector<std::string> lines;
  std::ifstream f(file.c_str());
  if (f.is_open())
  {
      std::string line = "";
      while (std::getline(f, line))
          lines.push_back(line);
  }

  std::vector<cv::Rect> rcs;
  int num = -1;
  int tmpNum = -1;
  for (int i = 0; i < lines.size(); i++)
    {
      std::vector<std::string> cols=split(lines[i],',');

      if (cols.size() < 6)
        {
          continue;
        }

      tmpNum = toInt(trim(cols[0]));
      if (num!=-1 && tmpNum!=num)
        {
          map_rect_.insert(std::make_pair(num, rcs));
          rcs.clear();
          num = tmpNum;
        }
      if (num == -1)
        {
          num = tmpNum;
        }
      cv::Rect rc;
      rc.x = toInt(trim(cols[2]));
      rc.y = toInt(trim(cols[3]));
      rc.width = toInt(trim(cols[4]));
      rc.height = toInt(trim(cols[5]));
      rcs.push_back(rc);
    }
  if (!rcs.empty())
    {
      map_rect_.insert(std::make_pair(tmpNum, rcs));
    }
}
void ReadGtFile(const std::string &file)
{

  std::vector<std::string> lines;
  std::ifstream f(file.c_str());
  if (f.is_open())
  {
      std::string line = "";
      while (std::getline(f, line))
          lines.push_back(line);
  }

  std::cout<<"lines size:"<<lines.size()<<std::endl;

  for (int i = 0; i < lines.size(); i++)
    {
      std::vector<std::string> cols;
      splitStr(lines[i], ",", cols);
     vec_gt_.push_back(cols);
    }
  std::cout<<"vec_gt_ size:"<<vec_gt_.size()<<std::endl;
}
// Draw the  bounding box
void DrawDet( float conf, int left, int top, int right, int bottom, Mat& frame)
{
  //Draw a rectangle displaying the bounding box
  rectangle(frame, Point(left, top), Point(right, bottom), Scalar(255, 178, 50), 3);


  string label = format("%.2f", conf);

  //Display the label at the top of the bounding box
  int baseLine;
  Size labelSize = getTextSize(label, FONT_HERSHEY_SIMPLEX, 0.5, 1, &baseLine);
  top = max(top, labelSize.height);
  rectangle(frame, Point(left, top - round(1.5*labelSize.height)), Point(left + round(1.5*labelSize.width), top + baseLine), Scalar(255, 255, 255), FILLED);
  putText(frame, label, Point(left, top), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(0,0,0),1);
}

void DrawGroundTruth(int trackID, float VisibilityRatio, int left, int top, int right, int bottom, Mat& frame)
{
  //Draw a rectangle displaying the bounding box
  rectangle(frame, Point(left, top), Point(right, bottom), Scalar(255, 178, 50), 3);


  string label = format("%.2f", VisibilityRatio);

  label =  std::to_string(trackID)+":" + label;
  //Display the label at the top of the bounding box
  int baseLine;
  Size labelSize = getTextSize(label, FONT_HERSHEY_SIMPLEX, 0.5, 1, &baseLine);
  top = max(top, labelSize.height);
  rectangle(frame, Point(left, top - round(1.5*labelSize.height)), Point(left + round(1.5*labelSize.width), top + baseLine), Scalar(255, 255, 255), FILLED);
  putText(frame, label, Point(left, top), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(0,0,0),1);
}
void  det_show(cv::Mat &frame, int num)
{

  if (map_rect_.empty())
    {
      ReadDetFile(det_file_path_);
    }
  std::vector<cv::Rect> rcs;
  std::map<int, std::vector<cv::Rect>>::iterator it = map_rect_.find(num);
  if (it != map_rect_.end())
    {
      rcs = it->second;
    }

  for (size_t i = 0; i < rcs.size(); ++i)
    {
      //置信度默认用了1，没有取数据
      Rect box = rcs[i];
      DrawDet(1, box.x, box.y,box.x + box.width, box.y + box.height, frame);
    }
  static const string kWinName = "Data visualization";
  namedWindow(kWinName, WINDOW_NORMAL);
  imshow(kWinName, frame);

  cv::waitKey(3);
}

void  gt_show(cv::Mat &frame,int num)
{

  int person_count=0;
  for (size_t i = 0; i < vec_gt_.size(); i++)
    {
      int FrameID =std::atoi(vec_gt_[i][0].c_str());
      if(FrameID == num)
        {
          person_count++;
          int TrackID=std::atoi(vec_gt_[i][1].c_str());
          float VisibilityRatio=std::atof(vec_gt_[i][8].c_str());
          int   bb_left =std::atoi(vec_gt_[i][2].c_str());
          int   bb_top=std::atoi(vec_gt_[i][3].c_str());
          int   bb_width=bb_left+std::atoi(vec_gt_[i][4].c_str());
          int   bb_height=bb_top+std::atoi(vec_gt_[i][5].c_str());
          DrawGroundTruth(TrackID,VisibilityRatio,bb_left,bb_top,bb_width,bb_height,frame);
        }
      if(person_count >=80)break; //目标检测中不仅仅包含了人


    }


  static const string kWinName = "Data visualization";
  namedWindow(kWinName, WINDOW_NORMAL);
  imshow(kWinName, frame);

  cv::waitKey(3);
}

int main(int argc, char** argv)
{


  std::string root = img_dir_;
  int img_count=1050;

  ReadDetFile(det_file_path_);//det data
  ReadGtFile(gt_file_path);//gt data "

  for (int i = 1; i < img_count; i++)
    {

      std::string path = root;
      path += to6dStr(i);
      path += ".jpg";
      cv::Mat mat = cv::imread(path);
    //  det_show(mat, i);  //Det data visualization 输入
      gt_show(mat,i); //GT data visualization 输出
    }

  return 0;

}


