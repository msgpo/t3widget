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
#ifndef T3_WIDGET_WIDGETS_H
#define T3_WIDGET_WIDGETS_H

#include <deque>

#include "interfaces.h"

namespace t3_widget {

class widget_t : public virtual window_component_t {
	protected:
		bool redraw;

		widget_t(container_t *parent, int height, int width);
		widget_t(void);

		void init_window(container_t *parent, int height, int width);

	public:
		virtual ~widget_t(void);
		virtual bool is_hotkey(key_t key);
		virtual bool accepts_focus(void);
		virtual void set_position(optint top, optint left);
		virtual void show(void);
		virtual void hide(void);
		virtual void set_anchor(window_component_t *anchor, int relation);
		virtual void force_redraw(void);
};

class complex_widget_t : public widget_t {
	protected:
		window_component_t *center_window;
	public:
		complex_widget_t(container_t *parent, int height, int width) : widget_t(parent, height, width), center_window(this) {}
		virtual void set_center_window(window_component_t *_center_window) { center_window = _center_window; }
};

typedef std::deque<widget_t *> widgets_t;

}; // namespace
#endif
