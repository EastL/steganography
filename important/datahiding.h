#include <string>
#include <vector>
#include <complex>
#define header_size 32

using std::string;
using std::vector;
using std::complex;

string short2bin(short n);  //short to binary
short bin2short(string str);//binary to short
string Tobinary(string str);//secret message convert into binary string
int classify(int data);    //pvd interval
string set_header(string str);//calculate message size and insert information into first n bits

//difference way matrix embedding
short get_better(short num, string s);
//pvd based, can insert lsb and metrix_embbeding
void pvd(vector<short> &buffer, string str, short format, short method);
//single data hide n bit message by lsb
short lsb(short cover, string message);
//singal data use last 6 bit to hide message by matrix embbeding
short matrix_embbeding(short cover, string message);

string retrieve_pvd(vector<short> buffer, short format, short method);

complex<short> *DFT(short *input, int size);//discrete fourier transform 

vector<short> IDFT(complex<short> *input, int size, vector<short> &buffer);//inverse discrete fourier transform

void DFT_LSB(vector<short> &buffer, string str);
