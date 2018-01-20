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
#include "widgets/bullet.h"
#include "colorscheme.h"

namespace t3_widget {

bullet_t::bullet_t(const signals::slot<bool> &_source)
    : widget_t(1, 1, false), source(_source), has_focus(false) {}

bool bullet_t::set_size(optint height, optint width) {
  (void)height;
  (void)width;
  return true;
}

bool bullet_t::process_key(key_t key) {
  (void)key;
  return false;
}

void bullet_t::update_contents() {
  t3_win_set_default_attrs(window, attributes.dialog);
  t3_win_set_paint(window, 0, 0);
  if (source())
    t3_win_addch(window, T3_ACS_DIAMOND, T3_ATTR_ACS | (has_focus ? T3_ATTR_REVERSE : 0));
  else
    t3_win_addch(window, ' ', has_focus ? T3_ATTR_REVERSE : 0);
}

void bullet_t::set_focus(focus_t focus) { has_focus = focus; }

};  // namespace
