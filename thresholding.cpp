#include <flow.h>

void Hysteresis_Thresholding(unsigned char low_Thresh, unsigned char high_Thresh, unsigned char diff[320][200])
{
    unsigned int a = 0, b = 0;
    unsigned char peakCandidate = 0;
    //mark all peaks
    for(a=0;a<320;a++)
    {
       for(b=0;b<200;b++)
       {
          peakCandidate = diff[a][b];
           if(peakCandidate >= high_Thresh )
           {
               // mark as peak
               diff[a][b] = 255;
           }
       }
    }
    //Check for oscillation near low threshold
    unsigned char candidate = 0;
    for(a=0;a<320;a++)
    {
        for(b=0;b<200;b++)
        {
            candidate = diff[a][b];

            if((diff[a][b] == 255) &&
               (a != 319)          &&
               (b != 199))
            {
                if(diff[a-1][b] >= low_Thresh)
                {
                    diff[a-1][b] = 255;
                }
                if(diff[a+1][b] >= low_Thresh)
                {
                    diff[a+1][b] = 255;
                }
                if(diff[a][b+1] >= low_Thresh)
                {
                    diff[a][b-1] = 255;
                }
                if(diff[a][b+1] >= low_Thresh)
                {
                    diff[a][b+1] = 255;
                }
            }
        }
    }
    // mark all other pixels low
    unsigned char pixel = 0;
    for(a=0;a<320;a++)
    {
       for(b=0;b<200;b++)
       {
          pixel = diff[a][b];
          if(pixel != 255 )
          {
              // mark as 0
              diff[a][b] = 0;
          }
       }
    }
}
