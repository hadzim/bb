#ifndef INCLUDED_TBS_DATABASE_TRANSACTION
#define INCLUDED_TBS_DATABASE_TRANSACTION

namespace TBS {

	template <class C>
	class DatabaseTransaction {
	public:
		DatabaseTransaction(C& database): 
			database(database),
			opened(false)
		{
			begin();
		}

		~DatabaseTransaction() { if (opened) rollback(); } 

		void begin() { database.begin(); opened = true; }
		void commit() { database.commit(); opened = false; }
		void rollback() { database.rollback(); opened = false; }
		
	private:
		C& database;
		bool opened;
	};

} // namespace

#endif // included
