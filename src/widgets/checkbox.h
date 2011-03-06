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
#ifndef CHECKBOX_H
#define CHECKBOX_H

#include "widgets.h"

class checkbox_t : public widget_t {
	private:
		bool state, has_focus;
		smart_label_t *label;

	public:
		checkbox_t(container_t *parent, bool _state = false);
		virtual bool process_key(key_t key);
		virtual bool set_size(optint height, optint width);
		virtual void update_contents(void);
		virtual void set_focus(bool focus);
		bool get_state(void);
		void set_state(bool _state);
		void set_label(smart_label_t *_label);
		virtual bool is_hotkey(key_t key);

	SIGNAL(activate, void);
	SIGNAL(toggled, void);
	SIGNAL(move_focus_left, void);
	SIGNAL(move_focus_right, void);
	SIGNAL(move_focus_up, void);
	SIGNAL(move_focus_down, void);
};

#endif
