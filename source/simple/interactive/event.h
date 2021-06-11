#ifndef SIMPLE_INTERACTIVE_EVENT_H
#define SIMPLE_INTERACTIVE_EVENT_H
#include "codes.h"
#include <chrono>
#include <variant>
#include <optional>
#include "simple/geom/vector.hpp"

namespace simple::interactive
{
	using int2 = simple::geom::vector<int, 2>;
	using float2 = simple::geom::vector<float, 2>;

	enum class keystate : uint8_t
	{
		pressed = SDL_PRESSED,
		released = SDL_RELEASED
	};

#if SDL_VERSION_ATLEAST(2,0,4)
	enum class wheel_direction : uint32_t
	{
		normal = SDL_MOUSEWHEEL_NORMAL,
		flipped = SDL_MOUSEWHEEL_FLIPPED
	};
#endif

	enum class mouse_button : uint8_t
	{
		left = SDL_BUTTON_LEFT,
		right = SDL_BUTTON_RIGHT,
		middle = SDL_BUTTON_MIDDLE,
		x1 = SDL_BUTTON_X1,
		x2 = SDL_BUTTON_X2
	};

	enum class mouse_button_mask : uint32_t
	{
		none = 0,
		left = SDL_BUTTON_LMASK,
		right = SDL_BUTTON_RMASK,
		middle = SDL_BUTTON_MMASK,
		x1 = SDL_BUTTON_X1MASK,
		x2 = SDL_BUTTON_X2MASK
	};
	using ::operator |;
	using ::operator &;
	using ::operator &&;

	constexpr uint32_t touch_mouse_id = SDL_TOUCH_MOUSEID;

	struct event_data
	{
		std::chrono::milliseconds timestamp;
	};

	struct window_event_data : public event_data
	{
		uint32_t window_id;
	};

	struct key_data : public window_event_data
	{
		enum keycode keycode;
		enum scancode scancode;
		keystate state;
		uint8_t repeat;
	};

	struct mouse_data : public window_event_data
	{
		uint32_t device_id;
		int2 position;
	};

	struct mouse_button_data : public mouse_data
	{
		mouse_button button;
		keystate state;
#if SDL_VERSION_ATLEAST(2,0,2)
		uint8_t clicks;
#endif
	};

	struct mouse_motion_data : public mouse_data
	{
		int2 motion;
		mouse_button_mask button_state;
	};

	struct mouse_wheel_data : public mouse_data
	{
#if SDL_VERSION_ATLEAST(2,0,4)
		wheel_direction direction;
#endif
	};

	struct text_input_data : public window_event_data
	{
		std::array<char, 32> text;
	};

	struct text_edit_data : public text_input_data
	{
		simple::support::range<int> edit_range;
	};

	struct pointer_data : public event_data
	{
		int64_t device_id;
		int64_t pointer_id;
		float2 position;
		float2 motion;
		float pressure;
	};

	struct key_event
	{
		const key_data data;
	};

	struct key_pressed : key_event
	{};

	struct key_released : key_event
	{};

	std::optional<float2> window_normalized_position(const mouse_data& data) noexcept;
	std::optional<float2> screen_normalized_position(const mouse_data& data) noexcept;
	std::optional<float2> window_normalized_motion(const mouse_motion_data& data) noexcept;
	std::optional<float2> screen_normalized_motion(const mouse_motion_data& data) noexcept;

	template<typename Data>
	struct mouse_position_event
	{
		const Data data;
		auto window_normalized_position() const noexcept { return interactive::window_normalized_position(data); }
		auto screen_normalized_position() const noexcept { return interactive::screen_normalized_position(data); }
	};

	struct mouse_motion : public mouse_position_event<mouse_motion_data>
	{
		auto window_normalized_motion() const noexcept { return interactive::window_normalized_motion(data); }
		auto screen_normalized_motion() const noexcept { return interactive::screen_normalized_motion(data); }
	};

	struct mouse_wheel
	{
		const mouse_wheel_data data;
#if SDL_VERSION_ATLEAST(2,0,4)
		int2 motion() const noexcept;
#endif
	};

	struct mouse_button_event : public mouse_position_event<mouse_button_data>
	{
	};

	struct mouse_down : public mouse_button_event
	{};

	struct mouse_up : public mouse_button_event
	{};

	struct text_input { const text_input_data data; };
	struct text_edit { const text_edit_data data; };

	struct pointer_motion { const pointer_data data; };
	struct pointer_down { const pointer_data data; };
	struct pointer_up { const pointer_data data; };

	struct quit_request { const event_data data; };

	// TODO: screen normalized position/size getters as with mouse?
	struct window_vector_data : public window_event_data { int2 value; };

	struct window_shown { const window_event_data data; };
	struct window_hidden { const window_event_data data; };
	struct window_exposed { const window_event_data data; };
	struct window_moved { const window_vector_data data; };
	struct window_resized { const window_vector_data data; };
	struct window_size_changed { const window_vector_data data; };
	struct window_minimized { const window_event_data data; };
	struct window_maximized { const window_event_data data; };
	struct window_restored { const window_event_data data; };
	struct window_entered { const window_event_data data; };
	struct window_left { const window_event_data data; };
	struct window_focus_gained { const window_event_data data; };
	struct window_focus_lost { const window_event_data data; };
	struct window_closed { const window_event_data data; };
#if SDL_VERSION_ATLEAST(2,0,5)
	struct window_take_focus { const window_event_data data; };
	struct window_hit_test { const window_event_data data; };
#endif

	using event = std::variant<
		key_pressed
		,key_released
		,mouse_down
		,mouse_up
		,mouse_motion
		,mouse_wheel
		,text_input
		,text_edit
		,pointer_motion
		,pointer_down
		,pointer_up
		,quit_request

		,window_shown
		,window_hidden
		,window_exposed
		,window_moved
		,window_resized
		,window_size_changed
		,window_minimized
		,window_maximized
		,window_restored
		,window_entered
		,window_left
		,window_focus_gained
		,window_focus_lost
		,window_closed
#if SDL_VERSION_ATLEAST(2,0,5)
		,window_take_focus
		,window_hit_test
#endif
	>;

	std::optional<event> next_event() noexcept;

	// better to use expected<bool, error>
	bool relative_mouse_mode() noexcept;
	bool relative_mouse_mode(bool enable) noexcept;
	void require_relative_mouse_mode(bool enable);

#if SDL_VERSION_ATLEAST(2,0,4)
	// better to use expected<bool, error>
	bool mouse_capture(bool enable) noexcept;
	void require_mouse_capture(bool enable);
#endif

} // namespace simple::interactive

template<> struct simple::support::define_enum_flags_operators<simple::interactive::mouse_button_mask>
	: std::true_type {};

#endif /* end of include guard */
