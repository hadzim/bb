#ifndef OBSERVALBE_EVENT
#define OBSERVALBE_EVENT

#include "Poco/DefaultStrategy.h"
#include "Poco/BasicEvent.h"
#include "PocoEventChaining.h"
#include "Poco/AutoPtr.h"
#include "Poco/SharedPtr.h"

namespace TBS {

	template<class TArgs, class TDelegate>
	class ObservableStrategy: public Poco::NotificationStrategy<TArgs, TDelegate>
	/// Default notification strategy.
	///
	/// Internally, a std::vector<> is used to store
	/// delegate objects. Delegates are invoked in the
	/// order in which they have been registered.
	{
		public:
			typedef Poco::SharedPtr<TDelegate> DelegatePtr;
			typedef std::vector<DelegatePtr> Delegates;
			typedef typename Delegates::iterator Iterator;

		public:

			Poco::BasicEvent<int> DelegatesChanged;

		public:
			ObservableStrategy() {
			}

			ObservableStrategy(const ObservableStrategy& s) :
					defaultStrategy(s.defaultStrategy) {
			}

			~ObservableStrategy() {
			}

			void notify(const void* sender, TArgs& arguments) {
				defaultStrategy.notify(sender, arguments);
			}

			void add(const TDelegate& delegate) {
				defaultStrategy.add(delegate);
				delegatesUpdated();
			}

			void remove(const TDelegate& delegate) {
				defaultStrategy.remove(delegate);
				delegatesUpdated();
			}

			ObservableStrategy& operator =(const ObservableStrategy& s) {
				if (this != &s) {
					defaultStrategy = s.defaultStrategy;
				}
				return *this;
			}

			void clear() {
				defaultStrategy.clear();
				delegatesUpdated();
			}

			bool empty() const {
				return defaultStrategy.empty();
			}

		private:
			Poco::DefaultStrategy<TArgs, TDelegate> defaultStrategy;

			void delegatesUpdated() {
				int cnt = defaultStrategy.empty() ? 0 : 1;
				DelegatesChanged.notify(this, cnt);
			}
	};

	template<class TArgs, class TMutex = Poco::FastMutex>
	class ObservableEvent: public Poco::AbstractEvent<TArgs, ObservableStrategy<TArgs, Poco::AbstractDelegate<TArgs> >, Poco::AbstractDelegate<TArgs>, TMutex>
	/// A BasicEvent uses the DefaultStrategy which
	/// invokes delegates in the order they have been registered.
	///
	/// Please see the AbstractEvent class template documentation
	/// for more information.
	{
		public:
			ObservableEvent() {
				eventFwd = forwardFromTo(this->_strategy.DelegatesChanged, this->DelegatesChanged);
			}

			~ObservableEvent() {
				eventFwd = NULL;
			}

			Poco::BasicEvent <int> DelegatesChanged;

		private:
			ObservableEvent(const ObservableEvent& e);
			ObservableEvent& operator =(const ObservableEvent& e);
		private:
			IEventForwarder::AutoPtr eventFwd;
	};

} // namespace TBS

#endif // Foundation_DefaultStrategy_INCLUDED
