/*
	Copyright (C) 2009 - 2025
	by Mark de Wever <koraq@xs4all.nl>
	Part of the Battle for Wesnoth Project https://www.wesnoth.org/

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.
	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY.

	See the COPYING file for more details.
*/

#define GETTEXT_DOMAIN "wesnoth-lib"

#include "gui/dialogs/core_selection.hpp"

#include "gui/widgets/listbox.hpp"
#include "gui/widgets/multi_page.hpp"
#include "gui/widgets/window.hpp"

#include <functional>

namespace gui2::dialogs
{

REGISTER_DIALOG(core_selection)

void core_selection::core_selected()
{
	const int selected_row
			= find_widget<listbox>("core_list")
					  .get_selected_row();

	multi_page& pages
			= find_widget<multi_page>("core_details");

	pages.select_page(selected_row);
}

void core_selection::pre_show()
{
	/***** Setup core list. *****/
	listbox& list = find_widget<listbox>("core_list");

	connect_signal_notify_modified(list, std::bind(&core_selection::core_selected, this));

	keyboard_capture(&list);

	/***** Setup core details. *****/
	multi_page& pages
			= find_widget<multi_page>("core_details");

	for(const auto & core : cores_)
	{
		/*** Add list item ***/
		grid* grid = &list.add_row(widget_data{
			{ "image", {
				{ "label", core["image"] }
			}},
			{ "name", {
				{ "label", core["name"] }
			}}
		});
		assert(grid);

		/*** Add detail item ***/
		pages.add_page({
			{ "description", {
				{ "label", core["description"] },
				{ "use_markup", "true" }
			}}
		});
	}
	list.select_row(choice_, true);

	core_selected();
}

void core_selection::post_show()
{
	choice_ = find_widget<listbox>("core_list")
					  .get_selected_row();
}

} // namespace dialogs
