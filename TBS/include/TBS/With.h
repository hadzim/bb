#ifndef INCLUDED_TBS_WITH
#define INCLUDED_TBS_WITH

#include <memory>
#include <type_traits>


/*
	Macro to allow this:
	

	Poco::FastMutex mutex;

	TBS_WITH(lock, new Poco::FastMutex::ScopedLock(mutex)) {
		.... critical section ....
	}


	If the scoped object is called, remembert it is stored in a pointer, call it with '->'

*/

#define TBS_WITH(name, creation) \
	for (std::unique_ptr<std::remove_pointer<decltype(creation)>::type> name(creation);  name; name.reset())

/*
	How it works:
		1. object is initialized
		2. iteration condition is met
		3. body is executed
		4. iteration step deletes object
		5. iteration condition is no longer met
*/


#endif // included
