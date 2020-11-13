
#include <Windows.h>
#include<highgui/highgui.hpp>
#include<time.h>

using namespace cv;

const int icount = 200;     //��������

const float c = -0.8;       //ʵ��
const float d = 0.156;      //�鲿
double m_real, m_image;     //Mandelbro��

class ComplexClass
{
public:
	double real;
	double image;

	ComplexClass(double r = 0, double i = 0)     //���ڳ�ʼ��������������Ԥ���壬ʵ�����鲿������Ϊ0
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

ComplexClass operator*(const ComplexClass& a, const ComplexClass &b)   //��Ҫע���operator������������Ƕ���*���������в����ģ����䶨��Ϊ�����ĳ˷�����
{
	ComplexClass c;
	c.real = a.real * b.real - a.image * b.image;
	c.image = a.image * b.real + a.real * b.image;
	return c;
}

double Model(ComplexClass a)
{
	return sqrtf(a.real * a.real + a.image * a.image);  //ע��˴���ϸ������sqrtf�ķ���ֵ������float
}

double Iteration(ComplexClass a, int n)
{
	if (n == 0)
		return Model(a);                            //��ʾ���ڸ���a�����״ε���
	else
	{
		ComplexClass temp = a*a;
		temp.real += c;
		temp.image += d;
		return Iteration(temp, n - 1);
	}
}

Vec3b dye(double dist)                 //Vec3b���������Ϊ���ض��ĵ�λ���л�ͼ
{
	if (dist < 1000000 && dist>-1000000)
		return Vec3b(255, 0, 0);     //ͼƬ��������Ϊ��ɫ
	else
		return Vec3b(0, 0, 0);   //Julia��֮���������Ϊ��ɫ
}

int main()
{
	Mat image = Mat(Size(500, 500), CV_8UC3, Scalar::all(10));                        //scalar��ʾ��ͼƬ�У�500*500���㼯��ȫ������ͳһ�ĻҶ�ֵ eg��scalar��255��0��0��
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
			image.at<Vec3b>(Y, X) = dye(dist);                                        //.at(int y,int x)��ʾ��λ��Ӧ���꣨x,y����Ԫ�����꣬����֮ǰ����ʾ�����ݼ�֮��ģ�0��0��0�������ݼ�֮�ڵģ�255��0��0�� PS��Vec3b��ʾ��ͨ��  
		}
	}
	end_time = clock();
	printf("CONSUMING TIME;%fs\n", (double)(end_time - start_time) / CLOCKS_PER_SEC);

	//namedWindow("OpenCV For Julia", 0);
	imshow("OpenCV For Julia | c = -0.8  d = 0.156", image);
	waitKey();
}