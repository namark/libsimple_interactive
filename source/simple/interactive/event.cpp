#include "event.h"
#include "simple/sdlcore/utils.hpp"

namespace simple::interactive
{

	std::optional<event> next_event() noexcept
	{
		SDL_Event event;
		while(SDL_PollEvent(&event))
		{
			switch(event.type)
			{
				case SDL_KEYDOWN:
					return key_pressed
					{
						event.key.windowID,
						std::chrono::milliseconds(event.key.timestamp),
						static_cast<keycode>(event.key.keysym.sym),
						static_cast<scancode>(event.key.keysym.scancode),
						static_cast<keystate>(event.key.state),
						event.key.repeat
					};
				case SDL_KEYUP:
					return key_released
					{
						event.key.windowID,
						std::chrono::milliseconds(event.key.timestamp),
						static_cast<keycode>(event.key.keysym.sym),
						static_cast<scancode>(event.key.keysym.scancode),
						static_cast<keystate>(event.key.state),
						event.key.repeat
					};
				case SDL_MOUSEBUTTONDOWN:
					return mouse_down
					{
						event.button.windowID,
						std::chrono::milliseconds(event.button.timestamp),
						event.button.which,
						{event.button.x, event.button.y},
						static_cast<mouse_button>(event.button.button),
						static_cast<keystate>(event.button.state),
						event.button.clicks
					};
				case SDL_MOUSEBUTTONUP:
					return mouse_up
					{
						event.button.windowID,
						std::chrono::milliseconds(event.button.timestamp),
						event.button.which,
						{event.button.x, event.button.y},
						static_cast<mouse_button>(event.button.button),
						static_cast<keystate>(event.button.state),
#if SDL_VERSION_ATLEAST(2,0,2)
						event.button.clicks
#endif
					};
				case SDL_MOUSEMOTION:
					return mouse_motion
					{
						event.motion.windowID,
						std::chrono::milliseconds(event.motion.timestamp),
						event.motion.which,
						{event.motion.x, event.motion.y},
						{event.motion.xrel, event.motion.yrel},
						static_cast<mouse_button_mask>(event.motion.state),
					};
				case SDL_MOUSEWHEEL:
					return mouse_wheel
					{
						event.wheel.windowID,
						std::chrono::milliseconds(event.wheel.timestamp),
						event.wheel.which,
						{event.wheel.x, event.wheel.y},
#if SDL_VERSION_ATLEAST(2,0,4)
						static_cast<wheel_direction>(event.wheel.direction),
#endif
					};
			}
		}
		return std::nullopt;
	}

#if SDL_VERSION_ATLEAST(2,0,4)
	int2 mouse_wheel::motion() const noexcept
	{
		return data.direction == wheel_direction::flipped ? -data.position : data.position;
	}
#endif

	bool relative_mouse_mode() noexcept
	{
		return SDL_GetRelativeMouseMode();
	}

	bool relative_mouse_mode(bool enable) noexcept
	{
		return !sdlcore::utils::check_error(SDL_SetRelativeMouseMode(SDL_bool(enable)));
	}

	void require_mouse_mode(bool enable) noexcept
	{
		sdlcore::utils::throw_error(SDL_SetRelativeMouseMode(SDL_bool(enable)));
	}

	std::optional<float2> window_normalized(uint32_t window_id, int2 position) noexcept
	{
		float2 result = static_cast<float2>(position);

		auto window = SDL_GetWindowFromID(window_id);
		if(!window)
			return std::nullopt;

		int2 window_size;
		SDL_GetWindowSize(window, &window_size.x(), &window_size.y());
		if( std::any_of(window_size.begin(), window_size.end(), [](auto c){ return c == 0;}) )
			return std::nullopt;

		result /= static_cast<float2>(window_size);
		return result;
	}

	std::optional<float2> window_normalized_position(const common_mouse_data& data) noexcept
	{
		return window_normalized(data.window_id, data.position);
	}

	std::optional<float2> window_normalized_motion(const mouse_motion_data& data) noexcept
	{
		return window_normalized(data.window_id, data.motion);
	}

	std::optional<float2> screen_normalized(uint32_t window_id, int2 position, bool absolute = true) noexcept
	{
		float2 result = static_cast<float2>(position);

		auto window = SDL_GetWindowFromID(window_id);
		if(!window)
			return std::nullopt;

		if(absolute)
		{
			int2 window_position;
			SDL_GetWindowPosition(window, &window_position.x(), &window_position.y());
			result += static_cast<float2>(window_position);
		}

		SDL_DisplayMode mode;
		if(SDL_GetCurrentDisplayMode(SDL_GetWindowDisplayIndex(window), &mode) < 0)
			return std::nullopt;

		const int2 screen_size{mode.w, mode.h};

		result /= static_cast<float2>(screen_size);
		return result;
	}

	std::optional<float2> screen_normalized_position(const common_mouse_data& data) noexcept
	{
		return screen_normalized(data.window_id, data.position);
	}

	std::optional<float2> screen_normalized_motion(const mouse_motion_data& data) noexcept
	{
		return screen_normalized(data.window_id, data.motion, false);
	}

#if SDL_VERSION_ATLEAST(2,0,4)

	// better to use expected<bool, error>
	bool mouse_capture(bool enable) noexcept
	{
		SDL_PumpEvents();
		return !sdlcore::utils::check_error(SDL_CaptureMouse(SDL_bool(enable)));
	}

	void require_mouse_capture(bool enable)
	{
		SDL_PumpEvents();
		sdlcore::utils::throw_error(SDL_CaptureMouse(SDL_bool(enable)));
	}

#endif

} // namespace simple::interactive
