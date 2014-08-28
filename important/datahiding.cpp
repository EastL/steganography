#include <iostream>
#include <stdlib.h>
#include <math.h>
#include "datahiding.h"

using std::cin;
using std::cout;
using std::endl;

string short2bin(short n)
{
    bool temp[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    short i = 8;
    while(n > 0)
    {
        i--;
        temp[i] = n % 2;
        n /= 2;
    }
    
    string str;
    for(int j = 0; j < 8; j++)
        str.push_back(temp[j] + 48);
    
    return str;
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

short bin2short(string str)
{
    short cover(0);
    
    for(int i = 7; i >= 0; i--)
        cover += (str[7-i] - 48) * pow(2.0, i + 0.0);

    return cover;
}

string set_header(string str)
{
    int s_size = str.size();
    string t_set;

    while(s_size > 0)
    {
        t_set += s_size % 2;
        s_size /= 2;
    }
    
    int zero_set = header_size - t_set.size();
    string set;

    for(int i = 0; i < zero_set; i++)
        set += '0';
    for(int i = t_set.size() - 1; i >= 0; i--)
        set += t_set[i] + 48;
    set += str;

    return set;
}

void pvd(vector<short> &buffer, string str, short format, short method)
{
    string t_message = Tobinary(str);
    string message = set_header(t_message);
    int message_count = 0;

#ifdef debug_print
    for(int i = 0; i < message.size(); i++)
        cout << message[i];
    cout << endl;
#endif
    short div(0);
    if(method == 1)
    {
        cout << "Please input the Div(to separate lower-level and higher-level):" << endl;
        cin >> div;
    }

    for(int index = 0; index < buffer.size(); index += 2)
    {

        if(index + 1 == buffer.size() - 1)
            cout << "remain " << message.size() - message_count << "bit no hide!" << endl;
        int diff = buffer[index+1] - buffer[index];

        bool end = 0;
        int K = classify(abs(diff));
        int BIT, Lk;
        //classfy into 14 interval
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
            case 7:
                    BIT = 8;
                    Lk = 256;
                    break;
            case 8:
                    BIT = 9;
                    Lk = 512;
                    break;
            case 9:
                    BIT = 10;
                    Lk = 1024;
                    break;
            case 10:
                    BIT = 11;
                    Lk = 2048;
                    break;
            case 11:
                    BIT = 12;
                    Lk = 4096;
                    break;
            case 12:
                    BIT = 13;
                    Lk = 8192;
                    break;
            case 13:
                    BIT = 14;
                    Lk = 16384;
                    break;
            case 14:
                    BIT = 15;
                    Lk = 32768;
                    break;
        }
        
        int remain_bit = message.size() - message_count;
        //judgment hide method
        bool n_pvd = 0;     //if in the lower-level, doesn't need pvd
        if(method == 1)     //for pvd-lsb
        {
            
            string s_message1, s_message2;     //store 3 bit message or less
            if(abs(diff) <= div)
            {
                if(remain_bit <= 3)    //remain less than 3 bit
                {
                    for(int i = message_count; i < message.size(); i++)
                        s_message1.push_back(message[i]);
                    buffer[index] = lsb(buffer[index], s_message1);
                    break;
                }

                else if(remain_bit > 3 && remain_bit <= 6)    //remain 4~6 bit
                {
                    for(int i = 0; i < 3; i++)
                    {
                        s_message1.push_back(message[message_count]);
                        message_count++;
                    }

                    for(; message_count < message.size(); message_count++)
                        s_message2.push_back(message[message_count]);

                    buffer[index] = lsb(buffer[index], s_message1);
                    buffer[index+1] = lsb(buffer[index+1], s_message2);
                    break;
                }

                else
                {
                    short three = 6;
                    while(three > 3)       //first 3 bit
                    {
                        s_message1.push_back(message[message_count]);
                        message_count++;
                        three--;
                    }

                    while(three > 0)          //last 3 bit
                    {
                        s_message2.push_back(message[message_count]);
                        message_count++;
                        three--;
                    }
                          
                    buffer[index] = lsb(buffer[index], s_message1);
                    buffer[index+1] = lsb(buffer[index+1], s_message2);
                }

                int newd = buffer[index+1] - buffer[index], abs_newd;
                if(newd < 0)
                    abs_newd = -newd;
                else abs_newd = newd;
 
                if(abs_newd > div)
                {
                    if(newd > 0)
                    {
                        buffer[index+1] -= 8;
                        buffer[index] += 8; 
                    }
                    
                    else
                    {
                        buffer[index+1] += 8;
                        buffer[index] -= 8;
                    }
                }
                n_pvd =1;
            }
        }

        if(method == 2)     //for pvd - matrix embbeding
        {
           string smessage1, smessage2;    //store segment message
           if(abs(diff) <= 63)
           {
               if(remain_bit <= 3)    //if remain less than 3, lsb
               {
                   for(; message_count < message.size(); message_count++)
                       smessage1.push_back(message[message_count]);

                   buffer[index] = lsb(buffer[index], smessage1);
                   break;
               }

               else
               {
                   short th = 6;

                   while(th > 3)
                   {
                       smessage1.push_back(message[message_count]);
                       message_count++;
                       th--;
                   }
                   while(th > 0)
                   {
                       smessage2.push_back(message[message_count]);
                       message_count++;
                       th--;
                   }

                   buffer[index] = get_better(buffer[index], smessage1);
                   buffer[index+1] = get_better(buffer[index+1], smessage2);
               }
               n_pvd = 1;
           } 
        }

        //if difference value in lower-level, next 2 pixel
        if(n_pvd)
            continue;

        //calculate message value
        int dec = 0, Bcount;
     
        //if the message not enough the require bit, end hide
        if(remain_bit <= BIT)
        {    
            Bcount = remain_bit;
            end = 1;
        }
        else
            Bcount = BIT;

        for(; Bcount > 0 ; Bcount--)
        {
            dec += pow(2, Bcount - 1) * (message[message_count] - 48);
            message_count ++;
        }
#ifdef debug_print
        cout << "original:" << buffer[index] << ' ' << buffer[index+1] << endl;
#endif
        dec += Lk;    //add low bound
        if(diff < 0) dec = -dec;

#ifdef debug_print
        cout << "old:" << diff << endl << "new:" << dec << endl;
#endif

        int NewOldDiff = dec - diff, forward = buffer[index], back = buffer[index+1];
        //if(buffer[index] < 0) NewOldDiff = -NewOldDiff;
#ifdef debug_print        
        cout << "different:" << NewOldDiff << endl;
#endif
        if(format == 1)
        {
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
        }
        else if(format == 2)
        {
            if(NewOldDiff % 2 == 0)
            {
                forward -= NewOldDiff / 2;
                back += NewOldDiff / 2;
            
                /*if(forward < -32768)
                {
                    back -= -32768 - forward;
                    forward = -32768;
                }
    
                if(forward > 65535)
                {
                    back -= forward - 65535;
                    forward = 65535;
                }

                if(back < 0)
                {
                    forward -= back;
                    back = 0;
                }

                if(back > 65535)
                {
                    forward -= back - 65535;
                    back = 65535;
                }*/
            }

            else
	    {
	        if(NewOldDiff >= 0)
	        {
	            forward -= NewOldDiff / 2 + 1;
		    back += NewOldDiff / 2;

		    /*if(forward < 0)
		    {
		        back -= forward;
		        forward = 0;
		    }
	
		    if(forward > 65535)
		    {
		        back -= forward - 65535;
		        forward = 65535;
		    }

		    if(back < 0)
		    {
		        forward -= back;
		        back = 0;
		    }

		    if(back > 65535)
		    {
		        forward -= back - 65535;
		        back = 65535;
		    }*/		
	        }
	    
	        else
	        {
	            forward -= NewOldDiff / 2 - 1;
		    back += NewOldDiff / 2;

		    /*if(forward < 0)
		    {
		        back -= forward;
		        forward = 0;
		    }
	
		    if(forward > 65535)
		    {
		        back -= forward - 65535;
		        forward = 65535;
		    }

		    if(back < 0)
		    {
		        forward -= back;
		        back = 0;
		    }

		    if(back > 65535)
		    {
		        forward -= back - 65535;
		        back = 65535;
		    }*/
	        }   
	    }
        }   

        buffer[index] = forward;
        buffer[index+1] = back;
#ifdef debug_print
        cout << "before:" << buffer[index] << ' ' << buffer[index+1] << endl;
         cout << endl;
#endif
        if(end) break;

    }
}

int classify(int data)
{
    if(data >= 0 && data <= 7)
        return 1;
    else if(data >= 8 && data <= 15)
        return 2;
    else if(data >= 16 && data <= 31)
        return 3;
    else if(data >= 32 && data <= 63)
        return 4;
    else if(data >= 64 && data <= 127)
        return 5;
    else if(data >= 128 && data <= 255)
        return 6;
    else if(data >= 256 && data <= 511)
        return 7;
    else if(data >= 512 && data <= 1023)
        return 8;
    else if(data >= 1024 && data <= 2047)
        return 9;
    else if(data >= 2048 && data <= 4095)
        return 10;
    else if(data >= 4096 && data <= 8191)
        return 11;
    else if(data >= 8192 && data <= 16383)
        return 12;
    else if(data >= 16384 && data <= 32767)
        return 13;
    else if(data >= 32768 && data <= 65535)
        return 14;
}

short lsb(short cover, string message)
{
    string str = short2bin(cover);

    //lsb final n bit
    for(int i = message.size() - 1, j = 0; i >= 0 && j < message.size(); i--, j++)
        str[7-j] = message[i];
    
    short c = bin2short(str);
  
    return c;
}

short matrix_embedding(short cover, string message)
{
    string str = short2bin(cover);

    bool n_change[2], coset[6];       //first and second bit unchange, last 6bit use (6,3) hamming code
    bool syndrome[3];
    bool H_trans[3][6] = { {1, 0, 0, 0, 1, 1},
                           {0, 1, 0, 1, 0, 1},
                           {0, 0, 1, 1, 1, 0}
                         };                           //matrix H transport

    n_change[0] = str[0] - 48;
    n_change[1] = str[1] - 48;
    //coset leader set
    for(int i = 2; i < 8; i++)
        coset[i-2] = str[i] - 48;

    //syndrome set
    for(int i = 0; i < 3; i++)
        syndrome[i] = message[i] - 48;

#ifdef debug_print
    cout << "before:" << n_change[0] << n_change[1];
    for(int i = 0; i < 6; i++)
        cout << coset[i];
    cout << endl;
#endif

    //coset leader's syndrome
    bool co_syndrome[3];
    for(int i = 0; i < 3; i++)
    {
        int sum = 0;
        for(int j = 0; j < 6; j++)
           if(coset[j] * H_trans[i][j] == 1)
               sum++; 
        
        co_syndrome[i] = sum % 2;
    }


    //exclusive or the message
    short change_bit = 4 * (syndrome[0] ^ co_syndrome[0])
                     + 2 * (syndrome[1] ^ co_syndrome[1])
                     + 1 * (syndrome[2] ^ co_syndrome[2]);    //binary to short, easy to calculate

    //find coset leader and exclusive or the cover data
    if(change_bit == 1)
        coset[2] ^=1;
    if(change_bit == 2)
        coset[1] ^= 1;
    if(change_bit == 3)
        coset[3] ^= 1;
    if(change_bit == 4)
        coset[0] ^= 1;
    if(change_bit == 5 || change_bit == 6)   
        coset[change_bit-1] ^= 1;
    if(change_bit == 7)
    {
        coset[0] ^= 1;
        coset[3] ^= 1;
    }

    for(int i = 2; i < 8; i++)
        str[i] = coset[i-2] + 48;
    cover = bin2short(str);

    
#ifdef debug_print
    cout << "after :" << str << endl;
    cout << "syndrome:" << co_syndrome[0] << co_syndrome[1] << co_syndrome[2] << endl;
    cout << "cb:" << change_bit << endl;
    
    string c;
    for(int j = 0; j < 3; j++)
    {
        int s(0);
        for(int i = 0; i < 6; i++)
            if(coset[i] * H_trans[j][i] == 1)
                s++;
        c.push_back(s%2 + 48);
    }

    if(c != message)
        cout << "NOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO" << endl;
#endif
    return cover;
    
}

short get_better(short num, string s)
{
    string str = short2bin(num);

    short head = (str[0] -48) * 128 + (str[1] - 48) * 64;

    int better[8][8] = { { 0, 28, 42, 49, 54, 45, 27,  7},
                         {32, 60, 10, 17, 22, 13, 59, 39},
                         {16, 12, 58, 33, 38, 61, 11, 23},
                         { 8, 20, 34, 57, 62, 37, 19, 15},
                         { 4, 24, 46, 53, 50, 41, 31,  3},
                         { 2, 30, 40, 51, 52, 47, 25,  5},
                         { 1, 29, 43, 48, 55, 44, 26,  6},
                         {36, 56, 14, 21, 18,  9, 63, 35}
                       };
    int mes = 4 * (s[0] - 48) + 2 * (s[1] - 48) + (s[2] - 48);

    int N;
    switch(mes)
    {
        case 0: N = 0; break;
        case 1: N = 3; break;
        case 2: N = 2; break;
        case 3: N = 4; break;
        case 4: N = 1; break;
        case 5: N = 5; break;
        case 6: N = 6; break;
        case 7: N = 7; break;
    }

    short d = 64, select;
    for(int i = 0; i < 8; i++)
        if(abs(num - head - better[N][i]) < d)
        {
            d = abs(num - head - better[N][i]);
            select = better[N][i];
        }

    return select + head;
}

string retrieve_pvd(vector<short> buffer, short format, short method)
{
    string message, size_str, bin_message;
    int size;

    bool header_y = 0;//know header size
    for(int index = 0; index < buffer.size(); index+=2)
    {
        int diff = buffer[index+1] - buffer[index];

        int K = classify(abs(diff));
        int BIT, Lk;
        //classfy into 14 interval
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
            case 7:
                    BIT = 8;
                    Lk = 256;
                    break;
            case 8:
                    BIT = 9;
                    Lk = 512;
                    break;
            case 9:
                    BIT = 10;
                    Lk = 1024;
                    break;
            case 10:
                    BIT = 11;
                    Lk = 2048;
                    break;
            case 11:
                    BIT = 12;
                    Lk = 4096;
                    break;
            case 12:
                    BIT = 13;
                    Lk = 8192;
                    break;
            case 13:
                    BIT = 14;
                    Lk = 16384;
                    break;
            case 14:
                    BIT = 15;
                    Lk = 32768;
                    break;
        }

        int dec = abs(diff) - Lk;
        string temp = short2bin(dec);
        //put BIT bit in size string
            
        if(!header_y)     //first header_size bit is size
        {
            for(int i = 8 - BIT; i < 8; i++)  
                size_str.push_back(temp[i]);
            if(size_str.size() >= header_size)    //header end
            {
                int sum(0);
                for(int i = 0; i < header_size; i++)
                    sum += (size_str[i] - 48) * pow(2, (header_size - 1 - i));

                if(size_str.size() > header_size)  //maybe more bit than "header_size"
                    for(int i = header_size; i < size_str.size(); i++)
                        bin_message.push_back(size_str[i]);   //push to message

                size = sum;
                header_y = 1;
            }
        }

        else         //content
        {
            if(BIT < size - bin_message.size())
                for(int i = 8 - BIT; i < 8; i++)
                    bin_message.push_back(temp[i]);
            else     //end condition
            {
                for(int i = 8 - (size - bin_message.size()); i < 8; i++)
                    bin_message.push_back(temp[i]);
 
                int begin = 0;
                while(message.size() < (size / 8))
                {
                    int sum(0);
                    for(int i = begin, j = 7; i < begin + 8 && j >= 0; i++, j--)
                        sum += (bin_message[i] - 48) * pow(2.0, j);

                    message.push_back(sum);
                    //cout << sum << ' ';
                    begin += 8;
                }

                break;
            }
        }
    }
    return message;
}
/*
int main()
{
    vector<short> a;
    for(int i = 0; i < 100; i+=10)
        a.push_back(i);
    for(int i = 0; i < 1000; i += 100)
        a.push_back(i);
    for(int i = 0; i < 100; i+=10)
       a.push_back(i);
    string m;
    cin >> m;
    pvd(a,m,1,2);


   short a;
   string b;
   cin >> a >> b;
   a = get_better(a, b);
   cout << a;
}*/
