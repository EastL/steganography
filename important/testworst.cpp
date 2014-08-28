#include <iostream>
#include <stdlib.h>

using namespace std;

int main()
{
    int better[8][8] = { { 0, 28, 42, 49, 54, 45, 27,  7},
                         {32, 60, 10, 17, 22, 13, 59, 39},
                         {16, 12, 58, 33, 38, 61, 11, 23},
                         { 8, 20, 34, 57, 62, 37, 19, 15},
                         { 4, 24, 46, 53, 50, 41, 31,  3},
                         { 2, 30, 40, 51, 52, 47, 25,  5},
                         { 1, 29, 43, 48, 55, 44, 26,  6},
                         {36, 56, 14, 21, 18,  9, 63, 35}
                       };

    int max, min;
    for(int c = 0; c < 64; c++)
    {
        max = 0;
        for(int i = 0; i < 8; i++)
        {
            min = 64;
            for(int j = 0; j < 8; j++)
                if(abs(c - better[i][j]) < min)
                    min = abs(c - better[i][j]);
            if(min > max)
                max = min;
        }
        if(c < 10)
            cout << ' ' << c;
        else cout << c;
        cout << " : " << max << '\t' << '\t';
        if((c+1) % 5 == 0 && c != 0)
            cout << endl;
    }
    cout << endl;
    return 0;
}
