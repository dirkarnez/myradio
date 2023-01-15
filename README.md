myradio
=======
- [dirkarnez/portaudiocpp-prebuilt](https://github.com/dirkarnez/portaudiocpp-prebuilt)
- [dirkarnez/curl-prebuilt](https://github.com/dirkarnez/curl-prebuilt)
- [dirkarnez/curl-playground](https://github.com/dirkarnez/curl-playground)

### Documentation
- http://www.portaudio.com/docs/v19-doxydocs/start_stop_abort.html
- http://www.portaudio.com/docs/v19-doxydocs/open_default_stream.html

### Reference
- https://github.com/ABroadcasting/streamingplayer
- https://github.com/ABroadcasting/streamtranscoder/blob/2be06bbff686465c0523f746ba272f35af71cdb2/liboddcast.cpp
- https://github.com/maroonpu/FindAlexa/blob/411c9d5a7931facf26ca17d982c06d2f5c09dcc2/src/alexa.cpp
- https://github.com/ainouna/neutrino2/blob/e40731a5b7606ee5901ccc732cf0e35a1962a422/plugins/icecast/icecast.cpp
- https://learn.microsoft.com/en-us/windows/win32/api/mmeapi/ns-mmeapi-wavehdr
- https://stackoverflow.com/questions/61326170/im-trying-to-open-a-stream-in-portaudio
- https://github.com/PortAudio/portaudio/tree/master/examples
- ```cpp
  void CIceCast::processPlaylistUrl(const char *url, const char *name, const time_t tim) 
  {
    dprintf(DEBUG_NORMAL, "CIceCast::processPlaylistUrl\n");

    CURL *curl_handle;
    struct MemoryStruct chunk;

    chunk.memory = NULL; 	// we expect realloc(NULL, size) to work
    chunk.size = 0;    	// no data at this point

    curl_global_init(CURL_GLOBAL_ALL);

    // init the curl session
    curl_handle = curl_easy_init();

    // specify URL to get
    curl_easy_setopt(curl_handle, CURLOPT_URL, url);

    // send all data to this function
    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);

    // we pass our 'chunk' struct to the callback function
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&chunk);

    // some servers don't like requests that are made without a user-agent field, so we provide one
    curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");

    // don't use signal for timeout
    curl_easy_setopt(curl_handle, CURLOPT_NOSIGNAL, (long)1);

    // set timeout to 10 seconds
    curl_easy_setopt(curl_handle, CURLOPT_TIMEOUT, GET_PLAYLIST_TIMEOUT);

    if(strcmp(g_settings.softupdate_proxyserver, "") != 0)
    {
      curl_easy_setopt(curl_handle, CURLOPT_PROXY, g_settings.softupdate_proxyserver);

      if(strcmp(g_settings.softupdate_proxyusername, "") != 0)
      {
        char tmp[200];
        strcpy(tmp, g_settings.softupdate_proxyusername);
        strcat(tmp, ":");
        strcat(tmp, g_settings.softupdate_proxypassword);
        curl_easy_setopt(curl_handle, CURLOPT_PROXYUSERPWD, tmp);
      }
    }

    // get it! 
    curl_easy_perform(curl_handle);

    // cleanup curl stuff
    curl_easy_cleanup(curl_handle);

    long res_code;
    if (curl_easy_getinfo(curl_handle, CURLINFO_HTTP_CODE, &res_code ) ==  CURLE_OK) 
    {
      if (200 == res_code) 
      {
        //printf("\nchunk = %s\n", chunk.memory);
        std::istringstream iss;
        iss.str (std::string(chunk.memory, chunk.size));
        char line[512];
        char *ptr;

        while (iss.rdstate() == std::ifstream::goodbit) 
        {
          iss.getline(line, 512);
          if (line[0] != '#') 
          {
            //printf("chunk: line = %s\n", line);
            ptr = strstr(line, "http://");
            if (ptr != NULL) 
            {
              char *tmp;
              // strip \n and \r characters from url
              tmp = strchr(line, '\r');
              if (tmp != NULL)
                *tmp = '\0';
              tmp = strchr(line, '\n');
              if (tmp != NULL)
                *tmp = '\0';

              addUrl2Playlist(ptr, name, tim);
            }
          }
        }
      }
    }

    if(chunk.memory)
      free(chunk.memory);

    // we're done with libcurl, so clean it up
    curl_global_cleanup();
  }
  ```
