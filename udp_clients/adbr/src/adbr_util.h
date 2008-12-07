#pragma once

#ifndef _ADBR_UTIL_H
#define _ADBR_UTIL_H

#include <string>
#include <map>

#include <boost/regex.hpp>
#include <boost/filesystem.hpp>

using namespace std;
using namespace boost::filesystem;

typedef unsigned int uint;
typedef unsigned long ulong;
typedef unsigned short int ushort;

struct str_cmp
{
  bool operator()(const char* s1, const char* s2) const
  {
    return strcmp(s1, s2) < 0;
  }
};

typedef map<const char*, string, str_cmp> adbr_attr;

struct options
{
  bool add_mylist;
  bool use_dates;
  bool set_watched;
  uint set_mylist_state;
  bool rename;
  bool encrypt;
  bool use_repository;
  bool move;
  bool romaji;
  bool wstatus;
  bool debug;
  char *config;

  string username;
  string password;
  string apipass;

  string hashrepos;
  string animeroot;

  string format;
  string format_y;
  string format_yn;
  string format_ye;
};

struct adbr_entry
{
    long date;
    uint fileid;
    char ed2k[33];
};

typedef multimap<unsigned long, adbr_entry> adbr_hash;
typedef pair<adbr_hash::iterator, adbr_hash::iterator> range;

class adbr_repository
{
  public:
    adbr_repository(const char *filename);
    ~adbr_repository();

    bool initialised();

    void insert_hash(unsigned long size, char *ed2k, long date, uint fileid = 0);
    bool adbr_repository::update_hash(ulong size, long date, uint fileid);

    bool search_hash(unsigned long size, adbr_entry *e);
    bool search_hash(unsigned long size, long date, adbr_entry *e);

    void write_hashes(const char *filename);

  private:
    bool init;

    multimap<unsigned long, adbr_entry> *repository;
};

void clean(string &str);

void recurse_dir(vector<path> &files, path &root);
void recurse_rmdir(path &root);

#endif
