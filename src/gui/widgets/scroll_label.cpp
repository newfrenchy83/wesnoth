/*
	Copyright (C) 2008 - 2025
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

#include "gui/widgets/scroll_label.hpp"

#include "gui/widgets/label.hpp"
#include "gui/core/log.hpp"
#include "gui/core/window_builder/helper.hpp"
#include "gui/core/register_widget.hpp"
#include "gui/widgets/window.hpp"
#include "gettext.hpp"
#include "wml_exception.hpp"

#include <functional>

#define LOG_SCOPE_HEADER get_control_type() + " [" + id() + "] " + __func__
#define LOG_HEADER LOG_SCOPE_HEADER + ':'

namespace gui2
{

// ------------ WIDGET -----------{

REGISTER_WIDGET(scroll_label)

scroll_label::scroll_label(const implementation::builder_scroll_label& builder)
	: scrollbar_container(builder, type())
	, state_(ENABLED)
	, wrap_on_(builder.wrap_on)
	, text_alignment_(builder.text_alignment)
	, link_aware_(builder.link_aware)
{
	connect_signal<event::LEFT_BUTTON_DOWN>(
		std::bind(&scroll_label::signal_handler_left_button_down, this, std::placeholders::_2),
		event::dispatcher::back_pre_child);
}

label* scroll_label::get_internal_label()
{
	if(content_grid()) {
		return dynamic_cast<label*>(content_grid()->find("_label", false));
	}

	return nullptr;
}

void scroll_label::set_label(const t_string& lbl)
{
	// Inherit.
	styled_widget::set_label(lbl);

	if(label* widget = get_internal_label()) {
		widget->set_label(lbl);

		bool resize_needed = !content_resize_request();
		if(resize_needed && get_size() != point()) {
			place(get_origin(), get_size());
		}
	}
}

void scroll_label::set_text_alignment(const PangoAlignment text_alignment)
{
	// Inherit.
	styled_widget::set_text_alignment(text_alignment);

	text_alignment_ = text_alignment;

	if(label* widget = get_internal_label()) {
		widget->set_text_alignment(text_alignment_);
	}
}

void scroll_label::set_use_markup(bool use_markup)
{
	// Inherit.
	styled_widget::set_use_markup(use_markup);

	if(label* widget = get_internal_label()) {
		widget->set_use_markup(use_markup);
	}
}

void scroll_label::set_text_alpha(unsigned short alpha)
{
	if(label* widget = get_internal_label()) {
		widget->set_text_alpha(alpha);
	}
}

void scroll_label::set_link_aware(bool l)
{
	link_aware_ = l;

	if(label* widget = get_internal_label()) {
		widget->set_link_aware(l);
	}
}

void scroll_label::set_self_active(const bool active)
{
	state_ = active ? ENABLED : DISABLED;
}

bool scroll_label::get_active() const
{
	return state_ != DISABLED;
}

unsigned scroll_label::get_state() const
{
	return state_;
}

void scroll_label::finalize_subclass()
{
	label* lbl = get_internal_label();
	assert(lbl);

	lbl->set_label(get_label());
	lbl->set_can_wrap(wrap_on_);
	lbl->set_text_alignment(text_alignment_);
	lbl->set_link_aware(link_aware_);
	lbl->set_use_markup(get_use_markup());
}

void scroll_label::set_can_wrap(bool can_wrap)
{
	label* lbl = get_internal_label();
	assert(lbl);

	wrap_on_ = can_wrap;
	lbl->set_can_wrap(wrap_on_);
}

bool scroll_label::can_wrap() const
{
	return wrap_on_;
}

void scroll_label::signal_handler_left_button_down(const event::ui_event event)
{
	DBG_GUI_E << LOG_HEADER << ' ' << event << ".";

	get_window()->keyboard_capture(this);
}

// }---------- DEFINITION ---------{

scroll_label_definition::scroll_label_definition(const config& cfg)
	: styled_widget_definition(cfg)
{
	DBG_GUI_P << "Parsing scroll label " << id;

	load_resolutions<resolution>(cfg);
}

scroll_label_definition::resolution::resolution(const config& cfg)
	: resolution_definition(cfg), grid(nullptr)
{
	// Note the order should be the same as the enum state_t is scroll_label.hpp.
	state.emplace_back(VALIDATE_WML_CHILD(cfg, "state_enabled", missing_mandatory_wml_tag("scroll_label_definition][resolution", "state_enabled")));
	state.emplace_back(VALIDATE_WML_CHILD(cfg, "state_disabled", missing_mandatory_wml_tag("scroll_label_definition][resolution", "state_disabled")));

	auto child = VALIDATE_WML_CHILD(cfg, "grid", missing_mandatory_wml_tag("scroll_label_definition][resolution", "grid"));
	grid = std::make_shared<builder_grid>(child);
}

// }---------- BUILDER -----------{

namespace implementation
{

builder_scroll_label::builder_scroll_label(const config& cfg)
	: builder_scrollbar_container(cfg)
	, wrap_on(cfg["wrap"].to_bool(true))
	, text_alignment(decode_text_alignment(cfg["text_alignment"]))
	, link_aware(cfg["link_aware"].to_bool(false))
{
}

std::unique_ptr<widget> builder_scroll_label::build() const
{
	auto widget = std::make_unique<scroll_label>(*this);

	const auto conf = widget->cast_config_to<scroll_label_definition>();
	assert(conf);

	widget->init_grid(*conf->grid);
	widget->finalize_setup();

	DBG_GUI_G << "Window builder: placed scroll label '" << id
			  << "' with definition '" << definition << "'.";

	return widget;
}

} // namespace implementation

// }------------ END --------------

} // namespace gui2
