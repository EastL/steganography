#include <iostream>
#include <string>
#include <cstdlib>
#include <sndfile.h>
#include "format.h"
#include "bmpread.h"
#include "bmpwrite.h"
#include <fstream>

using std::cout;
using std::endl;
using std::string;

int size, pixel_size;
SF_INFO Info;
short *buffer;
int num_item;
int height, width;

void snd_read(vector<short> &buf, char input[])
{
    SNDFILE *InputAudio;

    InputAudio = sf_open (input, SFM_READ, &Info);
    if (InputAudio == NULL)
        cout << "Failed to open the file !!" << endl;
    
    num_item = Info.frames * Info.channels;
    buffer = (short *)malloc(num_item * sizeof (short));
    size = sf_read_short (InputAudio, buffer, num_item);

    for(int i = 0; i < size; i++)
        buf.push_back(buffer[i]);
    sf_close(InputAudio);
}

void snd_write(vector<short> buf, char output[])
{
    SNDFILE *OutputAudio;
    OutputAudio = sf_open (output, SFM_WRITE, &Info);
    
    for(int i = 0; i < buf.size(); i++)
        buffer[i] = buf[i];
    sf_write_short(OutputAudio, buffer, num_item);
    sf_close(OutputAudio);   
}

void bmp_read(vector<short> &buf, char input[])
{
    LoadBMP bmp(input);
    vector<short> rpix, gpix, bpix;

    for(int i = 0; i < bmp.get_Width(); i++)
        for(int j = 0; j < bmp.get_Height(); j++)
	{
	    rpix.push_back(bmp.get_pixel(i,j)& 0xff);
	    gpix.push_back((bmp.get_pixel(i,j)& 0xff00) >> 8);
	    bpix.push_back((bmp.get_pixel(i,j)& 0xff0000) >> 16);
	}
    pixel_size = rpix.size();
    
    for(int i = 0; i < pixel_size; i++)
        buf.push_back(rpix[i]);
    for(int i = 0; i < pixel_size; i++)
        buf.push_back(gpix[i]);
    for(int i = 0; i < pixel_size; i++)
        buf.push_back(bpix[i]);

    height = bmp.get_Height();
    width = bmp.get_Width();
}

void bmp_write(vector<short> buf, char output[])
{
    SaveBMP new_bmp(output);
    new_bmp.initBMP(width, height);
    vector<short> rpix, gpix, bpix;

    for(int i = 0; i < pixel_size; i++)
        rpix.push_back(buf[i]);
    for(int i = pixel_size; i < 2 * pixel_size; i++)
        gpix.push_back(buf[i]);
    for(int i = pixel_size * 2; i < pixel_size * 3; i++)
        bpix.push_back(buf[i]);

/*    string teststr;
    for(int i = 0; i < rpix.size(); i++)
        teststr.push_back(rpix[i]);

    cout << teststr;
*/
    int pix_dir_count = -1;
    for(int i = 0; i < width; i++)
	for(int j = 0; j < height; j++)
        {
            pix_dir_count++;
            new_bmp.put_pixel(i, j, rpix[pix_dir_count] | (gpix[pix_dir_count] << 8) | (bpix[pix_dir_count] << 16));
        }

    new_bmp.Save();
}
