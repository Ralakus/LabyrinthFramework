#include <Utilities/Platform.hpp>

#include <memory>
#include <functional>
#include <string>

namespace Labyrinth {
	
    class IEventCallback {

        friend class EventHandler;

		protected:

		    std::string mEventName;

    public:

        NON_COPYABLE(IEventCallback)

        IEventCallback();
        virtual ~IEventCallback();

        virtual void operator()() = 0;
        virtual bool IsValid() = 0;

        const std::string& GetRegisteredEvent() const;
    };

    class EventCallback : public IEventCallback {
		
        std::function<void()> mFunction;

    public:

        template<class T>
        EventCallback(const T& Function);
        EventCallback(const std::function<void()>& Function);
        ~EventCallback();
        
        virtual void operator()() override;
        
        virtual bool IsValid() override;
    };




    IEventCallback::IEventCallback()
	{}

	IEventCallback::~IEventCallback()
	{}

	const std::string& IEventCallback::GetRegisteredEvent() const
	{ return mEventName; }

    template<class T>
    EventCallback::EventCallback(const T & Function)
    : IEventCallback(), mFunction(Function) {}
    
    EventCallback::EventCallback(const std::function<void()>& Function)
    : IEventCallback(), mFunction(Function) {}
    
    EventCallback::~EventCallback()
    {}
    
    void EventCallback::operator()()
    { mFunction(); }
    
    bool EventCallback::IsValid()
    { return (mFunction != nullptr); }

}