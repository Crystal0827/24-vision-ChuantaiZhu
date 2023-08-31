#include<iostream>
#include<opencv2/opencv.hpp>
#include<cmath>
#include<vector>
using namespace std;
using namespace cv;
#define pi 3.1415926535
int main()
{
      VideoCapture video("rune-detect.avi");
      static int bool_1 = 1;
      RotatedRect r1;//R标
     while(1)
      {
Mat img,mask,imgHSV,dilate,struct0,erode;
        video >> img;
        int Hmin =0,Smin = 43,Vmin = 46,Hmax = 25,Smax = 255,Vmax = 255;
      cvtColor(img,imgHSV,COLOR_BGR2HSV);
    Scalar lower(Hmin,Smin,Vmin);
   Scalar upper(Hmax,Smax,Vmax);
   inRange(imgHSV,lower,upper,mask);
   struct0 = getStructuringElement(0,Size(3,3));
    cv::dilate(mask,dilate,struct0,Point(-1,-1),2);
vector<vector<Point>>contours1,contours2;
vector<Vec4i>hierarchy1,hierarchy2;
cv::findContours(mask,contours1,hierarchy1,3,3);
cv::findContours(dilate,contours2,hierarchy2,3,3);
RotatedRect r2;
for(int i =0;i<contours1.size();i++)
{
  double area = contourArea(contours1[i]);
  if(area>205&&area<230){
    RotatedRect r3 = minAreaRect(contours1[i]);
    if(r3.size.width<r3.size.height)
    {
      float t = r3.size.width;
      r3.size.width= r3.size.height;
      r3.size.height = t;
    };
    if((r3.size.width/r3.size.height)<1.2&&(bool_1==1)){
bool_1 = 0;
r1 = r3;
    }
  }
}
circle(img,r1.center,8,Scalar(255,255,0),-1);

//********************************************

static vector<Point2f>miss(contours2.size());
for(int i = 0;i<contours2.size();i++)
{
  double area = contourArea(contours2[i]);
  if(area>7000&&area<15000){
RotatedRect r2 = minAreaRect(contours2[i]);
  float width = r2.size.width;
  float height = r2.size.height;
  Point2f points[4];
  r2.points(points);
  if(width<height) 
  { float t = height;
  height = width;
  width = t;
  };
  float ratio = width/height;
  if(ratio>1.7&&area>10000)
  {
     for(int i =0;i<4;i++)
      {
        if(i==3){
          line(img,points[i],points[0],Scalar(255,0,0),2,8,0);
          break;
        }
        line(img,points[i],points[i+1],Scalar(255,0,0),2,8,0);
      }
  }
  else if(ratio<1.7&&ratio>0.8)
  {
    for(int i =0;i<4;i++)
      {
        if(i==3){
          line(img,points[i],points[0],Scalar(255,0,0),2,8,0);
          break;
        }
        line(img,points[i],points[i+1],Scalar(255,0,0),2,8,0);
      }
  }
  else if(ratio>1.7&&area<10000){
    miss[i] = r2.center;
    for(int i =0;i<4;i++)
      {
        
        if(i==3){
          line(img,points[i],points[0],Scalar(0,255,0),2,8,0);
          break;
        }
        line(img,points[i],points[i+1],Scalar(0,255,0),2,8,0);
      }
  }
  }
}

//*******************************************

Mat dilate1,dilate2,struct1,struct2;
struct1 = getStructuringElement(2,Size(3,3));
struct2 = getStructuringElement(2,Size(5,5));
cv::dilate(mask,dilate1,struct1,Point(-1,-1),1);
    cv::erode(dilate1,erode,struct2,Point(-1,-1),1);
    cv::dilate(erode,dilate2,struct1,Point(-1,-1),1);
    vector<vector<Point>>contours;
vector<Vec4i>hierarchy;
cv::findContours(dilate2,contours,hierarchy,3,3);
static vector<Point2f>struck(contours.size());
for(int i =0;i<contours.size();i++)
{
   double area = contourArea(contours[i]);
   /*double length = arcLength(contours[i],true);
   double length = arcLength(contours[i],true);
   double ratio1 = area/length;
   double ratio2 = 0.5*sqrt(area/pi);*/
   if(area>120&&area<2000)
   {
    RotatedRect r = minAreaRect(contours[i]);
    if(r.size.width<r.size.height)
    {
      float t = r.size.width;
      r.size.width= r.size.height;
      r.size.height = t;
    };
    float ratio3 = r.size.width/r.size.height;
    if(ratio3<1.25)
    {
struck[i] = r.center;
circle(img,r.center,4,Scalar(255,0,0),-1);
    }
   }
}
vector<double>distance(miss.size()*struck.size());
for(int a =0;a<miss.size();a++)
{
  if((miss[a].x!= 0)&&(miss[a].y!= 0))
  {
int min =0;
for(int i = 0;i<struck.size();i++)
{
  if((struck[i].x!= 0)&&(struck[i].y!= 0))
  {
distance[a*struck.size()+i] = sqrt((miss[a].x-struck[i].x)*(miss[a].x-struck[i].x)+(miss[a].y-struck[i].y)*(miss[a].y-struck[i].y));
} 
  }
  
  for(int i = 0;i<struck.size();i++)
  {
    if(distance[a*struck.size()+i]==0)
    {
      distance[a*struck.size()+i]=999999999999999;
    }
  if(distance[a*struck.size()+min]>=distance[a*struck.size()+i])
  {
    min = i;
  }
  }
circle(img,struck[min],6,Scalar(0,255,0),-1);
  struck[min].x = 0;
  struck[min].y = 0;
  }
}
/*for(int i = 0;i<struck.size();i++)
{
  if((struck[i].x!= 0)&&(struck[i].y!= 0))
  {
    circle(img,struck[i],4,Scalar(255,0,0),-1);
  }
}*/
for(int i = 0;i<struck.size();i++)
{
  if((struck[i].x!= 0)&&(struck[i].y!= 0))
  {
    struck[i].x = struck[i].y = 0;
  }
}
for(int i = 0;i<miss.size();i++)
{
  if((miss[i].x!= 0)&&(miss[i].y!= 0))
  {
    miss[i].x = miss[i].y = 0;
  }
}
        cv::imshow("video",img);
        waitKey(1000/video.get(CAP_PROP_FPS));
}

      }
     
 
  

