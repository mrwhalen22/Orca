#pragma once

#include "Hazel/Core.h"


//Need to be moved to precompiled headers
#include <string>
#include <functional>

namespace Hazel {

	//All event types
	enum class EventType {
		None = 0,
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
		AppTick, AppUpdate, AppRender, 
		KeyPressed, KeyReleased, 
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
	};

	//All event categories
	enum EventCategory {
		None = 0,
		EventCategoryApplication  = BIT(0),
		EventCategoryInput        = BIT(1),
		EventCategoryKeyboard     = BIT(2),
		EventCategoryMouse        = BIT(3),
		EventCategoryMouseButton  = BIT(4)
	};

// 2 defines to implement all the basic virtual functions in each event type
#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::##type; }\
								virtual EventType GetEventType() const override { return GetStaticType(); }\
								virtual const char* GetName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override { return category; }



	class HAZEL_API Event {
		friend class EventDispatcher;
	public:
		virtual EventType GetEventType() const = 0;
		virtual const char* GetName() const = 0;
		virtual int GetCategoryFlags() const = 0;
		virtual std::string ToString() const { return GetName(); }

		inline bool IsInCategory(EventCategory category) {
			return GetCategoryFlags() & category;
		}

	protected:
		bool m_Handled = false;

	};

	class EventDispatcher {
		template<typename T>
		using EventFn = std::function<bool(T&)>;

	public:
		EventDispatcher(Event& event)
			: m_Event(event) {}

		template<typename T>
		bool Dispatch(EventFn<T> func) {
			//runs the associated event function if the event matches the type of the event passed into the template
			if (m_Event.GetEventType() == T::GetStaticType()) {
				m_Event.m_Handled = func(*(T*)&m_Event); // this somehow executes the func pointer no clue how yet
				return true;
			}
			return false;
		}


	private:
		Event& m_Event;

	};

	// basic operator override for all event classes
	inline std::ostream& operator<<(std::ostream& os, const Event& e) {
		return os << e.ToString();
	}

}
