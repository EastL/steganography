#include <vector>

using std::vector;

void snd_read(vector<short> &buf, char input[]);   //read wav file
void snd_write(vector<short> buf, char output[]);  //write wav file
void bmp_read(vector<short> &buf, char input[]);   //read bmp file
void bmp_write(vector<short> buf, char output[]);  //wrrite bmp file
