hide message : -hm cover_file output (method)
retrieve message : -x cover_file (method)

hide file : -hf cover_file secret_file output (method)
retrieve file : -x cover_file (method)

The file format support bmp and wav.
(method) is steganographic method, support pixel-value differencing(pvd), pvd-lsb, pvd-matrix_embbeding, initial method is pvd.

(method) = pl      pvd-lsb
(method) = pm      pvd-matrix_embbeding

If you want to compile steganography, first you need install libsndfile(you can see the howto in my page:ms12.voip.edu.tw/~ddp509/HOWTO/libsndfile.html), then execute follow command:
  g++ steganography.cpp format.cpp datahiding.cpp -o steganography.exe -lsndfile

then you can execute program like :
  ./steganography.exe -hm XXX.bmp OOO.bmp pl (hide message to bmp in pvd-lsb)

