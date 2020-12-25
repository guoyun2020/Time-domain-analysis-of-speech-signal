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

/*@ ���ź���sgn()
@ m �������
*/
int sgn(float m) {
	if (m >= 0) return 1;
	else return -1;
}

/*@ chunk�ṹ��(RIFF�Ļ�����Ԫ)
@ file_size		 �ļ���С
@ channel		 ͨ����
@ frequency		 ����Ƶ��
@ Bps			 Byte��
@ sample_num_bit һ��������λ��
@ data_size		 ���ݴ�С
@ data		     ��Ƶ����
*/
struct wav_struct
{
	unsigned long file_size;        //�ļ���С
	unsigned short channel;            //ͨ����
	unsigned long frequency;        //����Ƶ��
	unsigned long Bps;                //Byte��
	unsigned short sample_num_bit;    //һ��������λ��
	unsigned long data_size;        //���ݴ�С
	unsigned char *data;            //��Ƶ���� 
};


int main(int argc, char **argv)
{
	fstream fs;
	wav_struct WAV;

	//һ�β���Ƶ��8000Hz,��������16bit��ʱ��1000ms��wav��Ƶ
	fs.open("D:\\test2.wav", ios::binary | ios::in);


	fs.seekg(0, ios::end);						  //����ļ���С
	WAV.file_size = fs.tellg();

	fs.seekg(0x14);								  //ͨ����
	fs.read((char*)&WAV.channel, sizeof(WAV.channel));

	fs.seekg(0x18);							      //����Ƶ��
	fs.read((char*)&WAV.frequency, sizeof(WAV.frequency));

	fs.seekg(0x1c);								  //Byte��
	fs.read((char*)&WAV.Bps, sizeof(WAV.Bps));

	fs.seekg(0x22);								  //һ��������λ��
	fs.read((char*)&WAV.sample_num_bit, sizeof(WAV.sample_num_bit));

	fs.seekg(0x28);								  //���ݴ�С
	fs.read((char*)&WAV.data_size, sizeof(WAV.data_size));

	WAV.data = new unsigned char[WAV.data_size];  


	fs.seekg(0x2c);								  //��Ƶ���� 
	fs.read((char *)WAV.data, sizeof(char)*WAV.data_size);

	cout << "�ļ���СΪ  ��" << WAV.file_size << endl;
	cout << "��Ƶͨ����  ��" << WAV.channel << endl;
	cout << "����Ƶ��    ��" << WAV.frequency << endl;
	cout << "Byte��      ��" << WAV.Bps << endl;
	cout << "����λ��    ��" << WAV.sample_num_bit << endl;
	cout << "��Ƶ���ݴ�С��" << WAV.data_size << endl;

	//��n֡�����ź�Xn(m)
	vector<float> Xm;
	vector<vector<float>> Xn;

	//���崰����(���δ�)
	//֡��=20ms��֡��=0
	//֡��=1000ms/20ms=50֡
	//ÿ֡������=16000/50=320

	int num = 0;
	for (unsigned long i = 0; i < WAV.data_size; i = i + 2)
	{
		num++;
		//�ұ�Ϊ���
		unsigned long data_low = WAV.data[i];
		unsigned long data_high = WAV.data[i + 1];
		double data_true = data_high * 256 + data_low;
		//printf("%d ",data_true);
		long data_complement = 0;
		//ȡ��˵����λ������λ��
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

		//��֡��Ӵ�
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
	cout << "���������֡��(0~49)��" << endl;
	cin >> n;

	//�����n֡��ʱ����
	float energy = 0;
	for (int i = 0; i < 320; i++) {
		energy += Xn[n][i] * Xn[n][i];
	}
	cout << "��ʱ������" << energy << endl;

	//�����n֡��ʱƽ������
	float result = 0;
	for (int m = 0; m < 320; m++) {
		result += abs(Xn[n][m]);
	}
	cout << "��ʱƽ�����ȣ�" << result / 320 << endl;

	//�����n֡��ʱ������
	float result1 = 0;
	for (int k = 1; k < 320; k++) {
		result1 += abs(sgn(Xn[n][k]) - sgn(Xn[n][k - 1]));
	}
	cout << "��ʱ�����ʣ�" << result1 << endl;

	system("pause");
	return 0;
}







