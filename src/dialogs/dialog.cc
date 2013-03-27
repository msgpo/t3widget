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
#include "colorscheme.h"
#include "main.h"
#include "dialogs/dialog.h"
#include "dialogs/mainwindow.h"
#include "internal.h"

using namespace std;
namespace t3_widget {

dialogs_t dialog_t::active_dialogs;
int dialog_t::dialog_depth;

dialog_t::dialog_t(int height, int width, const char *_title) : dialog_base_t(height, width, true), active(false), title(_title)
{
}

/** Create a new ::dialog_t.

    This constructor should only be called by ::main_window_base_t.
*/
dialog_t::dialog_t(void) : active(false), title(NULL) {}

void dialog_t::activate_dialog(void) {
	if (!active_dialogs.empty()) {
		if (this == active_dialogs.back())
			return;

		active_dialogs.back()->set_focus(window_component_t::FOCUS_OUT);
		if (active) {
			for (dialogs_t::iterator iter = active_dialogs.begin(); iter != active_dialogs.end(); iter++) {
				if (*iter == this) {
					active_dialogs.erase(iter);
					break;
				}
			}
		}
	}

	active = true;
	set_focus(window_component_t::FOCUS_SET);
	dialog_depth -= 2;
	t3_win_set_depth(window, dialog_depth);
	if (shadow_window != NULL)
		t3_win_set_depth(shadow_window, dialog_depth + 1);
	active_dialogs.push_back(this);
}

void dialog_t::deactivate_dialog(void) {
	this->active = false;
	if (this == active_dialogs.back()) {
		this->set_focus(window_component_t::FOCUS_OUT);
		active_dialogs.pop_back();
		active_dialogs.back()->set_focus(window_component_t::FOCUS_REVERT);
		dialog_depth = t3_win_get_depth(active_dialogs.back()->window);
		return;
	}

	for (dialogs_t::iterator iter = active_dialogs.begin(); iter != active_dialogs.end(); iter++) {
		if (*iter == this) {
			active_dialogs.erase(iter);
			break;
		}
	}
}

bool dialog_t::process_key(key_t key) {
	if ((key & EKEY_META) || key == EKEY_F10) {
		for (widgets_t::iterator iter = widgets.begin();
				iter != widgets.end(); iter++)
		{
			widget_container_t *widget_container;
			widget_t *hotkey_target;

			if (!(*iter)->is_enabled())
				continue;

			if ((*iter)->is_hotkey(key & ~EKEY_META)) {
				if ((*iter)->accepts_focus()) {
					(*current_widget)->set_focus(window_component_t::FOCUS_OUT);
					current_widget = iter;
					(*current_widget)->set_focus(window_component_t::FOCUS_SET);
				}
				if ((*iter)->process_key(EKEY_HOTKEY))
					return true;
			} else if ((widget_container = dynamic_cast<widget_container_t *>(*iter)) != NULL &&
					(hotkey_target = widget_container->is_child_hotkey(key)) != NULL)
			{
				if (hotkey_target->accepts_focus()) {
					(*current_widget)->set_focus(window_component_t::FOCUS_OUT);
					current_widget = iter;
					widget_container->set_child_focus(hotkey_target);
				}
				if ((*iter)->process_key(EKEY_HOTKEY))
					return true;
			}
		}
	}

	if ((*current_widget)->process_key(key))
		return true;

	switch (key) {
		case EKEY_ESC:
			close();
			break;
		case '\t':
			focus_next();
			break;
		case EKEY_SHIFT | '\t':
			focus_previous();
			break;
		default:
			return false;
	}
	return true;
}

void dialog_t::update_contents(void) {
	bool redraw_title = redraw;

	dialog_base_t::update_contents();

	if (redraw_title && title != NULL) {
		t3_win_set_paint(window, 0, 3);
		t3_win_addstr(window, " ", 0);
		t3_win_addstr(window, title, 0);
		t3_win_addstr(window, " ", 0);
	}
}

void dialog_t::show(void) {
	dialog_base_t::show();
	activate_dialog();
}

void dialog_t::hide(void) {
	deactivate_dialog();
	dialog_base_t::hide();
}

void dialog_t::close(void) {
	hide();
	closed();
}

}; // namespace
