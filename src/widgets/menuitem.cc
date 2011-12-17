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
#include "colorscheme.h"
#include "widgets/menuitem.h"

namespace t3_widget {

void menu_item_base_t::set_position(optint top, optint left) {
	(void) left;
	if (!top.is_valid())
		return;
	t3_win_move(window, top, 1);
}

bool menu_item_base_t::set_size(optint height, optint width) {
	(void) height;
	if (!width.is_valid())
		return true;
	redraw = true;
	return t3_win_resize(window, 1, width);
}

menu_item_t::menu_item_t(menu_panel_t *_parent, const char *_label, const char *_hotkey, int _id) :
		menu_item_base_t(_parent), label(new smart_label_t(_label)), hotkey(_hotkey), id(_id)
{
	has_focus = false;
}

bool menu_item_t::process_key(key_t key) {
	switch (key) {
		case EKEY_NL:
		case ' ':
		case EKEY_HOTKEY:
			parent->close();
			parent->signal(id);
			break;
		default:
			return false;
	}
	return true;
}

void menu_item_t::update_contents(void) {
	int spaces;

	if (!redraw)
		return;
	redraw = false;

	t3_win_set_paint(window, 0, 0);
	t3_win_set_default_attrs(window, has_focus ? attributes.dialog_selected: attributes.dialog);
	t3_win_addch(window, ' ', 0);
	label->draw(window, 0, has_focus);

	spaces = t3_win_get_width(window) - 3 - label->get_width();
	if (hotkey != NULL) {
		spaces -= t3_term_strwidth(hotkey) - 1;
		t3_win_addchrep(window, ' ', 0, spaces);
		t3_win_addstr(window, hotkey, 0);
		t3_win_addch(window, ' ', 0);
	} else {
		t3_win_addchrep(window, ' ', 0, spaces);
	}
}

void menu_item_t::set_focus(bool focus) {
	menu_item_base_t::set_focus(focus);
	if (focus != has_focus)
		redraw = true;
	has_focus = focus;
}

void menu_item_t::show(void) {}
void menu_item_t::hide(void) {}

bool menu_item_t::is_hotkey(key_t key) {
	return label->is_hotkey(key);
}

bool menu_item_t::process_mouse_event(mouse_event_t event) {
	if (event.button_state & EMOUSE_CLICKED_LEFT) {
		parent->close();
		parent->signal(id);
	}
	return true;
}

int menu_item_t::get_label_width(void) {
	return label->get_width() + 2;
}

int menu_item_t::get_hotkey_width(void) {
	return hotkey == NULL ? 0 : (t3_term_strwidth(hotkey) + 2);
}

menu_separator_t::menu_separator_t(menu_panel_t *_parent) : menu_item_base_t(_parent) {}

bool menu_separator_t::process_key(key_t key) {
	(void) key;
	return false;
}

void menu_separator_t::update_contents(void) {
	if (!redraw)
		return;
	redraw = false;
	t3_win_set_paint(window, 0, 0);
	t3_win_addchrep(window, T3_ACS_HLINE, T3_ATTR_ACS | attributes.dialog, t3_win_get_width(window));
}

void menu_separator_t::set_focus(bool focus) {
	(void) focus;
}

void menu_separator_t::show(void) {}
void menu_separator_t::hide(void) {}

bool menu_separator_t::accepts_focus(void) {
	return false;
}

}; // namespace
