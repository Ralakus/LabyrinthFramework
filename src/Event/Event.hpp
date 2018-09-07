#pragma once

#include "EventCallback.hpp"
#include <vector>
#include <algorithm>

namespace Labyrinth {

    class Event {

        std::vector<std::shared_ptr<IEventCallback>> mCallbacks;

    public:

        Event()  {}
        ~Event() {}

		void AddListener(const std::shared_ptr<IEventCallback>& Callback);
		void RemoveListener(const std::shared_ptr<IEventCallback>& Callback);
		void Trigger();
    };

    void Event::AddListener(const std::shared_ptr<IEventCallback>& Callback) {
		std::vector<std::shared_ptr<IEventCallback>>::iterator Index = std::find(mCallbacks.begin(), mCallbacks.end(), Callback);
		if (Index != mCallbacks.end()) {
			return;
		}
		else {
			mCallbacks.push_back(Callback);
		}
	}

	void Event::RemoveListener(const std::shared_ptr<IEventCallback>& Callback) {
		std::vector<std::shared_ptr<IEventCallback>>::iterator Index = std::find(mCallbacks.begin(), mCallbacks.end(), Callback);
		if (Index == mCallbacks.end()) {
			return;
		}
		else {
			mCallbacks.erase(Index);
		}
	}

	void Event::Trigger() {
		if (mCallbacks.size() > 0) {
			for (auto Function : mCallbacks) {
				(*Function)();
			}
		}
	}

}

