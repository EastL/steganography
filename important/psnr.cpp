#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string>
#include <vector>
#include "bmpread.h"
#include "format.h"

using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::vector;

int main(int argc, char **argv)
{
    int sum = 0;
    vector<short> bmp, compare;  //two bmp file

    LoadBMP bfile(argv[1]);     //for get height and width
    int NR = bfile.get_Height();
    int NC = bfile.get_Width();
    
    bmp_read(bmp, argv[1]);     //read to string
    bmp_read(compare, argv[2]);

    int size = bmp.size(), sub;

    for(int i = 0; i < size; i++)   //take every pixel calculate sum of square
    {
        sub = bmp[i] - compare[i];
        sum += pow(sub, 2);
    }

    double psnr = 0, mean = sum / (NR * NC);

    psnr = 10 * log10((255 * 255) / mean);

    cout << psnr << endl;

    return 0;
}
