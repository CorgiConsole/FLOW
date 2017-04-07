#include <QCoreApplication>
#include <capture.h>
#include <shift.h>
#include <signal.h>

unsigned char Solenoid[16];

void handle_SIGINT(int unused)
{
  // On CTRL+C - abort //

  printf("\nTHIS VESSEL HAS BEEN CORRUPTED BY THE RUINOUS POWERS!\n");
  exit(0);
}

int main(void)
{
  unsigned char background[200][320];
  unsigned char frame[200][320];
  unsigned char diff[200][320];
  unsigned char final[200][320];
  unsigned char HI_THRESH = 20;
  unsigned char LO_THRESH = 15;

  unsigned int i,q,l;

  q = 0;
  l = 0;

  FILE *bfp;
  FILE *dif;

  // Catch CTRL+C (SIGINT) signals
  signal(SIGINT, handle_SIGINT);

  printf("I HAVE AWOKEN.\n");

  //Configure Shift Register Output
  InitShiftRegister();

  // Connect pixy
  return_value = ConnectPixy();

  //**********************************************************//
  //  Capture background image and translate to data array    //
  //**********************************************************//
  printf("\nPREPARING TO CAPTURE BACKGROUND IMAGES\n");
  usleep(5000000);
  printf("\nANALYZING Background");
  CaptureImage(frame);
  memcpy(background, frame, sizeof(background));
  // wait 5 sec
  usleep(5000000);

  //**********************************************************//
  //  Produce the background image as a .pgm file             //
  //**********************************************************//
  bfp = fopen("BACKGROUND.pgm","wb");
  if(bfp != NULL)
  {
    fprintf(bfp, "P5\n%d ", 256);
    fprintf(bfp, "%d\n255\n", 256);
    for(q=0;q<200;q++)
    {
      for(l=0;l<256;l++)
      {
        fprintf(bfp,"%c",((char)(background[q][l])));
      }
    }
    fclose(bfp);
    printf("PRODUCED BACKGROUND.pgm\n BY THE OMNISSIAH's GRACE, OUR LOGIC HAS PREVAILED!\n");
  }
  else perror("FILE ALLOCATION PREVENTED BY MALEVOLENT MACHINE SPIRITS!\n");

  //**********************************************************//
  //  Capture subsequent frames and compare until terminated  //
  //**********************************************************//
  int loop = 0;
  //run until terminated
  while(1)
  {
    //loop++;

    //wait 1/10 sec
    usleep(50000);

    CaptureImage(frame);

    //compare to background image
    //create diff data array
    unsigned char x = 0;
    unsigned char y = 0;

    for(q=0;q<200;q++)
    {
      for(l=0;l<256;l++)
      {
        final[q][l] = 0;
      }
    }


    for(q=0;q<200;q++)
    {
      for(l=0;l<256;l++)
      {
        x = background[q][l];
        y = frame[q][l];
        diff[q][l] = abs(x-y);
      }
    }

    // Mark all pixels above HI_THRESH
    for(q=0;q<200;q++)
    {
      for(l=0;l<256;l++)
      {
        x = diff[q][l];
        if(x>HI_THRESH)
        {
          final[q][l] = 255;
        }
      }
    }

    // search for neighbor candidates
    // with intensity values above LO_THRESH
    for(q=0;q<200;q++)
    {
      for(l=0;l<256;l++)
      {
        x = diff[q][l];
        if((x==255)        &&
           ((q - 1) > 0)   &&
           ((q + 1) < 200) &&
           ((l - 1) > 0)   &&
           ((l + 1) < 256))
        {
          if(diff[q-1][l] > LO_THRESH)
          {
            final[q-1][l] = 255;
          }
          if(diff[q+1][l] > LO_THRESH)
          {
            final[q+1][l] = 255;
          }
          if(diff[q][l-1] > LO_THRESH)
          {
            final[q][l-1] = 255;
          }
          if(diff[q][l+1] > LO_THRESH)
          {
            final[q][l+1] = 255;
          }
        }
      }
    }

    //find horizonal L-CROM
    unsigned char prevX    = 0;
    unsigned char nextX    = 0;
    unsigned char xstart   = 0;
    unsigned char count    = 0;
    unsigned char maxCount = 0;
    unsigned char xLCROM   = 0;
    for(q=0;q<200;q++)
    {
      for(l=0;l<256;l++)
      {
        x     = final[q][l];
        prevX = final[q][l-1];
        nextX = final[q][l+1];

        if( (x == ((char)255)) && (prevX == ((char)0)) )
        {
          xstart = l;
          count  = 1;
        }
        else if((x == ((char)255)) && (nextX == ((char)255)))
        {
          count++;
        }
        else if((x == ((char)255)) && (nextX == ((char)0)))
        {
          if(count > maxCount)
          {
            xLCROM   = xstart;
            maxCount = count;
          }
        }
      }
    }

    // using L-CROM find solenoid width
    // using L-CROM find solenoid position from right to left
    unsigned char Swid = 0;
    unsigned char Spos = 0;
    Swid = (maxCount/16);
    Spos = xLCROM;
    unsigned char begin = (Spos/16);
    unsigned char end   = (begin + Swid);

    if((begin && end) > 0)
    {
      for(i = 0; i <16; i++)
      {
        if((i >= begin) && (i <= end))
        {
          Solenoid[i] = 1;
        }
        else
        {
          Solenoid[i] = 0;
        }
      }
    }
    else
    {
      for(i = 0; i <16; i++)
      {
        Solenoid[i] = 0;
      }
    }

    if((loop % 10) == 0)
    {
      //printf("\n");
      //printf("\n");
      for(i = 0; i <16; i++)
      {
        //printf("S ");
      }
      //printf("\n");
      // calculating Solenoid Array
      for(i = 0; i <16; i++)
      {
        //printf("%d ", Solenoid[i]);
      }
      UpdateShiftRegister(Solenoid);
    }
  }

  dif = fopen("DIFF.pgm","wb");
  if(dif != NULL)
  {
    fprintf(dif, "P5\n%d ", 256);
    fprintf(dif, "%d\n255\n", 256);
    for(q=0;q<200;q++)
    {
      for(l=0;l<256;l++)
      {
        fprintf(dif,"%c",((char)(final[q][l])));
      }
    }
    fclose(dif);
    printf("PRODUCED DIFF.pgm\n BY THE OMNISSIAH's GRACE, OUR LOGIC HAS PREVAILED!\n");
  }
  else perror("FILE ALLOCATION PREVENTED BY MALEVOLENT MACHINE SPIRITS!\n");

  printf("GLORY TO THE MACHINE-GOD!\n");
  exit(0);
}

