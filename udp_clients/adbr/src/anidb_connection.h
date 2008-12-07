#pragma once

#ifndef _ANIDB_CONNECTION_H
#define _ANIDB_CONNECTION_H

#if defined(_WIN32) && !defined(_WIN32_WINNT)
#define _WIN32_WINNT 0x0501
#endif

#include <string>
#include <iostream>
#include <time.h>

#include <boost/asio.hpp>
#include <boost/regex.hpp>
#include <boost/timer.hpp>

#include "adbr_util.h"

#include "cryptopp/modes.h"
#include "cryptopp/aes.h"

using namespace boost::asio;
using boost::asio::ip::udp;
using namespace std;
using namespace CryptoPP;

namespace AniDB
{
  const char ANIDB_ADDRESS[] = "api.anidb.info";
  const char ANIDB_PORT[]    = "9000";

  const char AMASK[] = "F0E0C0C0";
  const char FMASK[] = "75F86BE0";

  static const char *FILE_ANIME_TOKENS[] = { "FID", "AID", "EID", "GID", "OTHER_EPISODES", "STATE", "SIZE", "ED2K", "MD5", "SHA1", "CRC", "SOURCE", "AUDIO", "VIDEO", "VIDEO_RES", "EXT", "AUDIO_LANG", "SUB_LANG", "DURATION", "EPISODE_TOTAL", "EPISODE_HIGH", "ANIME_YEAR", "ANIME_TYPE", "ANIME_ROMAJI", "ANIME_KANJI", "ANIME_ENGLISH", "EPISODE", "EPISODE_NAME", "GROUP_NAME", "GROUP_SHORT" };
  static const size_t FILE_ANIME_TOKEN_COUNT = sizeof(FILE_ANIME_TOKENS)/
                                               sizeof(FILE_ANIME_TOKENS[0]);

  static const char *MYLIST_TOKENS[] = { "LID", "FID", "EID", "AID", "GID", "DATE", "STATE", "VIEWDATE", "STORAGE", "SOURCE", "OTHER", "FILESTATE" };
  static const size_t MYLIST_TOKEN_COUNT = sizeof(MYLIST_TOKENS)/
                                           sizeof(MYLIST_TOKENS[0]);

  static const char *EPISODE_TOKENS[] = { "EID", "AID", "LENGTH", "RATING", "VOTES", "EPISODE", "EPISODE_ENGLISH", "EPISODE_ROMAJI", "EPISODE_KANJI", "AIRED" };
  static const size_t EPISODE_TOKEN_COUNT = sizeof(EPISODE_TOKENS)/
                                            sizeof(EPISODE_TOKENS[0]);

  const size_t BUFFER_SIZE = 1024;

  class Connection
  {
    public:
      Connection();
      ~Connection();

      bool login(const char *username, const char *password,
                 const char *apipass = NULL, bool encrypt = false);
      bool logout();

      bool file(adbr_attr &attrib, uint file_id);
      bool file(adbr_attr &attrib, const char *ed2k, ulong size);

      bool episode(adbr_attr &attrib, uint anime_id, uint ep_no);

      bool myfile(adbr_attr &attrib, uint file_id);

      bool mylistadd(uint file_id, const options *opt);
      bool mylistadd(const char *ed2k, ulong size, const options *opt);

      void setDebug(bool enabled) { debug = enabled; }
      void setKillCallback(void (*kill)()) { killfunc = kill; }

    private:
      size_t send_recv();
      void send_recv(const char *out, size_t out_length, char *in, size_t in_length);
      void handle_receive(const boost::system::error_code& error, size_t);
      void close();

      bool file(adbr_attr &attrib);
      bool mylistadd(const options *opt);

      void parse_tokens(adbr_attr &attrib, const char *input,
                        const char **tokens, const size_t token_count);

      bool recvd_reply;

      bool encrypt;
      bool debug;
      char *username, *password;

      char reply[BUFFER_SIZE], request[BUFFER_SIZE], session_id[10],
          creply[BUFFER_SIZE], crequest[BUFFER_SIZE];
      byte key[16];
      size_t reply_length;
      clock_t last_access;
      boost::timer access_timer;

      static const size_t KEY_SIZE = 16, SESSION_LENGTH = 10;

      void (*killfunc)();

      boost::asio::io_service io_service;
      udp::endpoint anidb, local_endpoint;
      udp::socket *socket;
  };
}

#endif
