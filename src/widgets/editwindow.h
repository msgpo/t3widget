/* Copyright (C) 2010 G.P. Halkes
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
#ifndef T3_WIDGET_EDITWINDOW_H
#define T3_WIDGET_EDITWINDOW_H

namespace t3_widget {
	class edit_window_t;
}; // namespace

#include "dialogs/gotodialog.h"
#include "dialogs/finddialog.h"
#include "widgets/widget.h"
#include "widgets/scrollbar.h"
#include "textbuffer.h"
#include "findcontext.h"
#include "key.h"
#include "interfaces.h"

namespace t3_widget {

class edit_window_t : public widget_t, public center_component_t, public container_t {
	protected:
		static goto_dialog_t *goto_dialog;
		static sigc::connection goto_connection;
		static find_dialog_t *global_find_dialog;
		static sigc::connection global_find_dialog_connection;
		static finder_t global_finder;
		static replace_buttons_dialog_t *replace_buttons;
		static sigc::connection replace_buttons_connection;
		static bool init_connected;

		t3_window_t *edit_window, *bottom_line_window;
		scrollbar_t scrollbar;
		text_buffer_t *text;
		int screen_pos; // Cached position of cursor in screen coordinates
		bool focus;
		find_dialog_t *find_dialog;
		finder_t *finder;

		static void init(void);
		static const char *ins_string[];
		static bool (text_buffer_t::*proces_char[])(key_t);

		void ensure_cursor_on_screen(void);
		void repaint_screen(void);
		void inc_x(void);
		void next_word(void);
		void dec_x(void);
		void previous_word(void);
		void inc_y(void);
		void dec_y(void);
		void pgdn(void);
		void pgup(void);
		void reset_selection(void);
		void set_selection_mode(key_t key);
		void delete_selection(void);

		void find_activated(find_action_t action, finder_t *finder);

	public:
		edit_window_t(text_buffer_t *_text = NULL);
		virtual ~edit_window_t(void);
		virtual void set_text(text_buffer_t *_text);
		virtual bool process_key(key_t key);
		virtual bool set_size(optint height, optint width);
		virtual void update_contents(void);
		virtual void set_focus(bool focus);
		virtual void force_redraw(void);

		void goto_line(int line);

		void get_dimensions(int *height, int *width, int *top, int *left);
		bool get_selection_lines(int *top, int *bottom);

		void undo(void);
		void redo(void);
		void cut_copy(bool cut);
		void paste(void);
		void select_all(void);
		void insert_special(void);
		void goto_line(void);
		void find_replace(bool replace);
		void find_next(bool backward);
		text_buffer_t *get_text(void);
		void set_find_dialog(find_dialog_t *_find_dialog);
		void set_finder(finder_t *_finder);
};

}; // namespace
#endif
