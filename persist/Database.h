#ifndef ECO_DATABASE_H
#define ECO_DATABASE_H
/*******************************************************************************
@ name


@ function


--------------------------------------------------------------------------------
@ history ver 1.0 @
@ records: ujoy modifyed on 2013-01-01.
1.create and init this class.


--------------------------------------------------------------------------------
* copyright(c) 2013 - 2015, ujoy, reserved all right.

*******************************************************************************/
#include <eco/ExportApi.h>
#include <eco/meta/Meta.h>
#include <eco/persist/Recordset.h>
#include <eco/persist/JoinMapping.h>
#include <eco/persist/DatabaseConfig.h>


////////////////////////////////////////////////////////////////////////////////
namespace eco{;


////////////////////////////////////////////////////////////////////////////////
/* auto transaction management.
1.multi thread will execute the transaction sync.
2.transaction level is thread safe, it is a thread local data.
3.this thread manage a level transaction.
*/
template<typename DatabaseT>
class Transaction
{
public:
	inline Transaction(DatabaseT& db)
		: m_db(db), m_commited(false)
	{
		++m_db.transaction_level();
		if (m_db.transaction_level() == 1)
		{
			m_db.begin();
		}
	}

	// auto rollback when hasn't call commit explicit.
	inline ~Transaction()
	{
		if (!m_commited)
		{
			m_db.rollback();
			m_db.transaction_level() = 0;
		}
	}

	// commit when database operation finished.
	inline void commit()
	{
		if (m_db.transaction_level() == 0)
		{
			throw std::logic_error("commit error: db has no transaction.");
		}

		if (m_db.transaction_level() == 1)
		{
			m_db.commit();
		}
		--m_db.transaction_level();
		m_commited = true;
	}

	inline DatabaseT& get_db()
	{
		return m_db;
	}

private:
	DatabaseT& m_db;
	uint32_t m_commited;
};


////////////////////////////////////////////////////////////////////////////////
class ECO_API Database
{
	ECO_OBJECT(Database);
public:
	// datasouce client character set.
	enum CharSet
	{
		char_set_gbk = 1,
		char_set_gb2312,
		char_set_utf8,
		char_set_utf16,
		char_set_utf32,
	};

public:
	inline Database() {}

	// connect to server and login database.
	virtual void open(
		IN const char* server_ip,
		IN const int port,
		IN const char* db_name,
		IN const char* user_id,
		IN const char* password,
		IN const CharSet char_set = char_set_gbk) {};

	// connect to sqlite database.
	virtual void open(
		IN const char* db_name,
		IN const CharSet char_set = char_set_gbk) {};

	// disconnect to server
	virtual void close() = 0;

	// get the state of database connection.
	virtual bool is_open() = 0;

	// read the object from database.
	virtual void select(
		OUT Record& obj,
		IN  const char* sql) = 0;

	// read data sheet from database.
	virtual void select(
		OUT Recordset& obj_set,
		IN  const char* sql) = 0;

	// execute sql, return affected rows.
	virtual uint64_t execute_sql(
		IN  const char* sql) = 0;

	// ping mysql server: if success return true, else false.
	virtual bool ping() = 0;

	// get mysql server system time.
	virtual uint64_t get_system_time() = 0;

	// is exist table in this datasource.
	virtual bool has_table(
		IN const char* table_name) = 0;

	// get exist tables in this datasource.
	virtual void get_tables(
		OUT Recordset& tables,
		IN  const char* db_name = "") = 0;

	// get mysql server config.
	virtual DatabaseConfig& config() = 0;

////////////////////////////////////////////////////////////////////////////////
public:
	typedef Transaction<eco::Database> Transaction;

	// begin a transaction.
	virtual void begin() = 0;

	// commit a transaction.
	virtual void commit() = 0;

	// rollback a transaction.
	virtual void rollback() = 0;

	// transaction level, thread local data.
	virtual uint32_t& transaction_level() = 0;

////////////////////////////////////////////////////////////////////////////////
public:
	// create talbe.
	inline void create_table(
		IN const ObjectMapping& map,
		IN bool check_table_exist = true)
	{
		if (!check_table_exist || !has_table(map.get_table()))
		{
			std::string sql;
			map.get_create_table_sql(sql, &config());
			execute_sql(sql.c_str());
		}
	}

	// save data to database.
	template<typename meta_t, typename object_set_t>
	inline void save_all(
		IN const object_set_t& obj_set,
		IN const ObjectMapping& mapping)
	{
		object_set_t::const_iterator it = obj_set.begin();
		for (; it != obj_set.end(); ++it)
		{
			save<meta_t>(*it, mapping);
		}
	}

	// save data to database.
	template<typename meta_t, typename object_t>
	inline void save(
		IN const object_t& obj,
		IN const ObjectMapping& mapping)
	{
		meta_t meta;
		meta.attach(obj);

		std::string sql;
		if (meta.timestamp().is_original())		// origin object.
		{
			return ;	// no need to save.
		}
		else if (meta.timestamp().is_inserted())	// insert object.
		{
			mapping.get_insert_sql<meta_t>(sql, obj);
		}
		else if (meta.timestamp().is_updated())	// update object.
		{
			mapping.get_update_sql<meta_t>(sql, obj);
		}
		else if (meta.timestamp().is_removed())	// delete object.
		{
			mapping.get_delete_sql<meta_t>(sql, obj);
		}
		else
		{
			throw std::logic_error("save object failed, timestamp is invalid.");
		}

		// execute sql to save data.
		uint64_t rows = execute_sql(sql.c_str());

		// that update and insert object need to reset timestamp to original.
		meta.timestamp().origin();
	}

	// removed all data from table.
	inline uint64_t remove(
		IN const char* table,
		IN const std::string& cond_sql = std::string())
	{
		std::string sql("delete from ");
		sql += table;
		if (!cond_sql.empty())
		{
			sql += ' ';
			sql += cond_sql;
		}
		uint64_t rows = execute_sql(sql.c_str());
		return rows;
	}

	// read data from database.
	template<typename meta_t, typename object_t>
	inline void read(
		OUT object_t& obj,
		IN  const ObjectMapping& mapping)
	{
		std::string sql;
		mapping.get_select_sql<meta_t>(sql, obj);

		Record rd;
		select(rd, sql.c_str());
		mapping.decode<meta_t>(obj, rd, "eco_db");
	}

	// read data from database.
	template<typename meta_t, typename object_t>
	inline void read(
		OUT object_t& obj,
		IN  const ObjectMapping& mapping,
		IN  const char* cond_sql,
		IN  const char lias_table = 0)
	{
		std::string sql;
		mapping.get_select_sql(sql, cond_sql, lias_table);

		Record rd;
		select(rd, sql.c_str());
		mapping.decode<meta_t>(obj, rd, "eco_db");
	}

	// save data to database.
	template<typename meta_t, typename object_set_t>
	inline void select(
		OUT object_set_t& obj_set,
		IN  const ObjectMapping& mapping,
		IN  const char* cond_sql = "",
		IN  const char lias_table = 0)
	{
		std::string sql;
		mapping.get_select_sql(sql, cond_sql, lias_table);
		Recordset rd_set;
		select(rd_set, sql.c_str());
		mapping.decode_some<meta_t>(obj_set, rd_set, "eco_db");
	}

	// save data to database.
	template<typename object_set_t, typename object_t>
	inline void select_join(
		OUT object_set_t& obj_set,
		IN  const JoinMapping<object_t>& join_map,
		IN  const char* cond_sql = "")
	{	
		std::string sql;
		join_map.get_select_join_sql(sql, cond_sql);
		Recordset rd_set;
		select(rd_set, sql.c_str());
		
		// decode meta.
		join_map.decode_join(obj_set, rd_set, "eco_db");
	}

	// is exist record in table.
	inline uint32_t has_record(
		IN const char* table_name,
		IN const char* cond_sql = "")
	{
		// format sql.
		std::string sql("select count(*) from ");
		sql += table_name;
		if (!eco::empty(cond_sql))
		{
			sql += ' ';
			sql += cond_sql;
		}

		// query sql.
		Record rd;
		select(rd, sql.c_str());
		uint32_t rows = eco::cast<uint32_t>(rd[0]);
		return rows;
	}

	// auto generate table alias.
	inline char get_alias(IN const uint32_t i) const
	{
		return 'A' + i;
	}
};


////////////////////////////////////////////////////////////////////////////////
}// ns::eco
#endif