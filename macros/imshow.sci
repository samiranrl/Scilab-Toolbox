//Author  :Priyanka Hiranandani NIT Surat    
function[] =imshow(Image)
    
    global TYPE_DOUBLE;         //retrieving list and creating 3 dimensional matrix out of it
    dimensions=size(Image)
    MaxUInt8 = 2 ^ 8 - 1;
    MaximumGrayValue = MaxUInt8;
    if dimensions==3 then 
    [d c]=size(Image(1));
     r=matrix(Image(1),c,d);
     g=matrix(Image(2),c,d);
     b=matrix(Image(3),c,d);
     z(:,:,1)=r; 
     z(:,:,2)=g; 
     z(:,:,3)=b;
     [NumberOfRows NumberOfColumns NumberOfChannels] = size(z);
     NumberOfPixels = NumberOfRows * NumberOfColumns;
     Sample = z(1);
     if type(Sample) == 1 then
     ColorMap = matrix(z, NumberOfPixels, NumberOfChannels);
     disp(1); 
     else
     TypeName = typeof(Sample)
     select TypeName
     case 'uint8' 
     MaxGrayValue = 2 ^ 8 - 1;
     case 'uint16'
     MaxGrayValue = 2 ^ 16 - 1;
     case 'uint32'
     MaxGrayValue = 2 ^ 32 - 1;
     end;  
     ColorMap = double(matrix(z, NumberOfPixels, NumberOfChannels)) ...
           / MaxGrayValue;
     end;       
     Img=z;
     elseif dimensions==1 then
     [d c]=size(Image(1));
     Img=matrix(Image(1),c,d);
     Img=Img';
     
     select typeof(Img)
     case 'uint8' 
     MaximumGrayValue = MaxUInt8;
     case 'uint16'  
     MaximumGrayValue = max(Image(:)) ;
     case 'uint32' 
     MaximumGrayValue = max(Image(:));                
     end;
     ColorMap = graycolormap(double(MaximumGrayValue + 1));
     end;
     show(Img,ColorMap);
endfunction
