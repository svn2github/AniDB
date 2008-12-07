#include <stdio.h>
#include <iostream>
#include <fstream>

#include "adbr_util.h"

adbr_repository::adbr_repository(const char *filename)
{
  FILE *hashfile = fopen(filename, "r");

  if(hashfile == NULL)
  {
    printf("IO Error: failed to open %s for read.\n", filename);
    init = false;
  }

  repository = new multimap<unsigned long, adbr_entry, std::less<ulong> >();

  char buffer[100];
  char ed2k[33];

  ulong size;
  long date;
  uint fileid;
  while(fgets(buffer, 100, hashfile) != NULL)
  {
    size = 0;
    fileid = 0;
    sscanf(buffer, "%lu %ld %32s %u", &size, &date, ed2k, &fileid);
//  printf("%s%lu %u %s %u r\n", buffer, size, date, ed2k, fileid);

    if(size)
      insert_hash(size, ed2k, date, fileid);
  }

  fclose(hashfile);

  init = true;
}

adbr_repository::~adbr_repository()
{
  delete repository;
}

void adbr_repository::insert_hash(ulong size, char *ed2k, long date, uint fileid)
{
  adbr_entry e;
  strcpy(e.ed2k, ed2k);
  e.date = date;
  e.fileid = fileid;

  repository->insert(pair<unsigned long, adbr_entry>(size, e));
}

bool adbr_repository::update_hash(ulong size, long date, uint fileid)
{
  if(repository->count(size))
  {
    range r = repository->equal_range(size);
    for(adbr_hash::iterator i = r.first; i != r.second; i++)
    {
      if(date == i->second.date)
      {
        i->second.fileid = fileid;
        return true;
      }
    }
    r.first->second.date = date;
    r.first->second.fileid = fileid;
    return false;
  }
  else
  {
    printf("This should never happen.\n");
    return false;
  }
}

bool adbr_repository::search_hash(ulong size, adbr_entry *e)
{
  if(repository->count(size))
  {
    range r = repository->equal_range(size);
    pair<unsigned long, adbr_entry> a = *(r.first);
    *e = a.second;
    return true;
  }
  else
  {
    return false;
  }
}

bool adbr_repository::search_hash(unsigned long size, long date, adbr_entry *e)
{
  if(repository->count(size))
  {
    range r = repository->equal_range(size);
    for(adbr_hash::iterator i = r.first; i != r.second; i++)
    {
      pair<unsigned long, adbr_entry> a = *i;
      if(date == a.second.date)
      {
        *e = a.second;
        return true;
      }
    }
    return false;
  }
  else
  {
    return false;
  }
}

void adbr_repository::write_hashes(const char *filename)
{
  fstream file;
  file.open(filename, fstream::out | fstream::trunc);

  if(!file.good())
  {
    cout << "Error opening hash repository for writing (" << filename << ")" << endl;
  }

  for(adbr_hash::iterator i = repository->begin(); ; )
  {
    pair<ulong, adbr_entry> e = *i;
    file << e.first << " " << e.second.date << " " << e.second.ed2k;

    if(e.second.fileid)
      file << " " << e.second.fileid;

    i++;

    if(i != repository->end())
      file << endl;
    else
      break;
  }

  file.close();
}

bool adbr_repository::initialised()
{
  return init;
}

void clean(string &str)
{
  boost::regex e("%");
  str = boost::regex_replace(str, boost::regex(":"), ";");
  str = boost::regex_replace(str, boost::regex("`"), "'");
  str = boost::regex_replace(str, boost::regex("\"(.*?)\""),
                             "``\\1\\x{00b4}\\x{00b4}");
  str = boost::regex_replace(str, boost::regex("[*?<>|]"), "");
  str = boost::regex_replace(str, boost::regex("[\\\\\\/]"), ",");
  str = boost::regex_replace(str, boost::regex(" ,"), ",");
  str = boost::regex_replace(str, boost::regex("  "), " ");
}

void recurse_dir(vector<path> &files, path &root)
{
  for(directory_iterator it(root); it != directory_iterator(); it++)
  {
    if(is_regular_file(*it))
    {
      files.push_back(*it);
    }
    else if(is_directory(*it))
    {
      path p(*it);
      recurse_dir(files, p);
    }
  }
}

void recurse_rmdir(path &root)
{
  if(!is_directory(root))
    return;

  for(directory_iterator it(root); it != directory_iterator(); it++)
  {
    if(is_directory(*it))
    {
      path p(*it);
      recurse_rmdir(p);
    }
  }

  try
  {
    remove(root);
  }
  catch(const filesystem_error &e)
  {
  }
}
