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

#pragma once

#include "gui/widgets/panel.hpp"
#include "gui/widgets/selectable_item.hpp"

namespace gui2
{

namespace implementation
{
struct builder_toggle_panel;
}

// ------------ WIDGET -----------{

class toggle_panel : public panel, public selectable_item
{
public:
	explicit toggle_panel(const implementation::builder_toggle_panel& builder);

	/**
	 * Sets the members of the child controls.
	 *
	 * Sets the members for all controls which have the proper member id. See
	 * styled_widget::set_members for more info.
	 *
	 * @param data                Map with the key value pairs to set the
	 *                            members.
	 */
	void set_child_members(const widget_data& data);

	/***** ***** ***** ***** Inherited ***** ***** ***** *****/

	/** See @ref widget::find_at. */
	virtual widget* find_at(const point& coordinate,
							 const bool must_be_active) override;

	/** See @ref widget::find_at. */
	virtual const widget* find_at(const point& coordinate,
								   const bool must_be_active) const override;

	/** See @ref styled_widget::set_active. */
	virtual void set_active(const bool active) override;

	/** See @ref styled_widget::get_active. */
	virtual bool get_active() const override;

	/** See @ref styled_widget::get_state. */
	virtual unsigned get_state() const override;

	/**
	 * See @ref container_base::get_client_rect.
	 *
	 * @todo only due to the fact our definition is slightly different from
	 * panel_definition we need to override this function and do about the
	 * same, look at a way to 'fix' that.
	 */
	virtual rect get_client_rect() const override;

	/**
	 * See @ref container_base::border_space.
	 *
	 * @todo only due to the fact our definition is slightly different from
	 * panel_definition we need to override this function and do about the
	 * same, look at a way to 'fix' that.
	 */
	virtual point border_space() const override;

	/** Inherited from selectable_item */
	virtual unsigned get_value() const override
	{
		return state_num_;
	}

	/** Inherited from selectable_item */
	virtual void set_value(unsigned selected, bool fire_event = false) override;

	/** Inherited from selectable_item */
	virtual unsigned num_states() const override;

	/***** ***** ***** setters / getters for members ***** ****** *****/

	void set_retval(const int retval);

private:
	/**
	 * Possible states of the widget.
	 *
	 * Note the order of the states must be the same as defined in settings.hpp.
	 * Also note the internals do assume the order for 'up' and 'down' to be the
	 * same and also that 'up' is before 'down'. 'up' has no suffix, 'down' has
	 * the SELECTED suffix.
	 */
	enum state_t {
		ENABLED,
		DISABLED,
		FOCUSED,
		COUNT
	};

	void set_state(const state_t state);

	/**
	 * Current state of the widget.
	 *
	 * The state of the widget determines what to render and how the widget
	 * reacts to certain 'events'.
	 */
	state_t state_;

	/**
	 *	Usually 1 for selected and 0 for not selected, can also have higher values in tristate buttons.
	 */
	unsigned state_num_;

	/**
	 * The return value of the button.
	 *
	 * If this value is not 0 and the button is double clicked it sets the
	 * retval of the window and the window closes itself.
	 */
	int retval_;

	/** Mouse left double click callback */
	std::function<void(widget&)> callback_mouse_left_double_click_;

	/** See @ref widget::impl_draw_background. */
	virtual bool impl_draw_background() override;

	/** See @ref widget::impl_draw_foreground. */
	virtual bool impl_draw_foreground() override;

public:
	/** Static type getter that does not rely on the widget being constructed. */
	static const std::string& type();

private:
	/** Inherited from styled_widget, implemented by REGISTER_WIDGET. */
	virtual const std::string& get_control_type() const override;

	/***** ***** ***** signal handlers ***** ****** *****/

	void signal_handler_mouse_enter(const event::ui_event event, bool& handled);

	void signal_handler_mouse_leave(const event::ui_event event, bool& handled);

	void signal_handler_pre_left_button_click(const event::ui_event event);

	void signal_handler_left_button_click(const event::ui_event event,
										  bool& handled);

	void signal_handler_left_button_double_click(const event::ui_event event,
												 bool& handled);
};

// }---------- DEFINITION ---------{

struct toggle_panel_definition : public styled_widget_definition
{
	explicit toggle_panel_definition(const config& cfg);

	struct resolution : public resolution_definition
	{
		explicit resolution(const config& cfg);

		unsigned top_border;
		unsigned bottom_border;

		unsigned left_border;
		unsigned right_border;
	};
};

// }---------- BUILDER -----------{

namespace implementation
{

struct builder_toggle_panel : public builder_styled_widget
{
	explicit builder_toggle_panel(const config& cfg);

	using builder_styled_widget::build;

	virtual std::unique_ptr<widget> build() const override;

	builder_grid_ptr grid;

private:
	std::string retval_id_;
	int retval_;
};

} // namespace implementation

// }------------ END --------------

} // namespace gui2
