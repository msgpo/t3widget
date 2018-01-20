/* Copyright (C) 2011-2013 G.P. Halkes
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
#ifndef T3_WIDGET_TEXTWINDOW_H
#define T3_WIDGET_TEXTWINDOW_H

#include <t3widget/interfaces.h>
#include <t3widget/textbuffer.h>
#include <t3widget/widgets/scrollbar.h>
#include <t3widget/widgets/widget.h>
#include <t3widget/wrapinfo.h>

namespace t3_widget {

class T3_WIDGET_API text_window_t : public widget_t, public center_component_t, public container_t {
 private:
  struct T3_WIDGET_LOCAL implementation_t {
    cleanup_ptr<scrollbar_t>::t scrollbar;
    text_buffer_t *text;
    cleanup_ptr<wrap_info_t>::t wrap_info;
    text_coordinate_t top;
    bool focus;

    implementation_t() : top(0, 0), focus(false) {}
  };
  pimpl_ptr<implementation_t>::t impl;

  void scroll_up(int lines);
  void scroll_down(int lines);
  void scrollbar_clicked(scrollbar_t::step_t step);
  void scrollbar_dragged(int start);

 public:
  text_window_t(text_buffer_t *_text = NULL, bool with_scrollbar = true);
  bool process_key(key_t key) override;
  bool set_size(optint height, optint width) override;
  void update_contents() override;
  void set_focus(focus_t _focus) override;
  void set_child_focus(window_component_t *target) override;
  bool is_child(window_component_t *component) override;
  bool process_mouse_event(mouse_event_t event) override;

  void set_scrollbar(bool with_scrollbar);
  void set_text(text_buffer_t *_text);
  text_buffer_t *get_text();
  void set_tabsize(int size);
  int get_text_height();

  T3_WIDGET_SIGNAL(activate, void);
};

};  // namespace
#endif
