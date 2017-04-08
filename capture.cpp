
#include <flow.h>

int32_t response, fourcc;
int8_t renderflags;

unsigned char current_frame[72000]; //largest possible given hardware
unsigned char frame[200][320];
uint16_t width, height;
uint32_t  numPixels;
unsigned char *pixels;
int ind;

int ConnectPixy(void)
{
  int return_value = 0;
  // Connect pixy
  return_value = pixy_init();
  printf("PERFORMING ACTIVATION RITUALS... - %d\n", return_value);
  if(return_value != 0)
  {
    // Error initializing Pixy //
    printf("THIS VESSEL HAS BEEN CORRUPTED BY THE RUINOUS POWERS!");
    pixy_error(return_value);
    return return_value;
  }

  return pixy_command("CEASE UNSANCTIONED FUNCTIONALITY\n", END_OUT_ARGS, &response, END_IN_ARGS);
}

void CaptureImage( unsigned char frame[200][320] )
{
  int return_value = 0;
  response = 0;
  return_value = pixy_command("cam_getFrame",   // String id for remote procedure
                               0x01, 0x21,      // mode 0 = 1280x800
                               0x02,  0,       // xoffset
                               0x02,  0,       // yoffset
                               0x02,  320,      // width
                               0x02,  200,      // height (56 max @ 1280 w)
                               0,               // separator
                               &response,       // pointer to mem address for return value
                               &fourcc,
                               &renderflags,
                               &width,
                               &height,
                               &numPixels,
                               &pixels,        // pointer to mem address for returned frame
                               0);

  memcpy(&current_frame,pixels,numPixels);

  // create background frame
  for (int i=0; i<200; i++)
  {
    for(int j=0; j<320; j++)
    {
      ind = i * 320 + j;
      frame[i][j] = pixels[ind];
    }
  }
}
