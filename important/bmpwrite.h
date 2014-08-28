
class SaveBMP
{
private:
        FILE *fp;
        unsigned int *bitmap_array;
        bool load_success;
        
	    unsigned int  bfsize;


	    unsigned int biSize;
        unsigned int biWidth;;
        unsigned int biHeight;

        unsigned int biSizeImage;

        void putint(unsigned int i, FILE* outfile)
        {
            fputc( i & 0xff , outfile);
            i >>= 8;
            fputc(i & 0xff, outfile);
            i >>= 8;
            fputc(i & 0xff, outfile);
            i >>= 8;
            fputc(i & 0xff, outfile);
       }
        void putshortint(unsigned short int i, FILE* outfile)
        {
            fputc(i & 0xff, outfile);
            i >>= 8;
            fputc(i & 0xff, outfile);
       }
        void put3byte(unsigned int i, FILE* outfile)
        {
            fputc( (i & 0xff0000)>>16 , outfile);
            fputc( (i & 0xff00)>>8, outfile);
            fputc( i & 0xff , outfile);
       }
public:
	~SaveBMP()// ok
	{
		if(load_success)
		{
			delete [] bitmap_array;
		}
	}
	void initBMP(unsigned int w,unsigned h)
	{
          biWidth=w;
          biHeight=h;
          
          if(load_success) bitmap_array = new unsigned int [w*h];
          
    }
    void put_pixel(unsigned int x,unsigned int y ,unsigned pixel)
    {
          if(load_success)
	        bitmap_array[biWidth* biHeight - (biWidth - x - 1) - y * biWidth - 1] = pixel;
    }
    SaveBMP(char *file)
    {
        fp = fopen (file,"wb");
        if(fp) load_success = true;
    }
	void Save ()
	{
		if(load_success)
		{
            //header BITMAPFILEHEADER            
            putshortint(19778,fp); // 開頭BM標籤 
            
            bfsize = biWidth * biHeight * 3 + (4 - ((biWidth * 3) % 4))* biHeight + 54 ;
            //printf("%d",(4 - ((biWidth * 3) % 4))* biHeight);
            //system ("pause");
		    putint(bfsize,fp);//整體檔案大小  需要修改 !!! 
		    
			putint(0,fp);//保留 
			
			//header BITMAPINFOHEADER
			putint(54,fp); // bmp data offset
			putint(40,fp); // bmp header size
			putint(biWidth,fp);
		    putint(biHeight,fp);
		    putshortint(1,fp); //planes數目 
		    putshortint(24,fp); // pixel bit數目  
			putint(0,fp);//壓縮是否 
			
			//biSizeImage 需要check 
			biSizeImage = biWidth * biHeight * 3 + (4 - ((biWidth * 3) % 4))* biHeight ;
			//printf("%d",(( 4 - (biWidth % 4) ) + biWidth));
			//system ("pause");
			putint(biSizeImage,fp);//影像大小 
			
			putint(2953,fp);//x解析度 
            putint(2953,fp);//y解析度
            putint(0,fp);//所使用的顏色總數 
		    putint(0,fp);//重要顏色數量 
		    
		    //存入資料
            for( int i=0 ;i< biWidth* biHeight ; i++)
            {
                 put3byte( bitmap_array[i],fp);
			     if( (i+1) % biWidth == 0 && (biWidth*3)%4 != 0 )
					for(unsigned int j=0 ; j<(4-(biWidth*3)%4); fputc(0,fp),j++);
             }
			fclose(fp);
        }			
     }      
};



