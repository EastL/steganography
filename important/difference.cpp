#include <iostream>
#include <string>
#include <fstream>

using std::cout;
using std::endl;
using std::string;
using std::ifstream;
using std::ios;

string Tobinary(string str);

int main(int argc, char **argv)
{
    string str1, str2, temp;
    
    ifstream file(argv[1], ios::in);
    while(getline(file, temp))
        str1 += temp;
    file.close();
  
    ifstream file1(argv[2], ios::in);
    while(getline(file1, temp))
        str2 += temp;
    file1.close();

    string bstr1 = Tobinary(str1), bstr2 = Tobinary(str2);
    int sum(0);
    for(int i = 0; i < bstr1.size(); i++)
        if(bstr1[i] != bstr2[i])
            sum++;

    double percent = (sum + 0.0) / bstr1.size();

    cout << percent << endl;
    
    return 0;
}

string Tobinary(string str)
{
    string binary;
    for(int i = 0; i < str.size(); i++)
    {
        bool temp[8] = {0, 0, 0, 0, 0, 0, 0, 0};
        int make = str[i];
        int j = 8;
        while(make >= 2)
        {
            j--;
            temp[j] = make % 2;
            make /= 2;
        }
        temp[j-1] = make;
        
        for(int k = 0; k < 8; k++)
            binary.push_back(temp[k]);
    }

    for(int i = 0; i < binary.size(); i++)
        binary[i] += 48;
    return binary;
}
