/* Copyright (C) 2011 G.P. Halkes
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
#include "undo.h"
#include "textline.h"
#include <cstdlib>
#include <cstring>
#include <new>

namespace t3_widget {

undo_list_t::~undo_list_t() {
  current = head;
  while (current != nullptr) {
    head = head->next;
    delete current;
    current = head;
  }
}

void undo_list_t::add(undo_t *undo) {
  if (head == nullptr) {
    mark = head = tail = undo;
    return;
  }

  if (mark_beyond_current) mark_is_valid = false;

  if (current != nullptr) {
    if (mark_is_valid && current == mark) mark = undo;

    /* Free current and everything after. However, if current points to the start of the
       list, ie all edits have been undone, then we are left with an empty list but with head
       and tail pointing to something that doesn't exist anymore. Thus we set head to
       nullptr in this case so we can still see that this happened after we free everything. */

    if (current == head)
      head = nullptr;
    else
      tail = current->previous;

    while (current != nullptr) {
      undo_t *to_free = current;
      current = current->next;
      delete to_free;
    }

    if (head == nullptr) {
      head = tail = undo;
      return;
    }
  } else if (mark == nullptr && mark_is_valid) {
    mark = undo;
  }

  tail->next = undo;
  undo->previous = tail;
  tail = undo;
}

undo_t *undo_list_t::back() {
  if (current == head) return nullptr;

  if (mark_is_valid && current == mark) mark_beyond_current = true;

  if (current == nullptr) return current = tail;

  return current = current->previous;
}

undo_t *undo_list_t::forward() {
  undo_t *retval = current;

  if (current == nullptr) return nullptr;

  current = current->next;

  if (mark_is_valid && mark_beyond_current && current == mark) mark_beyond_current = false;

  return retval;
}

void undo_list_t::set_mark() {
  mark_is_valid = true;
  mark_beyond_current = false;
  mark = current;
}

bool undo_list_t::is_at_mark() const { return mark_is_valid && mark == current; }

#if 0
#ifdef DEBUG
#include "log.h"
static const char *undo_type_to_string[] = {
	"UNDO_NONE",
	"UNDO_DELETE",
	"UNDO_DELETE_BLOCK",
	"UNDO_BACKSPACE",
	"UNDO_ADD",
	"UNDO_ADD_BLOCK",
	"UNDO_REPLACE_BLOCK",
	"UNDO_OVERWRITE",
	"UNDO_DELETE_NEWLINE",
	"UNDO_BACKSPACE_NEWLINE",
	"UNDO_ADD_NEWLINE",
	"UNDO_BLOCK_START",
	"UNDO_BLOCK_END",
	"UNDO_ADD_REDO",
	"UNDO_BACKSPACE_REDO",
	"UNDO_REPLACE_BLOCK_REDO",
	"UNDO_OVERWRITE_REDO",
	"UNDO_BLOCK_START_REDO",
	"UNDO_BLOCK_END_REDO"
};

void undo_list_t::dump() {
	undo_t *ptr;

	ptr = head;

	while (ptr != nullptr) {
		text_coordinate_t start = ptr->get_start();

		lprintf("undo_t:%s%c(%d,%d) %s: '", ptr == current ? "->" : "  ", mark_is_valid && ptr == mark ? '@' : ' ',
			start.line, start.pos, undo_type_to_string[ptr->get_type_t()]);
		if (ptr->get_text() != nullptr)
			ldumpstr(ptr->get_text()->getData()->data(), ptr->get_text()->getLength());
		lprintf("'\n");

		if (ptr->get_replacement() != nullptr) {
			lprintf("        Replaced by: '");
			ldumpstr(ptr->get_replacement()->getData()->data(), ptr->get_replacement()->getLength());
			lprintf("'\n");
		}
		ptr = ptr->next;
	}
	if (current == nullptr)
		lprintf("undo_t:-> END\n");
}
#endif
#endif

#define TEXT_START_SIZE 32

undo_type_t undo_t::redo_map[] = {UNDO_NONE,
                                  UNDO_ADD,
                                  UNDO_ADD_BLOCK,
                                  UNDO_BACKSPACE_REDO,
                                  UNDO_ADD_REDO,
                                  UNDO_DELETE_BLOCK,
                                  UNDO_REPLACE_BLOCK_REDO,
                                  UNDO_OVERWRITE_REDO,
                                  UNDO_ADD_NEWLINE,
                                  UNDO_ADD_NEWLINE,
                                  UNDO_DELETE_NEWLINE,
                                  UNDO_UNINDENT,
                                  UNDO_INDENT,
                                  UNDO_ADD_NEWLINE_INDENT_REDO,
                                  UNDO_BLOCK_START_REDO,
                                  UNDO_BLOCK_END_REDO};

undo_t::~undo_t() {}

undo_type_t undo_t::get_type() const { return type; }
undo_type_t undo_t::get_redo_type() const { return redo_map[type]; }

text_coordinate_t undo_t::get_start() { return start; }
std::string *undo_t::get_text() { return nullptr; }
std::string *undo_t::get_replacement() { return nullptr; }
text_coordinate_t undo_t::get_end() const { return text_coordinate_t(-1, -1); }
text_coordinate_t undo_t::get_new_end() const { return text_coordinate_t(-1, -1); }

void undo_single_text_t::add_newline() { text.append(1, '\n'); }
std::string *undo_single_text_t::get_text() { return &text; }
void undo_single_text_t::minimize() { text.reserve(0); }

text_coordinate_t undo_single_text_double_coord_t::get_end() const { return end; }

std::string *undo_double_text_t::get_replacement() { return &replacement; }
void undo_double_text_t::minimize() {
  undo_single_text_double_coord_t::minimize();
  replacement.reserve(0);
}

void undo_double_text_triple_coord_t::set_new_end(text_coordinate_t _new_end) {
  new_end = _new_end;
}
text_coordinate_t undo_double_text_triple_coord_t::get_new_end() const { return new_end; }

};  // namespace
