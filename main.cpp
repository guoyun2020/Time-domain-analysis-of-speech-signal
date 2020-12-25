// author:郭赟 version:1.0 time:12/23/20
#include <iostream>
#include <fstream>
#include <string.h>
#include<math.h>
#include<cmath>
#include<stdlib.h>
#include <bitset>
#include <iomanip>
#include<vector>

using namespace std;

/*@ 符号函数sgn()
@ m 输入参数
*/
int sgn(float m) {
	if (m >= 0) return 1;
	else return -1;
}

/*@ chunk结构体(RIFF的基本单元)
@ file_size		 文件大小
@ channel		 通道数
@ frequency		 采样频率
@ Bps			 Byte率
@ sample_num_bit 一个样本的位数
@ data_size		 数据大小
@ data		     音频数据
*/
struct wav_struct
{
	unsigned long file_size;        //文件大小
	unsigned short channel;            //通道数
	unsigned long frequency;        //采样频率
	unsigned long Bps;                //Byte率
	unsigned short sample_num_bit;    //一个样本的位数
	unsigned long data_size;        //数据大小
	unsigned char *data;            //音频数据 
};


int main(int argc, char **argv)
{
	fstream fs;
	wav_struct WAV;

	//一段采样频率8000Hz,采样精度16bit，时长1000ms的wav音频
	fs.open("D:\\test2.wav", ios::binary | ios::in);


	fs.seekg(0, ios::end);						  //获得文件大小
	WAV.file_size = fs.tellg();

	fs.seekg(0x14);								  //通道数
	fs.read((char*)&WAV.channel, sizeof(WAV.channel));

	fs.seekg(0x18);							      //采样频率
	fs.read((char*)&WAV.frequency, sizeof(WAV.frequency));

	fs.seekg(0x1c);								  //Byte率
	fs.read((char*)&WAV.Bps, sizeof(WAV.Bps));

	fs.seekg(0x22);								  //一个样本的位数
	fs.read((char*)&WAV.sample_num_bit, sizeof(WAV.sample_num_bit));

	fs.seekg(0x28);								  //数据大小
	fs.read((char*)&WAV.data_size, sizeof(WAV.data_size));

	WAV.data = new unsigned char[WAV.data_size];  


	fs.seekg(0x2c);								  //音频数据 
	fs.read((char *)WAV.data, sizeof(char)*WAV.data_size);

	cout << "文件大小为  ：" << WAV.file_size << endl;
	cout << "音频通道数  ：" << WAV.channel << endl;
	cout << "采样频率    ：" << WAV.frequency << endl;
	cout << "Byte率      ：" << WAV.Bps << endl;
	cout << "样本位数    ：" << WAV.sample_num_bit << endl;
	cout << "音频数据大小：" << WAV.data_size << endl;

	//第n帧语音信号Xn(m)
	vector<float> Xm;
	vector<vector<float>> Xn;

	//定义窗函数(矩形窗)
	//帧长=20ms，帧移=0
	//帧数=1000ms/20ms=50帧
	//每帧样本数=16000/50=320

	int num = 0;
	for (unsigned long i = 0; i < WAV.data_size; i = i + 2)
	{
		num++;
		//右边为大端
		unsigned long data_low = WAV.data[i];
		unsigned long data_high = WAV.data[i + 1];
		double data_true = data_high * 256 + data_low;
		//printf("%d ",data_true);
		long data_complement = 0;
		//取大端的最高位（符号位）
		int my_sign = (int)(data_high / 128);

		if (my_sign == 1)
		{
			data_complement = data_true - 65536;
		}
		else
		{
			data_complement = data_true;
		}
		//printf("%d ", data_complement);
		setprecision(4);
		double float_data = (double)(data_complement / (double)32768);
		//printf("%f ", float_data);

		//分帧与加窗
		Xm.push_back(float_data);
		if (num == 320) {
			Xn.push_back(Xm);
			Xm.clear();
			num = 0;
		}         
	}
	fs.close();
	delete[] WAV.data;
	int n;
	cout << "请输入计算帧数(0~49)：" << endl;
	cin >> n;

	//计算第n帧短时能量
	float energy = 0;
	for (int i = 0; i < 320; i++) {
		energy += Xn[n][i] * Xn[n][i];
	}
	cout << "短时能量：" << energy << endl;

	//计算第n帧短时平均幅度
	float result = 0;
	for (int m = 0; m < 320; m++) {
		result += abs(Xn[n][m]);
	}
	cout << "短时平均幅度：" << result / 320 << endl;

	//计算第n帧短时过零率
	float result1 = 0;
	for (int k = 1; k < 320; k++) {
		result1 += abs(sgn(Xn[n][k]) - sgn(Xn[n][k - 1]));
	}
	cout << "短时过零率：" << result1 << endl;

	system("pause");
	return 0;
}







