#pragma once

#ifndef _ADBR_HASH_H
#define _ADBR_HASH_H

#include <boost/filesystem.hpp>

using namespace boost::filesystem;

void ed2k_hash(path filename, char *ed2k);

#endif
