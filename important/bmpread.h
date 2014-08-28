//2006.09.27

class LoadBMP
{
private:
	struct palette
	{
		unsigned char b;
		unsigned char g;
		unsigned char r;
		unsigned char reserved; 
	};
	FILE *fp;
	unsigned int *bitmap_array,tmpc,index;
	unsigned char tmp,tmp1,tmp2;
	palette *pal_array;
	//
	bool load_success;
	//
	unsigned short int  bftype;
	unsigned int  bfsize;
	unsigned int  bfreserved;
	unsigned int bfOffBits;
	//
	unsigned int biSize;
    unsigned int biWidth;
    unsigned int biHeight;
    unsigned short int biPlanes;
    unsigned short int biBitCount;
    unsigned int biCompression;
    unsigned int biSizeImage;
    unsigned int biXPelsPerMeter;
    unsigned int biYPelsPerMeter;
    unsigned int biClrUsed;
    unsigned int biClrImportant;
public:
	~LoadBMP()
	{
		if(load_success)
		{
			delete [] bitmap_array;
			if(biBitCount<16) delete []pal_array;
		}
	}
	LoadBMP (char * file)
	{
		fp = fopen (file,"rb");
		if(fp)
		{
			fread(&bftype,2,1,fp); //header檔案格式辨識字元
			if(bftype == 19778)
			{
				load_success = true;
				//header BITMAPFILEHEADER
				fread(&bfsize,4,1,fp);
				fread(&bfreserved,4,1,fp);
				fread(&bfOffBits,4,1,fp);
				//header BITMAPINFOHEADER
				fread(&biSize,4,1,fp);
				fread(&biWidth,4,1,fp);
				fread(&biHeight,4,1,fp);
				fread(&biPlanes,2,1,fp);
				fread(&biBitCount,2,1,fp);
				fread(&biCompression,4,1,fp);
				fread(&biSizeImage,4,1,fp);
				fread(&biXPelsPerMeter,4,1,fp);
				fread(&biYPelsPerMeter,4,1,fp);
				fread(&biClrUsed,4,1,fp);
				fread(&biClrImportant,4,1,fp);

				bitmap_array = new unsigned int [biWidth*biHeight];

				//處理32bit color
                                if( biBitCount == 32)
				{
					for(unsigned int i=0;i<biWidth*biHeight;i++)
					{
						bitmap_array[i] = (unsigned int) ((fgetc(fp))<<16 | 
							(fgetc(fp)) <<8 | (fgetc(fp)));
						//if( (i+1) % biWidth == 0 && (biWidth*3)%4 != 0 )
							//for(unsigned int j=0 ; j<(4-(biWidth*3)%4); fgetc(fp),j++);
                                                                                                 fgetc(fp);
					}
                                                                }

                                 //處理24bit color
				if( biBitCount == 24)
				{
					for(unsigned int i=0;i<biWidth*biHeight;i++)
					{
						bitmap_array[i] = (unsigned int) ((fgetc(fp))<<16 | 
							(fgetc(fp)) <<8 | (fgetc(fp)));
						if( (i+1) % biWidth == 0 && (biWidth*3)%4 != 0 )
							for(unsigned int j=0 ; j<(4-(biWidth*3)%4); fgetc(fp),j++);
					}
				}
				if( biBitCount < 16 )
				{
                    tmp = 1;
                    pal_array = new palette[(tmp<<(biBitCount-1))*2];
                    //cout<<(tmp<<(biBitCount-1))*2;
					//system("pause");
                    for(int i=0;i<(tmp<<(biBitCount-1))*2;i++)
					{
						fread(&pal_array[i].b,1,1,fp);
						fread(&pal_array[i].g,1,1,fp);
						fread(&pal_array[i].r,1,1,fp);
						pal_array[i].reserved = 0 ;
						fgetc(fp);
					}
                }
				//處理8bit index color 壓縮模式
				if( biBitCount == 8 && biCompression != 0 )
				{
					tmpc=0;
					while(1)
					{
						tmp1= fgetc(fp); tmp2= fgetc(fp);
						if(tmp1 == 0 && tmp2 == 1 ) break;
						if ( tmp1 > 0 )
						{
							for(int i=0 ;i< tmp1;i++)
							{
								bitmap_array[tmpc] = (unsigned int)
                                (pal_array[tmp2].b<<16 |
                                pal_array[tmp2].g<<8 | 
                                pal_array[tmp2].r) ;
								tmpc++;
							}
						}
						if ( tmp1 == 0 && tmp2 >2)
						{
							for(int i=0 ;i< tmp2;i++)
							{
								tmp = fgetc(fp);
								bitmap_array[tmpc] = (unsigned int)
                                (pal_array[tmp].b<<16 |
                                pal_array[tmp].g<<8 | 
                                pal_array[tmp].r) ;
								tmpc++;
							}
							if ( tmp2%2 != 0 ) fgetc(fp);
						}
					}
				}
				//處理8bit index color
				if( biBitCount == 8 && biCompression == 0 )
				{
					for(unsigned int ii=0;ii<biWidth*biHeight;ii++)
					{
						tmp = fgetc(fp);
						bitmap_array[ii] = (unsigned int)
                         (pal_array[tmp].b<<16 |
							pal_array[tmp].g<<8 |
                             pal_array[tmp].r) ;
						if( (ii+1) % biWidth == 0 && (biWidth)%4 != 0 )
							for(unsigned int j=0 ; j<(4-(biWidth)%4); fgetc(fp),j++);
					}

				}
				//處理4bit index color 壓縮模式
				if(biBitCount == 4 && biCompression != 0)
				{
					tmpc = 0;
					while(1)
					{
						tmp1= fgetc(fp); tmp2= fgetc(fp);
						if(tmp1 == 0 && tmp2 == 1 ) break;				
						if ( tmp1 > 0 )
						{
							for(int i=0 ;i<tmp1;i++)
							{
								if( i%2 == 0 ) index=(tmp2&0xf0)>>4;
                                else index=(tmp2&0xf);
								bitmap_array[tmpc] = (unsigned int)
                                (pal_array[index].b<<16 |
                                pal_array[index].g<<8 |
                                 pal_array[index].r) ;
								tmpc++;
							}
						}
						if ( tmp1 == 0 && tmp2 >2)
						{
							for(int i=0 ;i<tmp2;i++)
							{
								if ( i%2 == 0 ) tmp = fgetc(fp);
								if( i%2 == 0 ) index=(tmp&0xf0)>>4;
                                 else index=(tmp&0xf);
								bitmap_array[tmpc] = (unsigned int)
                                (pal_array[index].b<<16 |
                                pal_array[index].g<<8 | 
                                pal_array[index].r) ;
								tmpc++;
							}
							if ( tmp2%4 != 0 ) fgetc(fp);
						}
					} 
				}
				//處理4bit index color
				if( biBitCount == 4 && biCompression == 0 )
				{
					for(unsigned int ii=0;ii<biWidth*biHeight;ii++)
					{
						tmp = fgetc(fp); 
						bitmap_array[ii] = (unsigned int)
                         (pal_array[(tmp&0xf0)>>4].b<<16 |
							pal_array[(tmp&0xf0)>>4].g<<8 |
                             pal_array[(tmp&0xf0)>>4].r);
						if( (ii+1)%biWidth != 0 )
						{
							bitmap_array[ii+1] = (unsigned int)
                             (pal_array[tmp&0xf].b<<16 |
								pal_array[tmp&0xf].g<<8 |
                                 pal_array[tmp&0xf].r) ;
							ii++;
						}
						if( (ii+1) % biWidth == 0 && (biWidth/2)%4 != 0 && biWidth%2 == 0 )
							for(unsigned int j=0 ;j<(4-(biWidth/2+1)%4); fgetc(fp),j++);
						if( (ii+1) % biWidth == 0 && (biWidth/2+1)%4 != 0 && biWidth%2 != 0 )
							for(unsigned int j=0 ;j<(4-(biWidth/2+1)%4); fgetc(fp),j++);
					}
				}
				//處理1bit index 單色color
				if( biBitCount == 1 && biCompression == 0 )
				{
					tmpc = 0;
					for(unsigned int ii=0;ii<biHeight;ii++)
					{
						if( biWidth < 8 )
						{
							tmp = fgetc(fp); 
							for(unsigned int j=0;j<biWidth;j++)
							{
								bitmap_array[tmpc] = (unsigned int)
									(pal_array[((tmp<<j)&0x80)>>7].b<<16 |
									pal_array[((tmp<<j)&0x80)>>7].g<<8 | 
									pal_array[((tmp<<j)&0x80)>>7].r) ;
								tmpc++;
							}
							fgetc(fp); fgetc(fp); fgetc(fp); 
						}else
						{
							for(unsigned int i=0;i<biWidth/8;i++)
							{
								tmp = fgetc(fp); 
								for(int j=0 ;j<8;j++)
								{
									bitmap_array[tmpc] = (unsigned int) 
									   (pal_array[((tmp<<j)&0x80)>>7].b<<16|
										pal_array[((tmp<<j)&0x80)>>7].g<<8 | 
										pal_array[((tmp<<j)&0x80)>>7].r) ;
									tmpc++;
								}
							}
							if(biWidth%8 != 0)
							{
								tmp = fgetc(fp);
								for(unsigned int j=0 ;j<biWidth%8;j++)
								{
									bitmap_array[tmpc] = (unsigned int)
										(pal_array[((tmp<<j)&0x80)>>7].b<<16|
										pal_array[((tmp<<j)&0x80)>>7].g<<8 | 
										pal_array[((tmp<<j)&0x80)>>7].r);
									tmpc++;
								}
								for(unsigned int jj=0;jj<4-(biWidth/8)%4-1;jj++) fgetc(fp); 
													
							}else
							{
								if((biWidth/8)%4 != 0)
								for(unsigned int i=0 ;i<4-(biWidth/8)%4;i++) fgetc(fp);  
							}
						}
					}
				}
			}
		}
	}
	void error_fix()
	{
		unsigned char r,g,b,er,eg,eb,r1,g1,b1,r2,g2,b2,r3,g3,b3;
		if(biBitCount == 24)
		{
			for(unsigned int y=0 ;y<biHeight;y++)
				for(unsigned int x=0 ;x<biWidth;x++)
				{
					g = bitmap_array[biWidth*biHeight - (biWidth - x - 1)- y * biWidth - 1]>>16;
					b = bitmap_array[biWidth*biHeight - (biWidth - x - 1)- y * biWidth - 1]>>8;
					r = bitmap_array[biWidth*biHeight - (biWidth - x - 1)- y * biWidth - 1]&0xff;
					er = r - ((r>>3)<<3);
					eg = g - ((g>>3)<<3);
					eb = b - ((b>>3)<<3);
					if( (x+1) < biWidth)
					{
						b1 = bitmap_array[biWidth*biHeight - (biWidth - (x+1) - 1)- y * biWidth - 1]>>16;
						g1 = bitmap_array[biWidth*biHeight - (biWidth - (x+1) - 1)- y * biWidth - 1]>>8;
						r1 = bitmap_array[biWidth*biHeight - (biWidth - (x+1) - 1)- y * biWidth - 1]&0xff;
						if((r1 + er * 3 / 8) <= 255 ) r1 = r1 + er * 3 / 8 ; else r1=255;
						if((g1 + eg * 3 / 8) <= 255 ) g1 = g1 + eg * 3 / 8 ; else g1=255;
						if((b1 + eb * 3 / 8) <= 255 ) b1 = b1 + eb * 3 / 8 ; else b1=255;
						bitmap_array[biWidth*biHeight - (biWidth - (x+1) - 1)- y * biWidth - 1] = b1<<16 | g1<<8 | r1;
					}
					if( (y+1) < biHeight)
					{
						b2 = bitmap_array[biWidth*biHeight - (biWidth - x - 1)- (y+1) * biWidth - 1]>>16;
						g2 = bitmap_array[biWidth*biHeight - (biWidth - x - 1)- (y+1) * biWidth - 1]>>8;
						r2 = bitmap_array[biWidth*biHeight - (biWidth - x - 1)- (y+1) * biWidth - 1]&0xff;
						if((r2 + er * 3 / 8) <= 255 ) r2 = r2 + er * 2 / 8 ; else r2=255;
						if((g2 + eg * 3 / 8) <= 255 ) g2 = g2 + eg * 2 / 8 ; else g2=255;
						if((b2 + eb * 3 / 8) <= 255 ) b2 = b2 + eb * 2 / 8 ; else b2=255;
						bitmap_array[biWidth*biHeight - (biWidth - x - 1)- (y+1) * biWidth - 1] = b2<<16 | g2<<8 | r2;
					}
					if( (y+1) < biHeight && (x+1) < biWidth )
					{
						b3 = bitmap_array[biWidth*biHeight - (biWidth - (x+1) - 1)- (y+1) * biWidth - 1]>>16;
						g3 = bitmap_array[biWidth*biHeight - (biWidth - (x+1) - 1)- (y+1) * biWidth - 1]>>8;
						r3 = bitmap_array[biWidth*biHeight - (biWidth - (x+1) - 1)- (y+1) * biWidth - 1]&0xff;
						if((r3 + er * 3 / 8) <= 255 ) r3 = r3 + er * 3 / 8 ; else r3=255;
						if((g3 + eg * 3 / 8) <= 255 ) g3 = g3 + eg * 3 / 8 ; else g3=255;
						if((b3 + eb * 3 / 8) <= 255 ) b3 = b3 + eb * 3 / 8 ; else b3=255;
						bitmap_array[biWidth*biHeight - (biWidth - (x+1) - 1)- (y+1) * biWidth - 1] = b3<<16 | g3<<8 | r3;
					}
				}
		}
	}
	unsigned int get_pixel(int x,int y)
	{
		return (unsigned int) bitmap_array[biWidth*biHeight - (biWidth - x - 1) 
			- y * biWidth - 1];
	}

	bool File_read_state()
	{
		if (fp != NULL) return true;
		else return false;
	}

	bool BMP_check()
	{
		if(bftype == 19778) return true;
		else return false;
	}

	bool open_check()
	{
		if (fp != NULL && bftype == 19778) return true;
		else return false;
	}
	unsigned int get_offset()
	{
		if( load_success ) return bfOffBits ;
		else return 0;
		
	}
	unsigned int get_headersize()
	{
		if( load_success ) return biSize;
		else return 0;
	}
	unsigned int filesize()
	{
		if( load_success ) return bfsize;
		else return 0;
	}
	unsigned int get_Width()
	{
		if( load_success ) return biWidth;
		else return 0;
	}
	unsigned int get_Height()
	{
		if( load_success ) return biHeight;
		else return 0;
	}
	unsigned short int get_colorbits()
	{
		if( load_success ) return biBitCount;
		else return 0;
	}
	unsigned short int get_planes()
	{
		if( load_success ) return biPlanes;
		else return 0;
	}
	unsigned short int get_bitcount()
	{
		if( load_success ) return biBitCount;
		else return 0;
	}
	unsigned int get_compression()
	{
		if( load_success ) return biCompression;
		else return 0;
	};
	unsigned int get_imagesize()
	{
		if( load_success ) return biSizeImage;
		else return 0;
	};
	unsigned int get_x_resolution()
	{
		if( load_success ) return biXPelsPerMeter;
		else return 0;
	}
	unsigned int get_y_resolution()
	{
		if( load_success ) return biYPelsPerMeter;
		else return 0;
	}
	unsigned int get_color_used()
	{
		if( load_success ) return biClrUsed;
		else return 0;
	}
	unsigned int get_color_important_used()
	{
		if( load_success ) return biClrImportant;
		else return 0;
	}
	
};
