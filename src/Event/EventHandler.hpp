#include "Event.hpp"
#include <string>
#include <unordered_map>

namespace Labyrinth {

    class EventHandler {

	protected:

        std::unordered_map<std::string, std::shared_ptr<Event>> mEvents;

    public:

		EventHandler();
		virtual ~EventHandler();

		void RegisterCallback(const std::string& Name, const std::shared_ptr<IEventCallback>& Callback);
		void RegisterEvent(const std::string& Name, const std::shared_ptr<Event>& EventPtr);
		void TriggerEvent(const std::string& Name);
		void DeregisterCallback(const std::shared_ptr<IEventCallback>& Callback);
    };


    EventHandler::EventHandler()
	{}

	EventHandler::~EventHandler() {}

	void EventHandler::RegisterCallback(const std::string & Name, const std::shared_ptr<IEventCallback>& Callback) {
		if (mEvents.count(Name) < 1) {
			return;
		}
		else {
			Callback->mEventName = Name;
			mEvents[Name]->AddListener(Callback);
		}
	}

	void EventHandler::RegisterEvent(const std::string & Name, const std::shared_ptr<Event>& EventPtr) {
		if (mEvents.count(Name) < 1) {
			mEvents[Name] = EventPtr;
		}
	}

	void EventHandler::TriggerEvent(const std::string & Name) {
		if (mEvents.count(Name) > 0) {
			mEvents[Name]->Trigger();
		}
	}

	void EventHandler::DeregisterCallback(const std::shared_ptr<IEventCallback>& Callback) {
		if (mEvents.count(Callback->mEventName) > 0) {
			mEvents[Callback->mEventName]->RemoveListener(Callback);
		}
	}
}