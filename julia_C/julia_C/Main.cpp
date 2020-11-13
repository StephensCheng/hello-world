
#include <Windows.h>
#include<highgui/highgui.hpp>
#include<time.h>

using namespace cv;

const int icount = 200;     //迭代次数

const float c = -0.8;       //实部
const float d = 0.156;      //虚部
double m_real, m_image;     //Mandelbro集

class ComplexClass
{
public:
	double real;
	double image;

	ComplexClass(double r = 0, double i = 0)     //对于初始化复数函数进行预定义，实部和虚部都定义为0
	{
		real = r, image = i;
	}
};

ComplexClass operator+(const ComplexClass& a, const ComplexClass &b)
{
	ComplexClass c;
	c.real = a.real + b.real;
	c.image = a.image + b.image;
	return c;
}

ComplexClass operator*(const ComplexClass& a, const ComplexClass &b)   //需要注意的operator这个函数本身是对于*这个运算进行操作的，将其定义为复数的乘法操作
{
	ComplexClass c;
	c.real = a.real * b.real - a.image * b.image;
	c.image = a.image * b.real + a.real * b.image;
	return c;
}

double Model(ComplexClass a)
{
	return sqrtf(a.real * a.real + a.image * a.image);  //注意此处的细节区别，sqrtf的返回值类型是float
}

double Iteration(ComplexClass a, int n)
{
	if (n == 0)
		return Model(a);                            //表示对于复数a进行首次迭代
	else
	{
		ComplexClass temp = a*a;
		temp.real += c;
		temp.image += d;
		return Iteration(temp, n - 1);
	}
}

Vec3b dye(double dist)                 //Vec3b在这里理解为对特定的点位进行绘图
{
	if (dist < 1000000 && dist>-1000000)
		return Vec3b(255, 0, 0);     //图片集合区域为蓝色
	else
		return Vec3b(0, 0, 0);   //Julia集之外的区域置为黑色
}

int main()
{
	Mat image = Mat(Size(500, 500), CV_8UC3, Scalar::all(10));                        //scalar表示将图片中（500*500）点集合全部设置统一的灰度值 eg：scalar（255，0，0）
	clock_t start_time, end_time;
	start_time = clock();
	for (int Y = 0; Y < image.rows; Y++)
	{
		for (int X = 0; X < image.cols; X++)
		{
			float x = (X - image.cols / 2) / 200.0;
			float y = (Y - image.rows / 2) / 200.0;

			m_real = x;
			m_image = y;
			ComplexClass a(x, y);
			float dist = Iteration(a, icount);
			image.at<Vec3b>(Y, X) = dye(dist);                                        //.at(int y,int x)表示方位对应坐标（x,y）的元素坐标，根据之前的提示在数据集之外的（0，0，0），数据集之内的（255，0，0） PS：Vec3b表示三通道  
		}
	}
	end_time = clock();
	printf("CONSUMING TIME;%fs\n", (double)(end_time - start_time) / CLOCKS_PER_SEC);

	//namedWindow("OpenCV For Julia", 0);
	imshow("OpenCV For Julia | c = -0.8  d = 0.156", image);
	waitKey();
}