// ADBR by arcturo
// Summary of features:
// * hashing including hash repository
// * add to mylist with state/watched status
// * file renaming based on preferred pattern
// * filename cleaning and accumulated episode names
// * move mode based on preferred name and watched status
// * AES encryption

#include <stdio.h>
#include <string>
#include <iostream>
#include <map>
#include <vector>

#include <boost/filesystem.hpp>
#include <boost/algorithm/string/trim.hpp>

#if defined(_WIN32)
#include <windows.h>
#endif

#include <SimpleOpt.h>
#include <SimpleGlob.h>

#include "inireader/parser.h"
#include "inireader/ini_file.h"

#include "adbr_util.h"
#include "adbr_hash.h"
#include "anidb_connection.h"

using namespace boost::filesystem;

void init_params(options *adbr_options);
bool init_config(options *adbr_opt);
void print_state(options *adbr_options);
void process_file(const options *adbr_opt, path &fpath);
void recurse_dir(vector<path> &files, path &root);
void recurse_rmdir(path &root);
void print_version();
void print_usage();

AniDB::Connection *adbc;
adbr_repository *repos;
options adbr_opt;

enum { OPT_FLAG            = 0x01,
       OPT_COMMAND         = 0x10,
       OPT_COMMAND_HELP    = 0x11,
       OPT_COMMAND_VERSION = 0x12,
       OPT_PARAM           = 0x20,
       OPT_PARAM_CONFIG    = 0x21,
       OPT_ENABLE          = 0x40,
       OPT_ENABLE_DEBUG    = 0x41 };
CSimpleOpt::SOption g_rgOptions[] =
{
  { OPT_FLAG, "-a", SO_NONE },
  { OPT_FLAG, "-A", SO_NONE },
  { OPT_FLAG, "-d", SO_NONE },
  { OPT_FLAG, "-D", SO_NONE },
  { OPT_FLAG, "-e", SO_NONE },
  { OPT_FLAG, "-E", SO_NONE },
  { OPT_FLAG, "-h", SO_NONE },
  { OPT_FLAG, "-H", SO_NONE },
  { OPT_FLAG, "-j", SO_NONE },
  { OPT_FLAG, "-J", SO_NONE },
  { OPT_FLAG, "-m", SO_NONE },
  { OPT_FLAG, "-M", SO_NONE },
  { OPT_FLAG, "-r", SO_NONE },
  { OPT_FLAG, "-R", SO_NONE },
  { OPT_FLAG, "-s", SO_NONE },
  { OPT_FLAG, "-S", SO_NONE },
  { OPT_FLAG, "-w", SO_NONE },
  { OPT_FLAG, "-W", SO_NONE },
  { OPT_FLAG, "-x", SO_NONE },
  { OPT_FLAG, "-X", SO_NONE },
  { OPT_PARAM_CONFIG, "-config",  SO_REQ_CMB },
  { OPT_PARAM_CONFIG, "--config", SO_REQ_CMB },
  { OPT_COMMAND_VERSION, "--version", SO_NONE },
  { OPT_COMMAND_HELP,    "--help",    SO_NONE },
  { OPT_ENABLE_DEBUG,    "--debug",   SO_NONE },

  SO_END_OF_OPTIONS
};

int main(int argc, char **argv)
{
  init_params(&adbr_opt);

  CSimpleOpt args(argc, argv, g_rgOptions, SO_O_CLUMP);
  while(args.Next())
  {
    if(args.LastError() == SO_SUCCESS)
    {
      if(args.OptionId() == OPT_FLAG)
      {
        char c = args.OptionText()[1];

        switch(c)
        {
          case 'a' : adbr_opt.add_mylist = true;  break;
          case 'A' : adbr_opt.add_mylist = false; break;
          case 'd' : adbr_opt.use_dates = true;  break;
          case 'D' : adbr_opt.use_dates = false; break;
          case 'e' : adbr_opt.encrypt = true;  break;
          case 'E' : adbr_opt.encrypt = false; break;
          case 'j' : adbr_opt.romaji = true;  break;
          case 'J' : adbr_opt.romaji = false; break;
          case 'm' : adbr_opt.move = true;  break;
          case 'M' : adbr_opt.move = false; break;
          case 'r' : adbr_opt.rename = true;  break;
          case 'R' : adbr_opt.rename = false; break;
          case 's' : adbr_opt.wstatus = true;  break;
          case 'S' : adbr_opt.wstatus = false; break;
          case 'w' : adbr_opt.set_watched = true;  break;
          case 'W' : adbr_opt.set_watched = false; break;
          case 'x' : adbr_opt.set_mylist_state = 3; break;
          case 'X' : adbr_opt.set_mylist_state = 1; break;
          default: printf("Unrecognised option: %s\n", args.OptionText());
        }
      }
      else if(args.OptionId() == OPT_PARAM_CONFIG)
      {
        if(adbr_opt.config)
          delete adbr_opt.config;

        adbr_opt.config = strdup(args.OptionArg());
      }
      else if(args.OptionId() & OPT_COMMAND)
      {
        if(args.OptionId() == OPT_COMMAND_VERSION)
        {
          print_version();
        }
        else if(args.OptionId() == OPT_COMMAND_HELP)
        {
          print_usage();
        }
        exit(0);
      }
      else if(args.OptionId() & OPT_ENABLE)
      {
        if(args.OptionId() == OPT_ENABLE_DEBUG)
        {
          adbr_opt.debug = true;
        }
      }
      else
      {
        printf("Identified: %s\n", args.OptionText());
      }
    }
    else
    {
      printf("Unidentified: %s\n", args.OptionText());
    }

    if(args.OptionId() == 0)
    {
      print_usage();
      return 0;
    }
  }

  CSimpleGlob glob(0);
  if(glob.Add(args.FileCount(), args.Files()) < SG_SUCCESS)
  {
    printf("Error while globbing files\n");
    return 1;
  }

  if(!init_config(&adbr_opt))
  {
    printf("Critical error: failed to load config file.\n");
    exit(1);
  }

  vector<path> files;
  for(size_t i = 0, l = glob.FileCount(); i < l; i++)
  {
    path p(glob.File(i));
    if(is_directory(p))
    {
      recurse_dir(files, p);
    }
    else
    {
      files.push_back(p);
    }
  }

  if(!files.size())
  {
    print_usage();
    exit(1);
  }

  if(adbr_opt.use_repository)
  {
    repos = new adbr_repository(adbr_opt.hashrepos.c_str());
    adbr_opt.use_repository = repos->initialised();
  }

  adbc = new AniDB::Connection();
  adbc->setKillCallback(terminate);

  if(adbr_opt.debug)
  {
    adbc->setDebug(true);
  }

  printf("    Logging in...\t\t\t");
  if((adbr_opt.encrypt && adbc->login(adbr_opt.username.c_str(),
            adbr_opt.password.c_str(), adbr_opt.apipass.c_str(), true)) ||
      adbc->login(adbr_opt.username.c_str(), adbr_opt.password.c_str()))
  {
    printf("Done\n");
  }
  else
  {
    printf("Failed\n");
    return 0;
  }

  for(size_t i = 0, l = files.size(); i < l; i++)
  {
    printf("Processing \"%s\" [%u/%u]\n", files[i].string().c_str(), i+1, l);
    process_file(&adbr_opt, files[i]);
  }

  printf("    Logging out...\t\t\t");
  if(adbc->logout())
  {
    printf("Done\n");
  }
  else
  {
    printf("Failed\n");
  }

  if(adbr_opt.move)
  {
    path p(adbr_opt.animeroot);
    for(directory_iterator it(p); it != directory_iterator(); it++)
    {
      if(is_directory(*it))
      {
        path d = *it;
        recurse_rmdir(d);
      }
    }
  }

  delete adbc;

  if(adbr_opt.use_repository)
  {
    repos->write_hashes(adbr_opt.hashrepos.c_str());
  }

  return 0;
}

void process_file(const options *adbr_opt, path &fpath)
{
  long fdate = last_write_time(fpath);
  ulong fsize = file_size(fpath);
  uint fid = 0;
  char ed2k[33];
  adbr_attr attrib;

  bool hash_req = false,
       ident_req = true;

  if(adbr_opt->use_repository)
  {
    printf("\tSearching repository...\t\t");

    adbr_entry e;
    if((adbr_opt->use_dates && repos->search_hash(fsize, fdate, &e)) ||
       (repos->search_hash(fsize, &e)))
    {
      strcpy(ed2k, e.ed2k);
      fid = e.fileid;

      printf("Found (%s|%lu)\n", ed2k, fsize);
    }
    else
    {
      hash_req = true;
      printf("Failed\n");
    }
  }

  if((!adbr_opt->use_repository && !adbr_opt->rename &&
      !adbr_opt->add_mylist && !adbr_opt->move) ||
     (fid && !adbr_opt->rename && !adbr_opt->move))
  {
    ident_req = false;
  }

  if(hash_req)
  {
    printf("\tHashing...\t\t\t");
    ed2k_hash(fpath.string().c_str(), ed2k);
    printf("Done (%s)\n", ed2k);

    if(adbr_opt->use_repository)
    {
      repos->insert_hash(fsize, ed2k, fdate);
    }
  }

  if(ident_req)
  {
    printf("\tIdentifying file...\t\t");

    if((fid && adbc->file(attrib, fid)) ||
      (!fid && adbc->file(attrib, ed2k, fsize)))
    {
      if(adbr_opt->romaji)
        attrib["PREFERRED_NAME"] = attrib["ANIME_ROMAJI"];
      else
        attrib["PREFERRED_NAME"] = attrib["ANIME_ENGLISH"];

      sscanf(attrib["FID"].c_str(), "%u", &fid);

      if(adbr_opt->use_repository)
        repos->update_hash(fsize, fdate, fid);

      printf("Done (%u)\n", fid);
    }
    else
    {
      printf("Failed\n");
      return;
    }
  }

  if(adbr_opt->rename || adbr_opt->move)
  {
    printf("\tRenaming file...\t\t");

    string str;

    if(adbr_opt->rename)
    {
      str = adbr_opt->format;

      if(!attrib["ANIME_TYPE"].compare("Film"))
      {
        if(attrib["EPISODE_NAME"].find("0123456789") == string::npos)
        {
          str = adbr_opt->format_yn;
        }
        else
        {
          str = adbr_opt->format_y;
        }
      }
      else if(!attrib["ANIME_TYPE"].compare("OVA") ||
              !attrib["ANIME_TYPE"].compare("TV Special"))
      {
        if(!attrib["EPISODE_TOTAL"].compare("1"))
        {
          if(attrib["EPISODE_NAME"].find("OVA") == string::npos)
          {
            str = adbr_opt->format_ye;
          }
          else
          {
            str = adbr_opt->format_y;
          }
        }
      }

      for(size_t i = 0; i < str.length(); i++)
      {
        size_t st;
        if(str.at(i) == '%')
        {
          st = i;
          while(++i < str.length())
          {
            if(str.at(i) != '%')
            {
              continue;
            }
            else
            {
              string at = str.substr(st+1, i-st-1);
              if(attrib.count(at.c_str()))
              {
                string r = attrib[at.c_str()];
                str.replace(st, i-st+1, r);
                i = st+r.length();
//              printf("after:  %s (<- %s)\n", str->c_str(), at.c_str());
              }
              else
              {
                printf("|%s| is not a valid token\n", at.c_str());
              }
            }
            break;
          }
        }
      }

      boost::algorithm::trim(str);
    }
    else
    {
      str = fpath.filename();
    }

    path fnew = fpath.parent_path();

    if(adbr_opt->move)
    {
      fnew = adbr_opt->animeroot;

      if(adbr_opt->wstatus)
      {
        size_t viewdate = 0;

        adbr_attr file_att;

        if(adbc->myfile(file_att, fid))
        {
          sscanf(file_att["VIEWDATE"].c_str(), "%u", &viewdate);
        }

        if(viewdate)
        {
          fnew /= "watched";
        }
        else
        {
          fnew /= "unwatched";
        }
      }

      fnew /= attrib["PREFERRED_NAME"];
      if(!exists(fnew))
      {
        if(!create_directories(fnew))
        {
          printf("Failed to create parent (%s) | ", fnew.file_string().c_str());
        }
      }

      fnew /= str;
    }
    else
    {
      fnew = fpath.parent_path() / str;
    }

    if(exists(fnew))
    {
      printf("Already exists (%s)\n", fnew.file_string().c_str());
    }
    else
    {
      rename(fpath, fnew);
      if(exists(fnew))
      {
        printf("Done (%s)\n", fnew.filename().c_str());
      }
      else
      {
        printf("Failed (%s)\n", fnew.file_string().c_str());
      }
    }
  }

  if(adbr_opt->add_mylist)
  {
    printf("\tAdding to mylist...\t\t");

    if((fid && adbc->mylistadd(fid, adbr_opt)) ||
      (!fid && adbc->mylistadd(ed2k, fsize, adbr_opt)))
    {
      printf("Done\n");
    }
    else
    {
      printf("Failed\n");
    }
  }
}

void terminate()
{
  if(adbr_opt.use_repository)
  {
    repos->write_hashes(adbr_opt.hashrepos.c_str());
  }
}

void init_params(options *adbr_opt)
{
  adbr_opt->add_mylist = true;
  adbr_opt->use_dates = true;
  adbr_opt->set_watched = false;
  adbr_opt->set_mylist_state = 1;
  adbr_opt->rename = true;
  adbr_opt->use_repository = true;
  adbr_opt->romaji = true;
  adbr_opt->encrypt = false;
  adbr_opt->move = false;
  adbr_opt->wstatus = false;
  adbr_opt->debug = false;

  char *config_path = getenv("ADBR_CONFIG");
  if(config_path)
    adbr_opt->config = strdup(config_path);
}

bool init_config(options *adbr_opt)
{
  CIniFile config;

  if(adbr_opt->config == NULL)
    adbr_opt->config = "~/adbr.ini";

  config.LoadFile(adbr_opt->config);
  if(config.m_status)
  {
    return false;
  }

  if(config.SetCategory("anidb"))
  {
    if(config.SetEntry("username"))
      adbr_opt->username = config.GetEntryValue();
    else
      return false;

    if(config.SetEntry("password"))
      adbr_opt->password = config.GetEntryValue();
    else
      return false;

    if(config.SetEntry("apipass"))
      adbr_opt->apipass = config.GetEntryValue();
    else
      return false;

    boost::algorithm::trim(adbr_opt->username);
    boost::algorithm::trim(adbr_opt->password);
    boost::algorithm::trim(adbr_opt->apipass);
  }
  else
  {
    return false;
  }

  if(config.SetCategory("paths"))
  {
    if(config.SetEntry("hashrepos"))
      adbr_opt->hashrepos = config.GetEntryValue();
    else
      adbr_opt->use_repository = false;

    if(config.SetEntry("animeroot"))
      adbr_opt->animeroot = config.GetEntryValue();
    else
      adbr_opt->move = false;

    boost::algorithm::trim(adbr_opt->hashrepos);
    boost::algorithm::trim(adbr_opt->animeroot);
  }

  if(config.SetCategory("formatting"))
  {
    if(config.SetEntry("format"))
      adbr_opt->format = config.GetEntryValue();
    else
      adbr_opt->format = "%preferred_name% [%episode%] - %episode_name%.%filetype%";

    if(config.SetEntry("format_y"))
      adbr_opt->format_y = config.GetEntryValue();
    else
      adbr_opt->format_y = "%preferred_name% (%anime_year%).%filetype%";

    if(config.SetEntry("format_yn"))
      adbr_opt->format_yn = config.GetEntryValue();
    else
      adbr_opt->format_yn = "%preferred_name% (%anime_year%) [%episode%].%filetype%";

    if(config.SetEntry("format_ye"))
      adbr_opt->format_ye = config.GetEntryValue();
    else
      adbr_opt->format_ye = "%preferred_name% (%anime_year%) - %episode_name%.%filetype%";
  }

  return true;
}

void print_version()
{
  printf("%s built on %s %s\n", __FILE__, __TIME__, __DATE__);
}

void print_usage()
{
  printf("adbr [options] <file> [files] ...\n\n"
"Options:         default: [arjWXhdMS]\n"
"  -a  Add hashed files to mylist\n"
"  -r  Rename files\n"
"  -j  Use Romaji anime titles\n"
"  -w  Mark file watched\n"
"  -x  Add files as deleted\n"
"  -h  Use hash repository\n"
"  -d  Peform date check\n"
"  -m  Move to anime root directory\n"
"  -s  Move using watched status\n\n"

"  --help\n"
"      Display this message.\n"
"  --version\n"
"      Display version information.\n\n"

"  --config=<file>\n"
"      Specify the config file.\n"
"      Otherwise, define ADBR_CONFIG environment variable.\n\n"

"Format vars:\n"
"  %%PREFERRED_NAME%%, %%FID%%, %%AID%%, %%EID%%, %%GID%%, %%OTHER_EPISODES%%,\n"
"  %%STATE%%, %%SIZE%%, %%ED2K%%, %%MD5%%, %%SHA1%%, %%CRC%%, %%SOURCE%%,\n"
"  %%AUDIO%%, %%VIDEO%%, %%VIDEO_RES%%, %%EXT%%, %%AUDIO_LANG%%, %%SUB_LANG%%,\n"
"  %%DURATION%%, %%EPISODE_TOTAL%%, %%EPISODE_HIGH%%, %%ANIME_YEAR%%,\n"
"  %%ANIME_TYPE%%, %%ANIME_ROMAJI%%, %%ANIME_KANJI%%, %%ANIME_ENGLISH%%,\n"
"  %%EPISODE%%, %%EPISODE_NAME%%, %%GROUP_NAME%%, %%GROUP_SHORT%%, %%VERSION%%\n");
}

void print_state(options *adbr_opt)
{
  printf("adbr_opt->add_mylist       %d\n", adbr_opt->add_mylist);
  printf("adbr_opt->set_watched      %d\n", adbr_opt->set_watched);
  printf("adbr_opt->set_mylist_state %d\n", adbr_opt->set_mylist_state);
  printf("adbr_opt->rename           %d\n", adbr_opt->rename);
  printf("adbr_opt->encrypt          %d\n", adbr_opt->encrypt);

  printf("username = %s\n", adbr_opt->username.c_str());
  printf("password = %s\n", adbr_opt->password.c_str());
  printf("apipass = %s\n", adbr_opt->apipass.c_str());

  printf("format  = %s\n", adbr_opt->format.c_str());
  printf("formaty = %s\n", adbr_opt->format_y.c_str());
  printf("formatn = %s\n", adbr_opt->format_yn.c_str());
  printf("formate = %s\n", adbr_opt->format_ye.c_str());
}
