#ifndef MAGMATIC_EVENTRELAY_HPP
#define MAGMATIC_EVENTRELAY_HPP

#include <functional>
#include <map>
#include <string>
#include <memory>

namespace magmatic::ecs
{
	class EventRelay
	{
	private:
		struct BaseEventDispatcher
		{};

		template<typename T>
		struct EventDispatcher : public BaseEventDispatcher
		{
			using Receiver = std::function<void(const T&)>;
			using ReceiverID = u_int64_t;
			void dispatch(const T& event_payload) const;

			ReceiverID subscribe(const Receiver& receiver);
			void unsubscribe(ReceiverID receiver_id);

			std::size_t subscribers_count() const noexcept ;

		private:
			ReceiverID last_id = 0;
			std::map<ReceiverID, Receiver> subscribers;
		};

	public:
		template<typename T>
		EventDispatcher<T>::ReceiverID subscribe(const typename EventDispatcher<T>::Receiver& receiver);

		template<typename T>
		void unsubscribe(typename EventDispatcher<T>::ReceiverID receiver_id);

		template<typename T>
		std::size_t subscribers_count() const;

		template<typename T, typename... Args>
		requires std::constructible_from<T, Args...>
		void emit(Args&&... args);

		template<typename T>
		void emit(const T& event_payload);

		template<typename T>
		void emit(T& event_payload);

	private:
		std::map<std::string, std::shared_ptr<BaseEventDispatcher>> events;

		template<typename T>
		static constexpr std::string getEventName();
	};

	template<typename T>
	constexpr std::string EventRelay::getEventName()
	{
		return typeid(T).name();
	}

	template<typename T, typename... Args>
	requires std::constructible_from<T, Args...>
	void EventRelay::emit(Args &&... args)
	{
		const T event(std::forward<Args>(args)...);
		emit(event);
	}

	template<typename T>
	void EventRelay::emit(const T& event_payload)
	{
		const auto event_name = getEventName<T>();
		if(!events.contains(event_name))
		{
			return;
		}

		const auto dispatcher = std::static_pointer_cast<EventDispatcher<T>>(events[event_name]);
		dispatcher->dispatch(event_payload);
	}

	template<typename T>
	EventRelay::EventDispatcher<T>::ReceiverID EventRelay::subscribe(const EventRelay::EventDispatcher<T>::Receiver& receiver)
	{
		const auto event_name = getEventName<T>();

		std::shared_ptr<EventDispatcher<T>> dispatcher;
		if(events.contains(event_name))
		{
			 dispatcher = std::static_pointer_cast<EventDispatcher<T>>(events[getEventName<T>()]);
		}
		else
		{
			dispatcher = std::make_shared<EventDispatcher<T>>();
			events[event_name] = dispatcher;
		}

		return dispatcher->subscribe(receiver);
	}

	template<typename T>
	void EventRelay::unsubscribe(EventRelay::EventDispatcher<T>::ReceiverID receiver_id)
	{
		const auto event_name = getEventName<T>();
		assert(events.contains(event_name));

		auto dispatcher = std::static_pointer_cast<EventDispatcher<T>>(events[event_name]);
		dispatcher->unsubscribe(receiver_id);
	}

	template<typename T>
	void EventRelay::emit(T &event_payload)
	{
		emit(static_cast<const T&>(event_payload));
	}

	template<typename T>
	std::size_t EventRelay::subscribers_count() const
	{
		const auto event_name = getEventName<T>();
		if(!events.contains(event_name))
		{
			return 0;
		}
		else
		{
			const auto dispatcher = std::static_pointer_cast<EventDispatcher<T>>(events.at(event_name));
			return dispatcher->subscribers_count();
		}
	}

	template<typename T>
	EventRelay::EventDispatcher<T>::ReceiverID
	EventRelay::EventDispatcher<T>::subscribe(const EventRelay::EventDispatcher<T>::Receiver &receiver)
	{
		const auto current_id = last_id;

		subscribers[current_id] = receiver;
		++last_id;
		return current_id;
	}

	template<typename T>
	void EventRelay::EventDispatcher<T>::unsubscribe(EventRelay::EventDispatcher<T>::ReceiverID receiver_id)
	{
		assert(subscribers.contains(receiver_id));
		subscribers.erase(receiver_id);
	}

	template<typename T>
	void EventRelay::EventDispatcher<T>::dispatch(const T &event_payload) const
	{
		for(const auto& entry : subscribers)
		{
			const auto& handler = entry.second;
			handler(event_payload);
		}
	}

	template<typename T>
	std::size_t EventRelay::EventDispatcher<T>::subscribers_count() const noexcept
	{
		return subscribers.size();
	}
}

#endif //MAGMATIC_EVENTRELAY_HPP
