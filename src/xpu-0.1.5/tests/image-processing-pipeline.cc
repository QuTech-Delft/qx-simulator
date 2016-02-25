/**
 * @file		image-processing-pipeline.cc
 * @author	Nader KHAMMASSI - nader.khammassi@gmail.com 
 * @date		20-04-13
 * @copyright  none - confidential
 * @brief		
 */
#include <memory>
#include <cstdlib> // mem alloc
#include <cstring>
#include <cmath>

#include <xpu/runtime>
#include <xpu.h>
#include <xpu/timer.h>


static inline void * alloc(size_t size)
{
  void * p;

  posix_memalign((void**)&p,64,size);
  if(!p) {
     std::bad_alloc ba;
     throw ba;
  }
  return p;
}


class image
{
   int           __height;
   int           __width;
   
   public:
     
	unsigned char * pixel;
    
     image( ):__height(640),
	         __width (480)
	{
	   //pixel = posix_memalign((void**)&pixel, 64, 307200);
	   pixel = (unsigned char *)alloc(640*480*sizeof(unsigned char));
	   if(!pixel) {
		 std::bad_alloc ba;
		 throw ba;
		 }
	}
     
	image(int h, int w):__height(h),
	                    __width (w)
	{
	   pixel =  (unsigned char *)alloc(h*w*sizeof(unsigned char)); 
	   if(!pixel) {
		 std::bad_alloc ba;
		 throw ba;
		 }
	}

	image(const image & img):__height(img.__height),
	                         __width (img.__width)
	{
	   pixel =  (unsigned char *)alloc(__height*__width*sizeof(unsigned char)); 
	   if(!pixel) {
		 std::bad_alloc ba;
		 throw ba;
		 }
        memcpy(pixel, img.pixel,__height*__width*sizeof(unsigned char));
	}

	~image()
	{
	   if (pixel) 
		 free(pixel);
	}

	int height() { return __height; }
	int width()  { return __width;  }

	void fill()
	{
	   for(int i=0; i<__height; i++) {
		 for(int j=0; j<__width; j++) {
		    pixel[i*__width+j] = 0;
		 }
	   }
	}

	void fill(char v)
	{
	   for(int i=0; i<__height; i++) {
		 for(int j=0; j<__width; j++) {
		    pixel[i*__width+j] = v;
		 }
	   }
	}

	void info()
	{
	   std::cout << "[ image info | height: " << __height << " width: " << __width << "]" << std::endl;
	}

     void copyfrom(image &img)
	{
	   if (__height == img.__height && __width == img.__width)
		 memcpy(pixel, img.pixel,__height*__width*sizeof(unsigned char));
	   else throw std::exception(); // "image::copyfrom() : images have diffrent dimensions."
     }

	void negative()
	{
	   for(int i=0; i<__height; i++) {
		 for(int j=0; j<__width; j++) {
		    pixel[i*__width+j] -= 255;
		 }
	   }
	}

	void binarize(int threshold)
	{
	   for(int i=0; i<__height; i++) {
		 for(int j=0; j<__width; j++) {
		    pixel[i*__width+j] = (pixel[i*__width+j] > threshold ? 0 : 255);
		 }
	   }
	}

	void sobel()
	{
	   int gx, gy, line, col;
	   long g;
	   unsigned char ampl[__width];
        image nwimg;

	   for(line=0;line<__height;line++)
	   {
		 for(col=0;col<__width;col++)
		 {
		    if (line==0 || line==__height-1)
			  nwimg.pixel[line*__width+col] = 0; //ampl[col]  = 0;
		    else if (col==0 || col==__width-1)
			  nwimg.pixel[line*__width+col] = 0; //ampl[line] = 0;
		    else
		    {
			  gx=(int)(
			          -  (pixel[(line-1)*__width+col-1] + pixel[line*__width+col-1] + pixel[(line+1)*__width+col-1])
				     +  (pixel[(line-1)*__width+col+1] + pixel[line*__width+col+1] + pixel[(line+1)*__width+col+1])
				     );

					   
			  gy=(int)(
			          -  (pixel[(line-1)*__width+col-1] + pixel[(line-1)*__width+col] + pixel[(line-1)*__width+col+1])
				     +  (pixel[(line+1)*__width+col-1] + pixel[(line+1)*__width+col] + pixel[(line+1)*__width+col+1])
					);
					  
			  g=(long int)sqrt((float)gx*gx+(float)gy*gy);               // amplitude
			  if (g>255)  g=255;
			  pixel[line*__width+col]=g;
		    }
		 }
	   }
	   copyfrom(nwimg);
	};

};


inline int filter1(int element, std::vector<image> * vimg)                 { (*vimg)[element].negative();          return 0; }
inline int filter2(int element, std::vector<image> * vimg)                 { (*vimg)[element].sobel();             return 0; }
inline int filter3(int element, std::vector<image> * vimg, int threshold)  { (*vimg)[element].binarize(threshold); return 0; }


int main(int argc, char **argv)
{
   #define __size 200

   xpu::init();
   
   __println("[-] building image std::vector...");

   std::vector<image> vimg;

   for(int i=0; i<__size ; i++) {
	 vimg.push_back(image());
	 vimg[i].fill(i);
   }

   __println("[-] image std::vector building done.");
   __println("[-] starting sequential image processing ...");

   xpu::timer t;

   t.start(); 
   
   for(int i=0; i<__size ; i++) {
	 vimg[i].binarize(100);
	 vimg[i].sobel();
	 vimg[i].sobel();
	 vimg[i].negative();
	 //vimg[i].negative();
   }
   
   t.stop(); 

   double sequential_time =  t.elapsed();

   __println("[-] sequential processing time : " << sequential_time << " sec.");

   xpu::task fltr1(filter1, 0, &vimg );
   xpu::task fltr2(filter2, 0, &vimg );
   xpu::task fltr3(filter3, 0, &vimg, 100);

   xpu::pipeline edge_detector(vimg.size(), &fltr3, &fltr2, &fltr2, &fltr1);

   __println("[-] starting xpu pipeline (pseudo-parallel or overlapped) image processing ...");

   t.start(); 

   edge_detector.run(); // running pipeline

   t.stop(); 
   double parallel_time =  t.elapsed();

   __println("[-] pipeline time: " << parallel_time << " sec.");
  

   __println("\n\t >>>> speedup : " << 100*sequential_time/parallel_time << "%\n"  );
   __println("\n\t >>>> run " << sequential_time/parallel_time << " x times faster.\n"  );
 
   xpu::clean();
   
   return 0;
}

