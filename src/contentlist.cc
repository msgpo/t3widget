/* Copyright (C) 2011-2012 G.P. Halkes
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License version 3, as
   published by the Free Software Foundation.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <algorithm>
#include <cerrno>
#include <cstdarg>
#include <cstdint>
#include <cstring>
#include <dirent.h>
#include <fnmatch.h>

#include "contentlist.h"
#include "main.h"
#include "util.h"

namespace t3_widget {

string_list_t::~string_list_t() {
  for (std::string *string : strings) delete string;
}

size_t string_list_t::size() const { return strings.size(); }

const std::string *string_list_t::operator[](size_t idx) const { return strings[idx]; }

void string_list_t::push_back(std::string *str) { strings.push_back(str); }

file_name_list_t::file_name_entry_t::file_name_entry_t() : is_dir(false) {
  display_name = &file_name_entry_t::name;
}

file_name_list_t::file_name_entry_t::file_name_entry_t(const char *_name,
                                                       const std::string &_utf8_name, bool _is_dir)
    : name(_name), utf8_name(_utf8_name), is_dir(_is_dir) {
  display_name = utf8_name.size() == 0 ? &file_name_entry_t::name : &file_name_entry_t::utf8_name;
}

/* We need to supply a copy constructor, or else we risk a memory leak from the
   fact that a simple bit copy may destroy a reference to memory allocated by a
   std::string. */
file_name_list_t::file_name_entry_t::file_name_entry_t(const file_name_entry_t &other) {
  name = other.name;
  utf8_name = other.utf8_name;
  is_dir = other.is_dir;
  display_name = other.display_name;
}

bool file_name_list_t::compare_entries(file_name_entry_t first, file_name_entry_t second) {
  if (first.is_dir && !second.is_dir) return true;

  if (!first.is_dir && second.is_dir) return false;

  if (first.is_dir && first.name.compare("..") == 0) return true;

  if (second.is_dir && second.name.compare("..") == 0) return false;

  if (first.name[0] == '.' && second.name[0] != '.') return true;

  if (first.name[0] != '.' && second.name[0] == '.') return false;

  /* Use strcoll on the FS names. This will sort them as the user expects,
     provided the locale is set correctly. */
  return strcoll(first.name.c_str(), second.name.c_str()) < 0;
}

size_t file_name_list_t::size() const { return files.size(); }

const std::string *file_name_list_t::operator[](size_t idx) const {
  return &(files[idx].*(files[idx].display_name));
}

const std::string *file_name_list_t::get_fs_name(size_t idx) const { return &files[idx].name; }

bool file_name_list_t::is_dir(size_t idx) const { return files[idx].is_dir; }

int file_name_list_t::load_directory(std::string *dir_name) {
  struct dirent *entry;
  DIR *dir;

  files.clear();
  if (dir_name->compare("/") != 0) files.push_back(file_name_entry_t("..", "..", true));

  if ((dir = opendir(dir_name->c_str())) == nullptr) {
    content_changed();
    return errno;
  }

  // Make sure errno is clear on EOF
  errno = 0;
  while ((entry = readdir(dir)) != nullptr) {
    std::string utf8_name;

    if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) continue;

    convert_lang_codeset(entry->d_name, &utf8_name, true);
    if (strcmp(entry->d_name, utf8_name.c_str()) == 0) utf8_name.clear();
    files.push_back(
        file_name_entry_t(entry->d_name, utf8_name, t3_widget::is_dir(dir_name, entry->d_name)));

    // Make sure errno is clear on EOF
    errno = 0;
  }

  sort(files.begin(), files.end(), compare_entries);

  if (errno != 0) {
    int error = errno;
    closedir(dir);
    content_changed();
    return error;
  }
  closedir(dir);

  content_changed();
  return 0;
}

file_name_list_t &file_name_list_t::operator=(const file_name_list_t &other) {
  if (&other == this) return *this;

  files.resize(other.files.size());
  copy(other.files.begin(), other.files.end(), files.begin());
  content_changed();
  return *this;
}

bool string_compare_filter(string_list_base_t *list, size_t idx, const std::string *str) {
  return (*list)[idx]->compare(0, str->size(), *str, 0, str->size()) == 0;
}

bool glob_filter(string_list_base_t *list, size_t idx, const std::string *str, bool show_hidden) {
  file_list_t *file_list = dynamic_cast<file_list_t *>(list);
  const std::string *item_name = (*list)[idx];
  std::string fs_name;

  if (item_name->compare("..") == 0) return true;

  if (!show_hidden && (*item_name)[0] == '.') return false;

  /* fnmatch discards strings with characters that are invalid in the locale
     codeset. However, we do want to use fnmatch because it also involves
     collation which is too complicated to handle ourselves. So we convert the
     file names to the locale codeset, and use fnmatch on those. Note that the
     filter string passed to this function is already in the locale codeset. */
  convert_lang_codeset(item_name, &fs_name, false);
  if ((file_list == nullptr || !file_list->is_dir(idx)) &&
      fnmatch(str->c_str(), fs_name.c_str(), 0) != 0)
    return false;
  return true;
}

};  // namespace
