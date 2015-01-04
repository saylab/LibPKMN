/**
 * @file    Column.h
 * @ingroup SQLiteCpp
 * @brief   Encapsulation of a Column in a row of the result pointed by the prepared SQLite::Statement.
 *
 * Copyright (c) 2012-2013 Sebastien Rombauts (sebastien.rombauts@gmail.com)
 *               2013-2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#pragma once

#include <cstdint>

#include "sqlite3.h"

#include "Exception.h"
#include "Statement.h"

#include <pkmn/types/pkstring.hpp>

namespace SQLite
{


/**
 * @brief Encapsulation of a Column in a row of the result pointed by the prepared Statement.
 *
 *  A Column is a particular field of SQLite data in the current row of result
 * of the Statement : it points to a single cell.
 *
 * Its value can be expressed as a text, and, when applicable, as a numeric
 * (integer or floating point) or a binary blob.
 *
 * Thread-safety: a Column object shall not be shared by multiple threads, because :
 * 1) in the SQLite "Thread Safe" mode, "SQLite can be safely used by multiple threads
 *    provided that no single database connection is used simultaneously in two or more threads."
 * 2) the SQLite "Serialized" mode is not supported by SQLiteC++,
 *    because of the way it shares the underling SQLite precompiled statement
 *    in a custom shared pointer (See the inner class "Statement::Ptr").
 */
class Column
{
public:
    /**
     * @brief Encapsulation of a Column in a Row of the result.
     *
     * @param[in] aStmtPtr  Shared pointer to the prepared SQLite Statement Object.
     * @param[in] aIndex    Index of the column in the row of result
     */
    Column(Statement::Ptr& aStmtPtr, int aIndex)    throw(); // nothrow
    /// @brief Simple destructor
    virtual ~Column(void)                           throw(); // nothrow
    
    // default copy constructor and assignment operator are perfectly suited :
    // they copy the Statement::Ptr which in turn increments the reference counter.

#ifdef SQLITE_ENABLE_COLUMN_METADATA
    /**
     * @brief Return a pointer to the column name
     * 
     *  Require definition of the SQLITE_ENABLE_COLUMN_METADATA preprocessor macro :
     * - for compilation of the SQLite library,
     * - and also when compiling this wrapper.
     */
	const char*     getName  (void) const throw(); // nothrow
#endif

    /// @brief Return the integer value of the column.
    int             getInt   (void) const throw(); // nothrow
    /// @brief Return the 64bits integer value of the column.
    sqlite3_int64   getInt64 (void) const throw(); // nothrow
    /// @brief Return the double (64bits float) value of the column.
    double          getDouble(void) const throw(); // nothrow
    /**
     * @brief Return a pointer to the text value (NULL terminated string) of the column.
     *
     * @warning The value pointed at is only valid while the statement is valid (ie. not finalized),
     *          thus you must copy it before using it beyond its scope (to a std::string for instance).
     */
    const char*     getText  (void) const throw(); // nothrow
    /**
     * @brief Return a pointer to the 16-bit text value (NULL terminated string) of the column.
     *
     * @warning The value pointed at is only valid while the statement is valid (ie. not finalized),
     *          thus you must copy it before using it beyond its scope (to a std::string for instance).
     */
    const uint16_t* getText16(void) const throw(); // nothrow
    /**
     * @brief Return a pkmn::pkstring of the column.
     *
     * @warning The value pointed at is only valid while the statement is valid (ie. not finalized),
     *          thus you must copy it before using it beyond its scope (to a std::string for instance).
     */
    inline pkmn::pkstring getPKString(void) const throw() // nothrow
    {
        return pkmn::pkstring(getText16());
    }

    /**
     * @brief Return a pointer to the binary blob value of the column.
     *
     * @warning The value pointed at is only valid while the statement is valid (ie. not finalized),
     *          thus you must copy it before using it beyond its scope (to a std::string for instance).
     */
    const void*     getBlob  (void) const throw(); // nothrow
    
    /**
     * @brief Return the type of the value of the column
     *
     * Return either SQLITE_INTEGER, SQLITE_FLOAT, SQLITE_TEXT, SQLITE_BLOB, or SQLITE_NULL.
     *
     * @warning After a type conversion (by a call to a getXxx on a Column of a Yyy type),
     *          the value returned by sqlite3_column_type() is undefined.
     */
    int getType(void) const throw(); // nothrow
    
    /// @brief Test if the column is an integer type value (meaningful only before any conversion)
    inline bool isInteger(void) const throw() // nothrow
    {
        return (SQLITE_INTEGER == getType());
    }
    /// @brief Test if the column is a floating point type value (meaningful only before any conversion)
    inline bool isFloat(void) const throw() // nothrow
    {
        return (SQLITE_FLOAT == getType());
    }
    /// @brief Test if the column is a text type value (meaningful only before any conversion)
    inline bool isText(void) const throw() // nothrow
    {
        return (SQLITE_TEXT == getType());
    }
    /// @brief Test if the column is a binary blob type value (meaningful only before any conversion)
    inline bool isBlob(void) const throw() // nothrow
    {
        return (SQLITE_BLOB == getType());
    }
    /// @brief Test if the column is NULL (meaningful only before any conversion)
    inline bool isNull(void) const throw() // nothrow
    {
        return (SQLITE_NULL == getType());
    }

    /**
     * @brief Return the number of bytes used by the text (or blob) value of the column
     *
     * Return either :
     * - size in bytes (not in characters) of the string returned by getText() without the '\0' terminator
     * - size in bytes of the string representation of the numerical value (integer or double)
     * - size in bytes of the binary blob returned by getBlob()
     * - 0 for a NULL value
     */
    int getBytes(void) const throw();

    /// @brief Alias returning the number of bytes used by the text (or blob) value of the column
    inline int size(void) const throw()
    {
        return getBytes ();
    }

    #define INT_OPERATOR(type) inline operator type() const \
                               { \
                                   return type(getInt()); \
                               }

    INT_OPERATOR(uint8_t)
    INT_OPERATOR(uint16_t)
    INT_OPERATOR(uint32_t)
    INT_OPERATOR(uint64_t)
    INT_OPERATOR(int8_t)
    INT_OPERATOR(int16_t)
    INT_OPERATOR(int32_t)
    INT_OPERATOR(int64_t)

    /// @brief Inline cast operator to float
    inline operator float() const
    {
        return float(getDouble());
    }
    /// @brief Inline cast operator to double
    inline operator double() const
    {
        return getDouble();
    }
    /**
     * @brief Inline cast operator to char*
     *
     * @see getText 
     */
    inline operator const char*() const
    {
        return getText();
    }
    /**
     * @brief Inline cast operator to uint16_t*
     *
     * @see getText 
     */
    inline operator const uint16_t*() const
    {
        return getText16();
    }
    /**
     * @brief Inline cast operator to pkmn::pkstring
     *
     * @see getText
     */
    inline operator pkmn::pkstring() const
    {
        return getPKString();
    }
    /**
     * @brief Inline cast operator to void*
     *
     * @see getBlob
     */
    inline operator const void*() const
    {
        return getBlob();
    }
#ifdef __GNUC__
    // NOTE : the following is required by GCC to cast a Column result in a std::string
    // (error: conversion from ‘SQLite::Column’ to non-scalar type ‘std::string {aka std::basic_string<char>}’ requested)
    // but is not working under Microsoft Visual Studio 2010 and 2012
    // (error C2440: 'initializing' : cannot convert from 'SQLite::Column' to 'std::basic_string<_Elem,_Traits,_Ax>'
    //  [...] constructor overload resolution was ambiguous)
    /// Inline cast operator to std::string
    inline operator const std::string() const
    {
        return getText();
    }
#endif

    /// @brief Return UTF-8 encoded English language explanation of the most recent error.
    inline const char* errmsg(void) const
    {
        return sqlite3_errmsg(mStmtPtr);
    }
private:
    Statement::Ptr  mStmtPtr;   //!< Shared Pointer to the prepared SQLite Statement Object
    int             mIndex;     //!< Index of the column in the row of result
};

/**
 * @brief Standard std::ostream text inserter
 *
 * Insert the text value of the Column object, using getText(), into the provided stream.
 *
 * @param[in] aStream   Stream to use
 * @param[in] aColumn   Column object to insert into the provided stream
 *
 * @return  Reference to the stream used
 */
std::ostream& operator<<(std::ostream& aStream, const Column& aColumn);


}  // namespace SQLite
