#include <iostream>
#include <string.h>
#include <cstdlib>
#include <stdio.h>
#include "format.h"
#include "datahiding.h"

using std::cin;
using std::cout;
using std::endl;
using std::string;

int main (int argc, char **argv)
{
    short final = strlen(argv[2]) - 1; // determine format(final word)
    short format(0), method(0);
    vector<short> buffer;
    
    //all format
    if(argv[2][final-2] == 'b' && argv[2][final-1] == 'm' && argv[2][final] == 'p')
        format = 1;
    else if(argv[2][final-2] == 'w' && argv[2][final-1] == 'a' && argv[2][final] == 'v')
        format = 2;

    //steganographic method
    if(argv[argc-1][0] == 'p' && argv[argc-1][1] == 'l')
        method = 1;
    if(argv[argc-1][0] == 'p' && argv[argc-1][1] == 'm')
        method = 2;

    if(!format)
    {
        cout << "Sorry, we can't support this format." << endl;
        exit(0);
    }
    else
    {
        //read format
        if(format == 1)
            bmp_read(buffer, argv[2]);
        if(format == 2)
            snd_read(buffer, argv[2]);

        //Hide message
        if(argv[1][0] == '-' && argv[1][1] == 'h' && argv[1][2] == 'm')
        {
            string message;
            cout << "please input the message:\n";
            getline(cin, message);
            pvd(buffer, message, format, method);

            if(format == 1)
                bmp_write(buffer, argv[3]);
            if(format == 2)
                snd_write(buffer, argv[3]);
        }

        //Hide file
        else if(argv[1][0] == '-' && argv[1][1] == 'h' && argv[1][2] == 'f')
        {
            string str;
            char temp;
            FILE *file;
            file = fopen(argv[3], "rb");
  
            while((temp = fgetc(file)) != EOF)
                str += temp;
            pvd(buffer, str, format, method);
            fclose(file);

            if(format == 1)
                bmp_write(buffer, argv[4]);
            if(format == 2)
                snd_write(buffer, argv[4]);
        }

        //retrieve message
        else if(argv[1][0] == '-' && argv[1][1] == 'x')
        {
            string x_message;

            if(format == 1)
                bmp_read(buffer, argv[2]);
            if(format == 2) 
                snd_read(buffer, argv[2]);

            x_message = retrieve_pvd(buffer, format, method);

            cout << x_message << endl;
        }
    }
    return 0;
}
