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
#ifndef T3_WIDGET_SEPARATOR_H
#define T3_WIDGET_SEPARATOR_H

#include <t3widget/widgets/widget.h>

namespace t3_widget {

class T3_WIDGET_API separator_t : public widget_t {
 private:
  bool horizontal;

 public:
  separator_t(bool _horizontal = true);
  bool process_key(key_t key) override;
  bool set_size(optint height, optint width) override;
  void update_contents() override;
  void set_focus(focus_t focus) override;
  bool accepts_focus() override;
};

};  // namespace
#endif
