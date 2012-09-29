/* Copyright (C) 2012 G.P. Halkes
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
#include <stdio.h>
#include "dialogs/attributepickerdialog.h"
#include "widgets/frame.h"
#include "colorscheme.h"

#define ATTRIBUTE_PICKER_DIALOG_HEIGHT 18
#define ATTRIBUTE_PICKER_DIALOG_WIDTH 43

namespace t3_widget {

attribute_picker_dialog_t::attribute_picker_dialog_t(const char *_title) :
	dialog_t(ATTRIBUTE_PICKER_DIALOG_HEIGHT, ATTRIBUTE_PICKER_DIALOG_WIDTH, _title),
	impl(new implementation_t())
{
	smart_label_t *underline_label, *bold_label, *dim_label, *reverse_label, *blink_label;
	frame_t *test_line_frame;

	impl->underline_box = new checkbox_t(false);
	impl->underline_box->set_position(1, 2);
	underline_label = new smart_label_t("_Underline");
	underline_label->set_anchor(impl->underline_box, T3_PARENT(T3_ANCHOR_TOPRIGHT) | T3_CHILD(T3_ANCHOR_TOPLEFT));
	underline_label->set_position(0, 1);
	impl->underline_box->connect_move_focus_up(sigc::mem_fun(this, &attribute_picker_dialog_t::focus_previous));
	impl->underline_box->connect_move_focus_down(sigc::mem_fun(this, &attribute_picker_dialog_t::focus_next));
	impl->underline_box->set_label(underline_label);
	impl->underline_box->connect_toggled(sigc::mem_fun(this, &attribute_picker_dialog_t::attribute_changed));
	impl->underline_box->connect_activate(sigc::mem_fun0(this, &attribute_picker_dialog_t::ok_activate));


	impl->bold_box = new checkbox_t(false);
	impl->bold_box->set_anchor(impl->underline_box, T3_PARENT(T3_ANCHOR_TOPLEFT) | T3_CHILD(T3_ANCHOR_TOPLEFT));
	impl->bold_box->set_position(1, 0);
	bold_label = new smart_label_t("_Bold");
	bold_label->set_anchor(impl->bold_box, T3_PARENT(T3_ANCHOR_TOPRIGHT) | T3_CHILD(T3_ANCHOR_TOPLEFT));
	bold_label->set_position(0, 1);
	impl->bold_box->connect_move_focus_up(sigc::mem_fun(this, &attribute_picker_dialog_t::focus_previous));
	impl->bold_box->connect_move_focus_down(sigc::mem_fun(this, &attribute_picker_dialog_t::focus_next));
	impl->bold_box->connect_toggled(sigc::mem_fun(this, &attribute_picker_dialog_t::attribute_changed));
	impl->bold_box->connect_activate(sigc::mem_fun0(this, &attribute_picker_dialog_t::ok_activate));


	impl->dim_box = new checkbox_t(false);
	impl->dim_box->set_anchor(impl->bold_box, T3_PARENT(T3_ANCHOR_TOPLEFT) | T3_CHILD(T3_ANCHOR_TOPLEFT));
	impl->dim_box->set_position(1, 0);
	dim_label = new smart_label_t("_Dim");
	dim_label->set_anchor(impl->dim_box, T3_PARENT(T3_ANCHOR_TOPRIGHT) | T3_CHILD(T3_ANCHOR_TOPLEFT));
	dim_label->set_position(0, 1);
	impl->dim_box->connect_move_focus_up(sigc::mem_fun(this, &attribute_picker_dialog_t::focus_previous));
	impl->dim_box->connect_move_focus_down(sigc::mem_fun(this, &attribute_picker_dialog_t::focus_next));
	impl->dim_box->connect_toggled(sigc::mem_fun(this, &attribute_picker_dialog_t::attribute_changed));
	impl->dim_box->connect_activate(sigc::mem_fun0(this, &attribute_picker_dialog_t::ok_activate));


	impl->reverse_box = new checkbox_t(false);
	impl->reverse_box->set_anchor(impl->dim_box, T3_PARENT(T3_ANCHOR_TOPLEFT) | T3_CHILD(T3_ANCHOR_TOPLEFT));
	impl->reverse_box->set_position(1, 0);
	reverse_label = new smart_label_t("_Reverse video");
	reverse_label->set_anchor(impl->reverse_box, T3_PARENT(T3_ANCHOR_TOPRIGHT) | T3_CHILD(T3_ANCHOR_TOPLEFT));
	reverse_label->set_position(0, 1);
	impl->reverse_box->connect_move_focus_up(sigc::mem_fun(this, &attribute_picker_dialog_t::focus_previous));
	impl->reverse_box->connect_move_focus_down(sigc::mem_fun(this, &attribute_picker_dialog_t::focus_next));
	impl->reverse_box->connect_toggled(sigc::mem_fun(this, &attribute_picker_dialog_t::attribute_changed));
	impl->reverse_box->connect_activate(sigc::mem_fun0(this, &attribute_picker_dialog_t::ok_activate));


	impl->blink_box = new checkbox_t(false);
	impl->blink_box->set_anchor(impl->reverse_box, T3_PARENT(T3_ANCHOR_TOPLEFT) | T3_CHILD(T3_ANCHOR_TOPLEFT));
	impl->blink_box->set_position(1, 0);
	blink_label = new smart_label_t("Bl_ink");
	blink_label->set_anchor(impl->blink_box, T3_PARENT(T3_ANCHOR_TOPRIGHT) | T3_CHILD(T3_ANCHOR_TOPLEFT));
	blink_label->set_position(0, 1);
	impl->blink_box->connect_move_focus_up(sigc::mem_fun(this, &attribute_picker_dialog_t::focus_previous));
	impl->blink_box->connect_move_focus_down(sigc::mem_fun(this, &attribute_picker_dialog_t::focus_next));
	impl->blink_box->connect_toggled(sigc::mem_fun(this, &attribute_picker_dialog_t::attribute_changed));
	impl->blink_box->connect_activate(sigc::mem_fun0(this, &attribute_picker_dialog_t::ok_activate));

	//FIXME: need to make the text and size configurable etc.
	test_line_frame = new frame_t();
	test_line_frame->set_anchor(this, T3_PARENT(T3_ANCHOR_TOPRIGHT) | T3_CHILD(T3_ANCHOR_TOPRIGHT));
	test_line_frame->set_position(1, -2);
	impl->test_line = new test_line_t(4, "Text");
	test_line_frame->set_size(3, 6);
	test_line_frame->set_child(impl->test_line);

	impl->fg_picker = new color_picker_t(true);
	impl->fg_picker->set_anchor(impl->blink_box, T3_PARENT(T3_ANCHOR_TOPLEFT) | T3_CHILD(T3_ANCHOR_TOPLEFT));
	impl->fg_picker->set_position(1, 0);
	impl->fg_picker->connect_selection_changed(sigc::mem_fun(this, &attribute_picker_dialog_t::attribute_changed));

	push_back(impl->underline_box);
	push_back(underline_label);
	push_back(impl->bold_box);
	push_back(bold_label);
	push_back(impl->dim_box);
	push_back(dim_label);
	push_back(impl->reverse_box);
	push_back(reverse_label);
	push_back(impl->blink_box);
	push_back(blink_label);
	push_back(impl->fg_picker);
	push_back(test_line_frame);
}

void attribute_picker_dialog_t::attribute_changed(void) {
	impl->test_line->set_attribute(get_attribute());
}

void attribute_picker_dialog_t::ok_activate(void) {
	attribute_selected(get_attribute());
}

t3_attr_t attribute_picker_dialog_t::get_attribute(void) {
	t3_attr_t result = 0;
	if (impl->underline_box->get_state())
		result |= T3_ATTR_UNDERLINE;
	if (impl->bold_box->get_state())
		result |= T3_ATTR_BOLD;
	if (impl->dim_box->get_state())
		result |= T3_ATTR_DIM;
	if (impl->blink_box->get_state())
		result |= T3_ATTR_BLINK;
	if (impl->reverse_box->get_state())
		result |= T3_ATTR_REVERSE;
	result |= impl->fg_picker->get_color();
	return result;
}

void attribute_picker_dialog_t::set_attribute(t3_attr_t attr) {
	impl->underline_box->set_state(attr & T3_ATTR_UNDERLINE);
	impl->bold_box->set_state(attr & T3_ATTR_BOLD);
	impl->blink_box->set_state(attr & T3_ATTR_BLINK);
	impl->reverse_box->set_state(attr & T3_ATTR_REVERSE);
}


//================================================================================
attribute_picker_dialog_t::test_line_t::test_line_t(int width, const char *_text) : widget_t(1, width), text(_text), attr(0) {}

bool attribute_picker_dialog_t::test_line_t::process_key(key_t key) {
	(void) key;
	return false;
}

bool attribute_picker_dialog_t::test_line_t::set_size(optint height, optint width) {
	(void) height;
	if (width.is_valid())
		return t3_win_resize(window, 1, width);
	return true;
}

void attribute_picker_dialog_t::test_line_t::update_contents(void) {
	if (!redraw)
		return;
	t3_win_set_default_attrs(window, attr);
	t3_win_set_paint(window, 0, 0);
	t3_win_clrtoeol(window);
	t3_win_set_paint(window, 0, 0);
	t3_win_addstr(window, text.c_str(), 0);
}

bool attribute_picker_dialog_t::test_line_t::accepts_focus(void) { return false; }

void attribute_picker_dialog_t::test_line_t::set_attribute(t3_attr_t _attr) {
	redraw = true;
	attr = _attr;
}

//================================================================================

#warning FIXME: need to handle changes in number of colors
//FIXME: handle terminals which only do color pairs, although maybe it is better to make a separate widget for that
#define COLORS_PER_LINE 36
attribute_picker_dialog_t::color_picker_t::color_picker_t(bool _fg) : current_color(-2), fg(_fg) {
	t3_term_caps_t terminal_capabilities;
	t3_term_get_caps(&terminal_capabilities);

	max_color = terminal_capabilities.colors - 1;

	init_window((max_color + 2 + COLORS_PER_LINE - 1) / COLORS_PER_LINE + 2, COLORS_PER_LINE + 2);
}

bool attribute_picker_dialog_t::color_picker_t::process_key(key_t key) {
	int start_color = current_color;
	switch (key) {
		case EKEY_UP:
			if (current_color >= 16 + COLORS_PER_LINE) {
				current_color -= COLORS_PER_LINE;
			} else if (current_color >= 16) {
				/* Subtract 18 because we have "undefined" and "default" before
				   the actual colors. */
				current_color -= 18;
				if (current_color > 16)
					current_color = 15;
			}
			break;
		case EKEY_DOWN:
			if (current_color < 16 && current_color + 18 <= max_color)
				/* Add 18 because we have "undefined" and "default" before
				   the actual colors. */
				current_color += 18;
			else if (current_color + COLORS_PER_LINE <= max_color)
				current_color += COLORS_PER_LINE;
			else if ((max_color - 16) / COLORS_PER_LINE != (current_color - 16) / COLORS_PER_LINE)
				current_color = max_color;
			break;
		case EKEY_RIGHT:
			if (current_color < max_color)
				current_color++;
			break;
		case EKEY_LEFT:
			if (current_color > -2)
				current_color--;
			break;
		case EKEY_HOME:
			current_color = -2;
			break;
		case EKEY_END:
			current_color = max_color;
			break;
		case EKEY_NL:
			activated();
			break;
		default:
			return false;
	}
	if (current_color != start_color)
		selection_changed();
	return true;
}

bool attribute_picker_dialog_t::color_picker_t::set_size(optint height, optint width) {
	(void) height;
	(void) width;
	return true;
}

void attribute_picker_dialog_t::color_picker_t::update_contents(void) {
	int i, max;
	t3_term_caps_t terminal_capabilities;

	if (!redraw)
		return;

	t3_term_get_caps(&terminal_capabilities);

	t3_win_set_default_attrs(window, attributes.dialog);
	t3_win_set_paint(window, 0, 0);
	t3_win_clrtobot(window);
	t3_win_box(window, 0, 0, t3_win_get_height(window), t3_win_get_width(window), 0);
	t3_win_set_paint(window, 1, 1);
	t3_win_addch(window, ' ', 0);
	t3_win_addch(window, ' ', T3_ATTR_BG_DEFAULT | (fg ? T3_ATTR_REVERSE : 0));

	max = max_color + 1 < 16 ? max_color + 1 : 16;

	for (i = 0; i < max; i++)
		t3_win_addch(window, ' ', T3_ATTR_BG(i));
	t3_win_addch(window, T3_ACS_VLINE, T3_ATTR_ACS);

	if (max_color >= 16) {
		max = terminal_capabilities.colors < 256 ? terminal_capabilities.colors : 256;
		for (i = 16; i < max; i++) {
			t3_win_set_paint(window, (i - 16) / COLORS_PER_LINE + 2, (i - 16) % COLORS_PER_LINE + 1);
			t3_win_addch(window, ' ', T3_ATTR_BG(i));
		}
		t3_win_addch(window, T3_ACS_VLINE, T3_ATTR_ACS);
	}

	if (current_color == -2) {
		t3_win_set_paint(window, 1, 1);
		t3_win_addch(window, T3_ACS_DIAMOND, T3_ATTR_ACS);
		t3_win_set_paint(window, 0, 1);
		t3_win_addch(window, T3_ACS_DARROW, T3_ATTR_ACS);
		t3_win_set_paint(window, 1, 0);
		t3_win_addch(window, T3_ACS_RARROW, T3_ATTR_ACS);
	} else if (current_color == -1) {
		t3_win_set_paint(window, 1, 2);
		t3_win_addch(window, T3_ACS_DIAMOND, T3_ATTR_ACS | T3_ATTR_BG_DEFAULT | (fg ? T3_ATTR_REVERSE : 0));
		t3_win_set_paint(window, 0, 2);
		t3_win_addch(window, T3_ACS_DARROW, T3_ATTR_ACS);
		t3_win_set_paint(window, 1, 0);
		t3_win_addch(window, T3_ACS_RARROW, T3_ATTR_ACS);
	} else if (current_color < 16) {
		t3_win_set_paint(window, 1, current_color + 3);
		t3_win_addch(window, T3_ACS_DIAMOND, T3_ATTR_ACS | T3_ATTR_BG(current_color));
		t3_win_set_paint(window, 0, current_color + 3);
		t3_win_addch(window, T3_ACS_DARROW, T3_ATTR_ACS);
		t3_win_set_paint(window, 1, 0);
		t3_win_addch(window, T3_ACS_RARROW, T3_ATTR_ACS);
	} else {
		t3_win_set_paint(window, (current_color - 16) / COLORS_PER_LINE + 2, (current_color - 16) % COLORS_PER_LINE + 1);
		t3_win_addch(window, T3_ACS_DIAMOND, T3_ATTR_ACS | T3_ATTR_BG(current_color));
		t3_win_set_paint(window, 0, (current_color - 16) % COLORS_PER_LINE + 1);
		t3_win_addch(window, T3_ACS_DARROW, T3_ATTR_ACS);
		t3_win_set_paint(window, (current_color - 16) / COLORS_PER_LINE + 2, 0);
		t3_win_addch(window, T3_ACS_RARROW, T3_ATTR_ACS);
	}
	t3_win_set_paint(window, t3_win_get_height(window) - 1, 1);
	t3_win_addstr(window, " Color: ", 0);
	if (current_color == -2) {
		t3_win_addstr(window, "Undefined", 0);
	} else if (current_color == -1) {
		t3_win_addstr(window, "Terminal default", 0);
	} else if (current_color < 16) {
		static const char *color_to_text[] = {
			"Black", "Red", "Green", "Yellow", "Blue", "Magenta", "Cyan", "Gray",
			"Dark gray", "Light red", "Light green", "Light yellow", "Light blue",
			"Light magenta", "Light cyan", "White" };
		t3_win_addstr(window, color_to_text[current_color], 0);
	} else {
		char color_number[20];
		sprintf(color_number, "%d", current_color);
		t3_win_addstr(window, color_number, 0);
	}
	t3_win_addch(window, ' ', 0);
}

int attribute_picker_dialog_t::color_picker_t::xy_to_color(int x, int y) {
	int color;
	if (x == 0 || x == t3_win_get_width(window) - 1 || y == 0 || y == t3_win_get_height(window) - 1)
		return INT_MIN;
	if (y == 1) {
		color = x - 3;
		if (color > 16 || color > max_color || current_color == color)
			return INT_MIN;
	} else {
		color = 16 + (y - 2) * COLORS_PER_LINE + x - 1;
		if (color > max_color)
			return INT_MIN;
	}
	return color;
}

bool attribute_picker_dialog_t::color_picker_t::process_mouse_event(mouse_event_t event) {
	int new_color;

	if (event.window != window)
		return true;
	if (event.button_state & EMOUSE_CLICKED_LEFT) {
		new_color = xy_to_color(event.x, event.y);

		if (new_color == INT_MIN)
			return true;

		current_color = new_color;
		redraw = true;
		selection_changed();
		if (event.button_state & EMOUSE_DOUBLE_CLICKED_LEFT)
			activated();
	}
	return true;
}

t3_attr_t attribute_picker_dialog_t::color_picker_t::get_color(void) {
	return fg ? (current_color >= 0 ? T3_ATTR_FG(current_color) : (current_color == -1 ? T3_ATTR_FG_DEFAULT : 0)) :
		(current_color >= 0 ? T3_ATTR_BG(current_color) : (current_color == -1 ? T3_ATTR_BG_DEFAULT : 0));
}

}; // namespace
