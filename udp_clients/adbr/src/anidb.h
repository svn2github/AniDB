#pragma once

#include "adbr_util.h"

// POSITIVE 2XX

namespace AniDB
{
  const ushort LOGIN_ACCEPTED                 = 200;
  const ushort LOGIN_ACCEPTED_NEW_VER         = 201;
  const ushort LOGGED_OUT                     = 203;
  const ushort RESOURCE                       = 205;
  const ushort STATS                          = 206;
  const ushort TOP                            = 207;
  const ushort UPTIME                         = 208;
  const ushort ENCRYPTION_ENABLED             = 209;

  const ushort MYLIST_ENTRY_ADDED             = 210;
  const ushort MYLIST_ENTRY_DELETED           = 211;

  const ushort ADDED_FILE                     = 214;
  const ushort ADDED_STREAM                   = 215;

  const ushort ENCODING_CHANGED               = 219;

  const ushort FILE                           = 220;
  const ushort MYLIST                         = 221;
  const ushort MYLIST_STATS                   = 222;

  const ushort ANIME                          = 230;
  const ushort ANIME_BEST_MATCH               = 231;
  const ushort RANDOMANIME                    = 232;

  const ushort EPISODE                        = 240;
  const ushort PRODUCER                       = 245;
  const ushort GROUP                          = 250;

  const ushort BUDDY_LIST                     = 253;
  const ushort BUDDY_STATE                    = 254;
  const ushort BUDDY_ADDED                    = 255;
  const ushort BUDDY_DELETED                  = 256;
  const ushort BUDDY_ACCEPTED                 = 257;
  const ushort BUDDY_DENIED                   = 258;

  const ushort VOTED                          = 260;
  const ushort VOTE_FOUND                     = 261;
  const ushort VOTE_UPDATED                   = 262;
  const ushort VOTE_REVOKED                   = 263;

  const ushort NOTIFICATION_ENABLED           = 270;
  const ushort NOTIFICATION_NOTIFY            = 271;
  const ushort NOTIFICATION_MESSAGE           = 272;
  const ushort NOTIFICATION_BUDDY             = 273;
  const ushort NOTIFICATION_SHUTDOWN          = 274;
  const ushort PUSHACK_CONFIRMED              = 280;
  const ushort NOTIFYACK_SUCCESSFUL_M         = 281;
  const ushort NOTIFYACK_SUCCESSFUL_N         = 282;
  const ushort NOTIFICATION                   = 290;
  const ushort NOTIFYLIST                     = 291;
  const ushort NOTIFYGET_MESSAGE              = 292;
  const ushort NOTIFYGET_NOTIFY               = 293;

  const ushort SENDMSG_SUCCESSFUL             = 294;
  const ushort USER                           = 295;

  // AFFIRMATIVE/NEGATIVE 3XX;

  const ushort PONG                           = 300;
  const ushort AUTHPONG                       = 301;
  const ushort NO_SUCH_RESOURCE               = 305;
  const ushort API_PASSWORD_NOT_DEFINED       = 309;

  const ushort FILE_ALREADY_IN_MYLIST         = 310;
  const ushort MYLIST_ENTRY_EDITED            = 311;
  const ushort MULTIPLE_MYLIST_ENTRIES        = 312;

  const ushort SIZE_HASH_EXISTS               = 314;
  const ushort INVALID_DATA                   = 315;
  const ushort STREAMNOID_USED                = 316;

  const ushort NO_SUCH_FILE                   = 320;
  const ushort NO_SUCH_ENTRY                  = 321;
  const ushort MULTIPLE_FILES_FOUND           = 322;

  const ushort NO_SUCH_ANIME                  = 330;
  const ushort NO_SUCH_EPISODE                = 340;
  const ushort NO_SUCH_PRODUCER               = 345;
  const ushort NO_SUCH_GROUP                  = 350;

  const ushort BUDDY_ALREADY_ADDED            = 355;
  const ushort NO_SUCH_BUDDY                  = 356;
  const ushort BUDDY_ALREADY_ACCEPTED         = 357;
  const ushort BUDDY_ALREADY_DENIED           = 358;

  const ushort NO_SUCH_VOTE                   = 360;
  const ushort INVALID_VOTE_TYPE              = 361;
  const ushort INVALID_VOTE_VALUE             = 362;
  const ushort PERMVOTE_NOT_ALLOWED           = 363;
  const ushort ALREADY_PERMVOTED              = 364;

  const ushort NOTIFICATION_DISABLED          = 370;
  const ushort NO_SUCH_PACKET_PENDING         = 380;
  const ushort NO_SUCH_ENTRY_M                = 381;
  const ushort NO_SUCH_ENTRY_N                = 382;

  const ushort NO_SUCH_MESSAGE                = 392;
  const ushort NO_SUCH_NOTIFY                 = 393;
  const ushort NO_SUCH_USER                   = 394;

  // NEGATIVE 4XX

  const ushort NOT_LOGGED_IN                  = 403;

  const ushort NO_SUCH_MYLIST_FILE            = 410;
  const ushort NO_SUCH_MYLIST_ENTRY           = 411;

  // CLIENT SIDE FAILURE 5XX

  const ushort LOGIN_FAILED                   = 500;
  const ushort LOGIN_FIRST                    = 501;
  const ushort ACCESS_DENIED                  = 502;
  const ushort CLIENT_VERSION_OUTDATED        = 503;
  const ushort CLIENT_BANNED                  = 504;
  const ushort ILLEGAL_INPUT_OR_ACCESS_DENIED = 505;
  const ushort INVALID_SESSION                = 506;
  const ushort NO_SUCH_ENCRYPTION_TYPE        = 509;
  const ushort ENCODING_NOT_SUPPORTED         = 519;

  const ushort BANNED                         = 555;
  const ushort UNKNOWN_COMMAND                = 598;

  // SERVER SIDE FAILURE 6XX

  const ushort INTERNAL_SERVER_ERROR          = 600;
  const ushort ANIDB_OUT_OF_SERVICE           = 601;
  const ushort SERVER_BUSY                    = 602;
  const ushort API_VIOLATION                  = 666;

  namespace fmask
  {
    const uint aid              = 0x40000000; // *
    const uint eid              = 0x20000000; // *
    const uint gid              = 0x10000000; // *
    const uint lid              = 0x08000000;
    const uint other_episodes   = 0x04000000; // *
    const uint IsDeprecated     = 0x02000000;
    const uint state            = 0x01000000;

    const uint size             = 0x00800000; // *
    const uint ed2k             = 0x00400000; // *
    const uint md5              = 0x00200000; // *
    const uint sha1             = 0x00100000; // *
    const uint crc32            = 0x00080000; // *

    const uint source           = 0x00004000; // *
    const uint audio_codec      = 0x00002000; // *
    const uint audio_bitrate    = 0x00001000;
    const uint video_codec      = 0x00000800; // *
    const uint video_bitrate    = 0x00000400;
    const uint video_resolution = 0x00000200; // *
    const uint file_ext         = 0x00000100; // *

    const uint dub_language     = 0x00000080; // *
    const uint sub_language     = 0x00000040; // *
    const uint duration         = 0x00000020; // *
    const uint description      = 0x00000010;
    const uint release_date     = 0x00000008;
    const uint anidb_file_name  = 0x00000001;
  }

  namespace amask
  {
    const uint total_ep         = 0x80000000; // *
    const uint high_ep          = 0x40000000; // *
    const uint year             = 0x20000000; // *
    const uint type             = 0x10000000; // *
    const uint related_aid      = 0x08000000;
    const uint related_aid_type = 0x04000000;
    const uint categories       = 0x02000000;

    const uint romaji_name      = 0x00800000; // *
    const uint kanji_name       = 0x00400000; // *
    const uint english_name     = 0x00200000; // *
    const uint other_name       = 0x00100000;
    const uint short_name       = 0x00080000;
    const uint synonyms         = 0x00040000;
    const uint producers        = 0x00020000;
    const uint producer_ids     = 0x00010000;

    const uint ep_no            = 0x00008000; // *
    const uint ep_name          = 0x00004000; // *
    const uint ep_name_romaji   = 0x00002000;
    const uint ep_name_kanji    = 0x00001000;

    const uint group_name       = 0x00000080; // *
    const uint group_short      = 0x00000040; // *
  }

  namespace status
  {
    const uint FILE_CRCOK       = 0x00000001;
    const uint FILE_CRCERR      = 0x00000002;
    const uint FILE_ISV2        = 0x00000004;
    const uint FILE_ISV3        = 0x00000008;
    const uint FILE_ISV4        = 0x00000010;
    const uint FILE_ISV5        = 0x00000020;
    const uint FILE_UNC         = 0x00000040;
    const uint FILE_CEN         = 0x00000080;
  }
}
