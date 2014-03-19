#ifndef BTSharedPtr
#define BTSharedPtr
#include "TBS/Log.h"



namespace Poco {
	template <class C, class RC = ReferenceCounter, class RP = ReleasePolicy<C> >
	class BtSharedPtr
		/// SharedPtr is a "smart" pointer for classes implementing
		/// reference counting based garbage collection.
		/// SharedPtr is thus similar to AutoPtr. Unlike the
		/// AutoPtr template, which can only be used with
		/// classes that support reference counting, SharedPtr
		/// can be used with any class. For this to work, a
		/// SharedPtr manages a reference count for the object
		/// it manages.
		///
		/// SharedPtr works in the following way:
		/// If an SharedPtr is assigned an ordinary pointer to
		/// an object (via the constructor or the assignment operator),
		/// it takes ownership of the object and the object's reference
		/// count is initialized to one.
		/// If the SharedPtr is assigned another SharedPtr, the
		/// object's reference count is incremented by one.
		/// The destructor of SharedPtr decrements the object's
		/// reference count by one and deletes the object if the
		/// reference count reaches zero.
		/// SharedPtr supports dereferencing with both the ->
		/// and the * operator. An attempt to dereference a null
		/// SharedPtr results in a NullPointerException being thrown.
		/// SharedPtr also implements all relational operators and
		/// a cast operator in case dynamic casting of the encapsulated data types
		/// is required.
	{
	public:
		BtSharedPtr(): _pCounter(new RC), _ptr(0)
		{
		}

		BtSharedPtr(C* ptr): _pCounter(new RC), _ptr(ptr)
		{
			TBS::dumpBacktrace("construct", "Ptr", false);
		}

		template <class Other, class OtherRP>
		BtSharedPtr(const BtSharedPtr<Other, RC, OtherRP>& ptr): _pCounter(ptr._pCounter), _ptr(const_cast<Other*>(ptr.get()))
		{
			TBS::dumpBacktrace("copyconstruct1", "Ptr", false);
			_pCounter->duplicate();
		}

		BtSharedPtr(const BtSharedPtr& ptr): _pCounter(ptr._pCounter), _ptr(ptr._ptr)
		{
			TBS::dumpBacktrace("copyconstruct2", "Ptr", false);
			_pCounter->duplicate();
		}

		~BtSharedPtr()
		{
			TBS::dumpBacktrace("destruct", "Ptr", false);
			release();
		}

		BtSharedPtr& assign(C* ptr)
		{
			if (get() != ptr)
			{
				RC* pTmp = new RC;
				release();
				_pCounter = pTmp;
				_ptr = ptr;
			}
			return *this;
		}

		BtSharedPtr& assign(const BtSharedPtr& ptr)
		{
			if (&ptr != this)
			{
				SharedPtr tmp(ptr);
				swap(tmp);
			}
			return *this;
		}

		template <class Other, class OtherRP>
		BtSharedPtr& assign(const SharedPtr<Other, RC, OtherRP>& ptr)
		{
			if (ptr.get() != _ptr)
			{
				SharedPtr tmp(ptr);
				swap(tmp);
			}
			return *this;
		}

		BtSharedPtr& operator = (C* ptr)
		{
			TBS::dumpBacktrace("operator=1", "Ptr", false);
			return assign(ptr);
		}

		BtSharedPtr& operator = (const BtSharedPtr& ptr)
		{
			TBS::dumpBacktrace("operator=2", "Ptr", false);
			return assign(ptr);
		}

		template <class Other, class OtherRP>
		BtSharedPtr& operator = (const BtSharedPtr<Other, RC, OtherRP>& ptr)
		{
				TBS::dumpBacktrace("operator=3", "Ptr", false);
			return assign<Other>(ptr);
		}

		void swap(BtSharedPtr& ptr)
		{
			std::swap(_ptr, ptr._ptr);
			std::swap(_pCounter, ptr._pCounter);
		}

		template <class Other>
		BtSharedPtr<Other, RC, RP> cast() const
			/// Casts the SharedPtr via a dynamic cast to the given type.
			/// Returns an SharedPtr containing NULL if the cast fails.
			/// Example: (assume class Sub: public Super)
			///    SharedPtr<Super> super(new Sub());
			///    SharedPtr<Sub> sub = super.cast<Sub>();
			///    poco_assert (sub.get());
		{
			Other* pOther = dynamic_cast<Other*>(_ptr);
			if (pOther)
				return BtSharedPtr<Other, RC, RP>(_pCounter, pOther);
			return BtSharedPtr<Other, RC, RP>();
		}

		template <class Other>
		BtSharedPtr<Other, RC, RP> unsafeCast() const
			/// Casts the SharedPtr via a static cast to the given type.
			/// Example: (assume class Sub: public Super)
			///    SharedPtr<Super> super(new Sub());
			///    SharedPtr<Sub> sub = super.unsafeCast<Sub>();
			///    poco_assert (sub.get());
		{
			Other* pOther = static_cast<Other*>(_ptr);
			return BtSharedPtr<Other, RC, RP>(_pCounter, pOther);
		}

		C* operator -> ()
		{
			return deref();
		}

		const C* operator -> () const
		{
			return deref();
		}

		C& operator * ()
		{
			return *deref();
		}

		const C& operator * () const
		{
			return *deref();
		}

		C* get()
		{
			return _ptr;
		}

		const C* get() const
		{
			return _ptr;
		}

		operator C* ()
		{
			return _ptr;
		}

		operator const C* () const
		{
			return _ptr;
		}

		bool operator ! () const
		{
			return _ptr == 0;
		}

		bool isNull() const
		{
			return _ptr == 0;
		}

		bool operator == (const BtSharedPtr& ptr) const
		{
			return get() == ptr.get();
		}

		bool operator == (const C* ptr) const
		{
			return get() == ptr;
		}

		bool operator == (C* ptr) const
		{
			return get() == ptr;
		}

		bool operator != (const SharedPtr& ptr) const
		{
			return get() != ptr.get();
		}

		bool operator != (const C* ptr) const
		{
			return get() != ptr;
		}

		bool operator != (C* ptr) const
		{
			return get() != ptr;
		}

		bool operator < (const BtSharedPtr& ptr) const
		{
			return get() < ptr.get();
		}

		bool operator < (const C* ptr) const
		{
			return get() < ptr;
		}

		bool operator < (C* ptr) const
		{
			return get() < ptr;
		}

		bool operator <= (const BtSharedPtr& ptr) const
		{
			return get() <= ptr.get();
		}

		bool operator <= (const C* ptr) const
		{
			return get() <= ptr;
		}

		bool operator <= (C* ptr) const
		{
			return get() <= ptr;
		}

		bool operator > (const BtSharedPtr& ptr) const
		{
			return get() > ptr.get();
		}

		bool operator > (const C* ptr) const
		{
			return get() > ptr;
		}

		bool operator > (C* ptr) const
		{
			return get() > ptr;
		}

		bool operator >= (const BtSharedPtr& ptr) const
		{
			return get() >= ptr.get();
		}

		bool operator >= (const C* ptr) const
		{
			return get() >= ptr;
		}

		bool operator >= (C* ptr) const
		{
			return get() >= ptr;
		}

		int referenceCount() const
		{
			return _pCounter->referenceCount();
		}

	private:
		C* deref() const
		{
			if (!_ptr)
				throw NullPointerException();

			return _ptr;
		}

		void release()
		{
			TBS::dumpBacktrace("release", "Ptr", false);
			poco_assert_dbg (_pCounter);
			int i = _pCounter->release();
			if (i == 0)
			{
				RP::release(_ptr);
				_ptr = 0;

				delete _pCounter;
				_pCounter = 0;
			}
		}

		BtSharedPtr(RC* pCounter, C* ptr): _pCounter(pCounter), _ptr(ptr)
			/// for cast operation
		{
			poco_assert_dbg (_pCounter);
			_pCounter->duplicate();
		}

	private:
		RC* _pCounter;
		C*  _ptr;

		template <class OtherC, class OtherRC, class OtherRP> friend class BtSharedPtr;
	};


	template <class C, class RC, class RP>
	inline void swap(BtSharedPtr<C, RC, RP>& p1, BtSharedPtr<C, RC, RP>& p2)
	{
		p1.swap(p2);
	}
}



#endif
