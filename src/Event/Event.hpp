#pragma once

#include "EventCallback.hpp"
#include <vector>
#include <algorithm>

namespace Labyrinth {

    class Event {

        std::vector<IEventCallback*> mCallbacks;

    public:

        Event()  {}
        ~Event() {}

		void AddListener(IEventCallback* Callback);
		void RemoveListener(const IEventCallback* Callback);
		void Trigger();
    };

    void Event::AddListener(IEventCallback* Callback) {
		std::vector<IEventCallback*>::iterator Index = std::find(mCallbacks.begin(), mCallbacks.end(), Callback);
		if (Index != mCallbacks.end()) {
			return;
		}
		else {
			mCallbacks.push_back(Callback);
		}
	}

	void Event::RemoveListener(const IEventCallback* Callback) {
		std::vector<IEventCallback*>::iterator Index = std::find(mCallbacks.begin(), mCallbacks.end(), Callback);
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

