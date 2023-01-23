#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <cmath>
#include <cassert>
#include <cstddef>
#include "portaudiocpp/PortAudioCpp.hxx"
#include "curl/curl.h"

struct MemoryStruct
{
  char *memory;
  size_t size;
};

class Collector
{
public:
  struct MemoryStruct chunk;

  Collector()
  {
    this->chunk.memory = NULL; // we expect realloc(NULL, size) to work
    this->chunk.size = 0;      // no data at this point
  }

  ~Collector()
  {
    if (chunk.memory)
    {
      free(chunk.memory);
    }
  }
  int get(const void *inputBuffer, void *outputBuffer, unsigned long framesPerBuffer,
          const PaStreamCallbackTimeInfo *timeInfo, PaStreamCallbackFlags statusFlags)
  {
    assert(outputBuffer != NULL);

    std::cout << "hahha" << std::endl;

    float **out = static_cast<float **>(outputBuffer);

    for (unsigned int i = 0; i < framesPerBuffer; ++i)
    {
      // out[0][i] = table_[leftPhase_];
      // out[1][i] = table_[rightPhase_];

      // leftPhase_ += 1;
      // if (leftPhase_ >= tableSize_)
      //     leftPhase_ -= tableSize_;

      // rightPhase_ += 3;
      // if (rightPhase_ >= tableSize_)
      //     rightPhase_ -= tableSize_;
    }

    return paContinue;
  }
};

Collector collector;

static size_t write_cb(char *data, size_t size, size_t nmemb, void *userp)
{
  size_t realsize = size * nmemb;
  struct MemoryStruct *mem = (struct MemoryStruct *)userp;

  char *ptr = (char *)realloc(mem->memory, mem->size + realsize + 1);
  if (ptr == NULL)
  {
    return 0; /* out of memory! */
  }

  mem->memory = ptr;
  memcpy(&(mem->memory[mem->size]), data, realsize);
  mem->size += realsize;
  mem->memory[mem->size] = 0;

  return realsize;
}

int main(void)
{
  std::cout << "Setting up PortAudio..." << std::endl;

  // Set up the System:
  portaudio::AutoSystem autoSys;
  portaudio::System &sys = portaudio::System::instance();

  // Set up the parameters required to open a (Callback)Stream:
  portaudio::DirectionSpecificStreamParameters outParams(sys.defaultOutputDevice(), 2, portaudio::FLOAT32, false, sys.defaultOutputDevice().defaultLowOutputLatency(), NULL);
  portaudio::StreamParameters params(portaudio::DirectionSpecificStreamParameters::null(), outParams, 44100, 64, paClipOff);

  std::cout << "Opening stereo output stream..." << std::endl;

  // Create (and open) a new Stream, using the SineGenerator::generate function as a callback:
  portaudio::MemFunCallbackStream<Collector> stream(params, collector, &Collector::get);

  //  std::cout << "Starting playback for " << NUM_SECONDS << " seconds." << std::endl;

  // Start the Stream (audio playback starts):
  stream.start();

  CURL *curl_handle;

  curl_global_init(CURL_GLOBAL_ALL);

  // init the curl session
  curl_handle = curl_easy_init();

  // specify URL to get
  curl_easy_setopt(curl_handle, CURLOPT_URL, "http://bignoise.club:8000/nonstop");

  // send all data to this function
  curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_cb);

  // we pass our 'chunk' struct to the callback function
  curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&collector.chunk);

  // some servers don't like requests that are made without a user-agent field, so we provide one
  curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");

  // don't use signal for timeout
  curl_easy_setopt(curl_handle, CURLOPT_NOSIGNAL, (long)1);

  // set timeout to 10 seconds
  curl_easy_setopt(curl_handle, CURLOPT_TIMEOUT, 10);

  // if(strcmp(g_settings.softupdate_proxyserver, "") != 0)
  // {
  //   curl_easy_setopt(curl_handle, CURLOPT_PROXY, g_settings.softupdate_proxyserver);

  //   if(strcmp(g_settings.softupdate_proxyusername, "") != 0)
  //   {
  //     char tmp[200];
  //     strcpy(tmp, g_settings.softupdate_proxyusername);
  //     strcat(tmp, ":");
  //     strcat(tmp, g_settings.softupdate_proxypassword);
  //     curl_easy_setopt(curl_handle, CURLOPT_PROXYUSERPWD, tmp);
  //   }
  // }

  // get it!
  curl_easy_perform(curl_handle);

  // cleanup curl stuff
  curl_easy_cleanup(curl_handle);

  long res_code;
  if (curl_easy_getinfo(curl_handle, CURLINFO_HTTP_CODE, &res_code) == CURLE_OK)
  {
    if (200 == res_code)
    {
      // //printf("\nchunk = %s\n", chunk.memory);
      // std::istringstream iss;
      // iss.str (std::string(chunk.memory, chunk.size));
      // char line[512];
      // char *ptr;

      // while (iss.rdstate() == std::ifstream::goodbit)
      // {
      //   iss.getline(line, 512);
      //   if (line[0] != '#')
      //   {
      //     //printf("chunk: line = %s\n", line);
      //     ptr = strstr(line, "http://");
      //     if (ptr != NULL)
      //     {
      //       char *tmp;
      //       // strip \n and \r characters from url
      //       tmp = strchr(line, '\r');
      //       if (tmp != NULL)
      //         *tmp = '\0';
      //       tmp = strchr(line, '\n');
      //       if (tmp != NULL)
      //         *tmp = '\0';

      //       addUrl2Playlist(ptr, name, tim);
      //     }
      //   }
      // }
    }
  }

  // we're done with libcurl, so clean it up
  curl_global_cleanup();
  return EXIT_SUCCESS;
}