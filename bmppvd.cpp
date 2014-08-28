#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "read.h"
#include "write.h"
#include <vector>
#include <math.h>
#include <string>

using namespace std;

struct pixel
{
	unsigned char red;
	unsigned char green;
	unsigned char blue;
};

int classify(int data)
{
	if(data >=0 && data <= 7)
		return 1;
	if(data >=8 && data <= 15)
		return 2;
	if(data >=16 && data <= 31)
		return 3;
	if(data >=32 && data <= 63)
		return 4;
	if(data >=64 && data <= 127)
		return 5;
	if(data >=128 && data <= 255)
		return 6;
}

void PVD(string pvd_message, vector<unsigned char> &pix_dir, bool type)
{
	//char to binary
	vector<bool> binary_message;
	for(int i = 0; i < pvd_message.size(); i++)
	{
		bool temp[8] = {0, 0, 0, 0, 0, 0, 0, 0};
		int make = pvd_message[i];
		int index = 8;
		while (make >= 2)
		{
			index--;
			temp[index] = make % 2;
			make /= 2;
		}
		temp[index - 1] = make;

		for(int j = 0; j < 8; j++)
			binary_message.push_back(temp[j]);
	}
	/*for (int i = 0; i < binary_message.size(); i++)
		cout << binary_message[i] << ' ';
	cout << endl;*/

	if(type)
	{
		//把訊息大小藏進第一個像素差值
		int tdiff = pix_dir[1] - pix_dir[0];
		int NewOldDiff = pvd_message.size() - tdiff;
			if(NewOldDiff % 2 == 0)
			{
				pix_dir[0] -= NewOldDiff / 2;
				pix_dir[1] += NewOldDiff / 2;
			}
			else
			{
				if( NewOldDiff > 0)
				{
					pix_dir[0] -= NewOldDiff / 2 + 1;
					pix_dir[1] += NewOldDiff / 2;
				}
				else
				{
					pix_dir[0] -= NewOldDiff / 2 - 1;
					pix_dir[1] += NewOldDiff / 2;
				}
			}
	}
	//start pvd
	int BinMessCount = 0; // 記錄binary message位置
	//像素兩兩一組
	for(int i = 2; i < pix_dir.size(); i+=2)
	{
		bool end = 0;
		//計算差值
		int diff = pix_dir[i+1] - pix_dir[i], classify_group;
		if(diff < 0)
			classify_group = -diff;
		else
			classify_group = diff;
		//依照差值分類
		int K = classify(classify_group);
		int BIT, Lk;
		switch(K)
		{
			case 1:
				BIT = 3;
				Lk = 0;
				break;
			case 2:
				BIT = 3;
				Lk = 8;
				break;
			case 3:
				BIT = 4;
				Lk = 16;
				break;
			case 4:
				BIT = 5;
				Lk = 32;
				break;
			case 5:
				BIT = 6;
				Lk = 64;
				break;
			case 6:
				BIT = 7;
				Lk = 128;
				break;
		}

		//將對應的bit數轉成10進位
		int dec/*存取轉好的10進位訊息*/, nowBMC = BinMessCount;
		vector<bool> tempBinMes;//暫時存取2進位的訊息

		//檢查是否為最後一筆藏入資訊(須做特殊處理)
		int remain = binary_message.size() - BinMessCount;
		if(remain < BIT)
		{
			for(;BinMessCount < binary_message.size(); BinMessCount++)
				tempBinMes.push_back(binary_message[BinMessCount]);
			end = 1;
		}
		else
			for(;BinMessCount < nowBMC + BIT; BinMessCount++)
				tempBinMes.push_back(binary_message[BinMessCount]);
		
		/*for(int j = 0; j < tempBinMes.size(); j++)
			cout << tempBinMes[j];
		cout << endl;*/
		//BinMessCount++;//讓下一次從還未藏入的位置開始

		int sum = 0;
		for(int j = 0; j < tempBinMes.size(); j++)
			sum += pow(2,tempBinMes.size() - 1.0 - j) * tempBinMes[j];		//將2進為轉為10進位(2^n * k)
		dec = sum + Lk;	//加上該區間的下限值
		//cout << sum << endl;
		if(diff < 0)
			dec = -dec;
		//cout << (int)pix_dir[i] << ' ' << (int)pix_dir[i+1] << endl;
		//平均分配到像素質上
		int NewOldDiff = dec - diff;
		int forward = pix_dir[i], back = pix_dir[i+1];

		if(NewOldDiff % 2 ==0)
		{
			forward -= NewOldDiff / 2;
			back += NewOldDiff / 2;
	
			if(forward < 0)
			{
				back -= forward;
				forward = 0;
			}
	
			if(forward > 255)
			{
				back -= forward - 255;
				forward = 255;
			}

			if(back < 0)
			{
				forward -= back;
				back = 0;
			}

			if(back > 255)
			{
				forward -= back - 255;
				back = 255;
			}	
		}

		else
		{
			if(NewOldDiff >= 0)
			{
				forward -= NewOldDiff / 2 + 1;
				back += NewOldDiff / 2;

				if(forward < 0)
				{
					back -= forward;
					forward = 0;
				}
	
				if(forward > 255)
				{
					back -= forward - 255;
					forward = 255;
				}

				if(back < 0)
				{
					forward -= back;
					back = 0;
				}

				if(back > 255)
				{
					forward -= back - 255;
					back = 255;
				}		
			}

			else
			{
				forward -= NewOldDiff / 2 - 1;
				back += NewOldDiff / 2;

				if(forward < 0)
				{
					back -= forward;
					forward = 0;
				}
	
				if(forward > 255)
				{
					back -= forward - 255;
					forward = 255;
				}

				if(back < 0)
				{
					forward -= back;
					back = 0;
				}

				if(back > 255)
				{
					forward -= back - 255;
					back = 255;
				}
			}
		}

		pix_dir[i] = forward;
		pix_dir[i+1] = back;

		//test
		//cout << (int)pix_dir[i] << ' ' << (int)pix_dir[i+1] << endl;
		if(end)
			break;
	}
}
void hidemessage(char str2[], char str3[])
{
	//read
		LoadBMP bmp(str2);
	
		int pix_size = bmp.get_Width() * bmp.get_Height() * 3;
		
		vector<unsigned char> rpix_dir, bpix_dir, gpix_dir;
		for(int i = 0; i < bmp.get_Width(); i++)
			for(int j = 0; j < bmp.get_Height(); j++)
			{
				rpix_dir.push_back(bmp.get_pixel(i,j)& 0xff);
				bpix_dir.push_back((bmp.get_pixel(i,j)& 0xff00) >> 8);
				gpix_dir.push_back(bmp.get_pixel(i,j)& 0xff0000);
			}
		//work
		string pvd_message;
		cout << "Please input the secret message:";
		getline(cin, pvd_message);
		cout << endl;
		//pvd steganography
		
		//check 
		int message_size = pvd_message.size() * 8;
		int pic_size = bmp.get_Width() * bmp.get_Height() * 9;
		if(message_size > pic_size)
		{
			cout << "message 檔案太大\n";
			exit(0);
		}
	
		PVD(pvd_message, rpix_dir, 1);
		

		/*//1D→2D
		int pix_dir_count = -1;
		for(int i = 0; i < bmp.get_Width(); i++)
			for(int j = 0; j < bmp.get_Height(); j++)
			{
				pix[i][j].red = pix_dir[++pix_dir_count];
				//pix[i][j].green = pix_dir[++pix_dir_count];
				//pix[i][j].blue = pix_dir[++pix_dir_count];
			}
*/
		//write
		SaveBMP new_bmp(str3);
		new_bmp.initBMP(bmp.get_Width(), bmp.get_Height());

		int pix_dir_count = -1;
		for(int i = 0; i < bmp.get_Width(); i++)
			for(int j = 0; j < bmp.get_Height(); j++)
                        {
                                pix_dir_count++;
				new_bmp.put_pixel(i, j, rpix_dir[pix_dir_count] | (gpix_dir[pix_dir_count] << 8) | (bpix_dir[pix_dir_count] << 16));
                        }

		new_bmp.Save();
}

void getmessage(char str2[])
{
	//read
		LoadBMP bmp(str2);
	
		vector<vector<pixel>> pix;
		for (unsigned int i = 0 ; i < bmp.get_Width(); i++)
		{
			vector<pixel> temp;
			for (unsigned int j = 0 ; j < bmp.get_Height(); j++)
			{
				pixel t_pixel;
				t_pixel.red = (bmp.get_pixel(i,j)& 0xff);
				t_pixel.green = (bmp.get_pixel(i,j)& 0xff00) >> 8;
				t_pixel.blue = (bmp.get_pixel(i,j)& 0xff0000) >> 16;
				temp.push_back(t_pixel);
			}
			pix.push_back(temp);
		}

		//2D→1D

		int pix_size = bmp.get_Width() * bmp.get_Height() * 3;

		vector<unsigned char> pix_dir;
		for(int i = 0; i < bmp.get_Width(); i++)
			for(int j = 0; j < bmp.get_Height(); j++)
			{
				pix_dir.push_back(pix[i][j].red);
				//pix_dir.push_back(pix[i][j].green);
				//pix_dir.push_back(pix[i][j].blue);
			}
		vector<bool> binary_message;
		int mes_size = pix_dir[1] - pix_dir[0];
		string message;
		int begin = 0;
		for(int i = 2; i < pix_dir.size(); i += 2)
		{
			//計算差值
			int diff = pix_dir[i+1] - pix_dir[i], classify_group;
			if(diff < 0)
				classify_group = -diff;
			else
				classify_group = diff;
			//依照差值分類
			int K = classify(classify_group);
			int BIT, Lk;
			switch(K)
			{
				case 1:
					BIT = 3;
					Lk = 0;
					break;
				case 2:
					BIT = 3;
					Lk = 8;
					break;
				case 3:
					BIT = 4;
					Lk = 16;
					break;
				case 4:
					BIT = 5;
					Lk = 32;
					break;
				case 5:
					BIT = 6;
					Lk = 64;
					break;
				case 6:
					BIT = 7;
					Lk = 128;
					break;
			}
			//將十進位差值轉為2進位表示
			bool temp[8] = {0, 0, 0, 0, 0, 0, 0, 0};
			int index = 8, make = classify_group - Lk;
			while (make >= 2)
			{
				index--;
				temp[index] = make % 2;
				make /= 2;
			}
			temp[index - 1] = make;

			//將背嵌入的訊息量如果嵌入後大於原本訊息量的話，代表已經該結束了
			if(BIT >= mes_size * 8 - binary_message.size())
			{
				for(int j = 8 - (mes_size * 8 - binary_message.size()); j < 8; j++)
					binary_message.push_back(temp[j]);
				for(int j = 0; j < binary_message.size(); j++)	//將隱藏訊息的2進位資訊表示出來
					cout << binary_message[j] << ' ';
				cout << endl;
				while (message.size() < mes_size)	//經由ASCII轉為string
				{
					int sum = 0;
					for(int j = begin, k = 7; j < begin + 8 && k >= 0; j ++, k--)
						sum += binary_message[j] * pow(2.0, k);
					message.push_back(sum);
					begin += 8;
				}
				
				break;
			}
			else
			{
				for(int j = 8 - BIT; j < 8; j++)
					binary_message.push_back(temp[j]);
			}
		}
		for(int i = 0; i < message.size(); i++)
			cout << message[i];
		cout << endl;
}

void hidepicture(char str2[], char str3[])
{
	LoadBMP bmp(str3);
	vector<vector<pixel>> pix;
	for (int i = 0 ; i < bmp.get_Width(); i++)
	{
		vector<pixel> temp;
		for (int j = 0 ; j < bmp.get_Height(); j++)
		{
			pixel t_pixel;
			t_pixel.red = (bmp.get_pixel(i,j)& 0xff);
			t_pixel.green = (bmp.get_pixel(i,j)& 0xff00) >> 8;
			t_pixel.blue = (bmp.get_pixel(i,j)& 0xff0000) >> 16;
			temp.push_back(t_pixel);
		}
		pix.push_back(temp);
	}

	//2D→1D

	long long pix_size = bmp.get_Width() * bmp.get_Height() * 3 / 2;

	vector<unsigned char> rpic, gpic, bpic;
	for(int i = 0; i < bmp.get_Width(); i++)
		for(int j = 0; j < bmp.get_Height(); j++)
		{
			rpic.push_back(pix[i][j].red);
			gpic.push_back(pix[i][j].green);
			bpic.push_back(pix[i][j].blue);
		}


	LoadBMP hbmp(str2);
	vector<vector<pixel>> hpix;
	for (int i = 0 ; i < hbmp.get_Width(); i++)
	{
		vector<pixel> temp;
		for (int j = 0 ; j < hbmp.get_Height(); j++)
		{
			pixel t_pixel;
			t_pixel.red = (hbmp.get_pixel(i,j)& 0xff);
			t_pixel.green = (hbmp.get_pixel(i,j)& 0xff00) >> 8;
			t_pixel.blue = (hbmp.get_pixel(i,j)& 0xff0000) >> 16;
			temp.push_back(t_pixel);
		}
		hpix.push_back(temp);
	}

	//2D→1D

	long long hpix_size = hbmp.get_Width() * hbmp.get_Height() * 8 ;

	string rpix_dir, gpix_dir, bpix_dir;
	for(int i = 0; i < hbmp.get_Width(); i++)
		for(int j = 0; j < hbmp.get_Height(); j++)
		{
			rpix_dir.push_back(hpix[i][j].red);
			gpix_dir.push_back(hpix[i][j].green);
			bpix_dir.push_back(hpix[i][j].blue);
		}

	if(pix_size < hpix_size)
	{
		cout << "請找小一點的圖片來藏訊息XD" << endl;
		exit(0);
	}
	
	PVD(rpix_dir, rpic, 0);
	PVD(bpix_dir, bpic, 0);
	PVD(gpix_dir, gpic, 0);

	//1D→2D
	long long pix_dir_count = 0;
	for(int i = 0; i < bmp.get_Width(); i++)
		for(int j = 0; j < bmp.get_Height(); j++)
		{
			pix[i][j].red = rpic[pix_dir_count];
			pix[i][j].green = gpic[pix_dir_count];
			pix[i][j].blue = bpic[pix_dir_count];
			pix_dir_count ++;
		}

	//write
	SaveBMP new_bmp(str3);
	new_bmp.initBMP(bmp.get_Width(), bmp.get_Height());

	for(int i = 0; i < bmp.get_Width(); i++)
		for(int j = 0; j < bmp.get_Height(); j++)
			new_bmp.put_pixel(i, j, pix[i][j].red | (pix[i][j].green << 8) | (pix[i][j].blue << 16));

	new_bmp.Save();
}

int main(int argc, char **argv )
{
	if(argv[1][0] == '-' && argv[1][1] == 'h')	//此指令代表隱藏訊息
		hidemessage(argv[2], argv[3]);

/*	if(argv[1][0] == '-' && argv[1][1] == 'x')	//此指令代表解開訊息
		getmessage(argv[2]);
	
	if(argv[1][0] == '-' && argv[1][1] == 'p' && argv[1][2] == 'h')	//此指令代表解開訊息
		hidepicture(argv[2], argv[3]);
*/	return 0;
}
