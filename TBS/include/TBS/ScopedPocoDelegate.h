#ifndef INCLUDED_TBS_SCOPED_POCO_DELEGATE
#define INCLUDED_TBS_SCOPED_POCO_DELEGATE

#include <memory>

namespace TBS {

	class ScopedPocoDelegate {
	public:
		virtual ~ScopedPocoDelegate() {};
		typedef std::unique_ptr<ScopedPocoDelegate> Holder;

		template <class Event, class Delegate>
		static Holder make(Event& event, Delegate delegate);

	private:
		template <class Event, class Delegate>
		class Impl;
	};

	template <class Event, class Delegate>
	class ScopedPocoDelegate::Impl: public ScopedPocoDelegate {
	public:
		Impl(Event& event, Delegate delegate):
			event(event), delegate(delegate)
		{
			event += delegate;
		}

		~Impl() {
			event -= delegate;
		}
		
	private:
		Event& event;
		Delegate delegate;
	};

	template <class Event, class Delegate>
	ScopedPocoDelegate::Holder ScopedPocoDelegate::make(Event& event, Delegate delegate) {
		return Holder(new Impl<Event, Delegate>(event, delegate));
	}

} // namespace

#endif // included
