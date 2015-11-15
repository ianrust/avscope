// cmidiin.cpp
#include <ao/ao.h>
#include <cstdlib>
#include <iostream>
#include "RtMidi.h"

#include "svgparser.h"
#include "wfassem.h"
#include "wftypes.h"

unsigned int port = 0;
unsigned int off = 8 << 4 + port;
unsigned int on = 9 << 4 + port;

float freqs[13] = {261.6, 277.2, 293.7, 311.1, 329.6, 349.2, 370.0, 392.0, 415.3, 440.0, 466.2, 493.9, 523.3};
WaveForm picsounds[13];
int current_freq = -1;
unsigned int middle = 60;

void mycallback( double deltatime, std::vector< unsigned char > *message, void *userData )
{
  unsigned int nBytes = message->size();
  if (nBytes < 2) return;
  if ((int)message->at(0) == on)
  {
    current_freq = (int)message->at(1) - middle;
  }
  if ((int)message->at(0) == off)
  {
    current_freq = -1;
  }
}

int main()
{
  SampleInfo info;
  //precompute a lot of image sounds
  for (int i = 0; i < 13; i++)
  {
    WaveForm wf;
    Points loop;
    info.rate = 44100;
    info.frequency = freqs[i];
    info.bytes_per_sample = 2;
    svgToPoints("resources/example_resources/butt.svg",&loop);
    loop.max_val /= 3.5;
    assembleWaveform(&loop,&wf,&info);
    picsounds[i] = wf;
  }
  //setup audio output
  ao_device * device;
  ao_sample_format format;
  int default_driver;

  ao_initialize();

  default_driver = ao_default_driver_id();

  memset(&format, 0, sizeof(format));
  format.bits = info.bytes_per_sample * 8;
  format.channels = 2;
  format.rate = info.rate;
  format.byte_format = AO_FMT_LITTLE;

  device = ao_open_live(default_driver,&format,NULL);

  RtMidiIn *midiin = new RtMidiIn();
  // Check available ports.
  unsigned int nPorts = midiin->getPortCount();
  if ( nPorts == 0 ) {
    std::cout << "No ports available!\n";
    goto cleanup;
  }
  midiin->openPort( 0 );
  // Set our callback function.  This should be done immediately after
  // opening the port to avoid having incoming messages written to the
  // queue.
  midiin->setCallback( &mycallback );
  // Don't ignore sysex, timing, or active sensing messages.
  midiin->ignoreTypes( false, false, false );
  std::cout << "\nReading MIDI input ... press <enter> to quit.\n";
  while(true)
  {
    if (current_freq != -1)
    {
      ao_play(device,picsounds[current_freq].data,picsounds[current_freq].length);
    }
  }
  // Clean up
 cleanup:
  delete midiin;
  ao_close(device);
  ao_shutdown();
  return 0;
}