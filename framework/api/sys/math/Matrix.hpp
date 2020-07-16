#pragma once

#include <stdexcept>
#include "api/sys/helpers/functions/print.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "MATH"



namespace {

   enum class eIndexType : uint8_t { ROW, COLUMN, ELEMENT };

}

#if __cplusplus > 201703L // C++2a


namespace {

   template< eIndexType tType >
   class TIndex
   {
      public:
         constexpr explicit TIndex( const size_t& value ) : m_value( value ) { }
         template< eIndexType T >
            constexpr explicit TIndex( const TIndex< T >& object ) : m_value( object.value( ) ) { }
         constexpr ~TIndex( ) { }

      public:
         operator size_t( ) const
         {
            return m_value;
         }
         TIndex& operator++( )
         {
            ++m_value;
            return *this;
         }
         bool operator==( const TIndex& object ) const
         {
            return object.m_value == m_value;
         }
         bool operator!=( const TIndex& object ) const
         {
            return object.m_value != m_value;
         }
         TIndex& operator=( const size_t value )
         {
            m_value = value;
            return *this;
         }

      public:
         void value( const size_t value ) { m_value = value; }
         size_t value( ) const { return m_value; }

      public:
         size_t m_value = 0;
   };

}

namespace base::math {

   using tRow = TIndex< eIndexType::ROW >;
   using tColumn = TIndex< eIndexType::COLUMN >;
   using tElement = TIndex< eIndexType::ELEMENT >;



   template< typename T, tRow ROWS, tColumn COLUMNS >
   class TMatrix
   {
      public:
         using tMatrix = TMatrix< T, ROWS, COLUMNS >;
         using tVectorCV = TMatrix< T, tRow{ 1 }, COLUMNS >;
         using tVectorCTV = TMatrix< T, ROWS, tColumn{ 1 } >;

      public:
         TMatrix( const std::string name = "TMatrix NoName", const std::initializer_list< T >& list = { } )
            : m_name( name )
         {
            m_elements = new T* [ ROWS.value( ) ];
            for ( size_t i = 0; i < ROWS.value( ); ++i )
               m_elements[ i ] = new T [ COLUMNS.value( ) ];

            size_t index = 0;
            for( const auto& item : list )
            {
               if( index == elements( ) )
                  break;
               m_elements[ index / COLUMNS.value( ) ][ index % COLUMNS.value( ) ] = item;
               ++index;
            }
         }
         TMatrix( const tMatrix& object )
         {
            m_elements = new T* [ ROWS.value( ) ];
            for( tRow row{ 0 }; row < ROWS; ++row )
            {
               m_elements[ row.value( ) ] = new T [ COLUMNS.value( ) ];
               for( tColumn column{ 0 }; column < COLUMNS; ++column )
                  m_elements[ row.value( ) ][ column.value( ) ] = object.m_elements[ row.value( ) ][ column.value( ) ];
            }
         }
         ~TMatrix( )
         {
            for ( size_t i = 0; i < ROWS.value( ); i++ )
               delete [] m_elements[ i ];
            delete [] m_elements;
         }

         size_t elements( ) const
         {
            static const size_t size = ROWS * COLUMNS;
            return size;
         }

      public:
         operator T( ) const
         {
            T result = { };
            for( tRow row{ 0 }; row < ROWS; ++row )
               for( tColumn column{ 0 }; column < COLUMNS; ++column )
                  result += m_elements[ row.value( ) ][ column.value( ) ];
            return result;
         }

      public:
         const T& get( const tRow& row, const tColumn& column ) const
         {
            check( row, column );
            return m_elements[ row.value( ) ][ column.value( ) ];
         }

         T& get( const tRow& row, const tColumn& column )
         {
            check( row, column );
            return m_elements[ row.value( ) ][ column.value( ) ];
         }

         tVectorCV get( const tRow& row ) const
         {
            check( row );
            tVectorCV vector;
            for( tColumn column{ 0 }; column < COLUMNS; ++column )
               vector.set( tRow{ 0 }, column, get( row, column ) );

            return vector;
         }

         tVectorCTV get( const tColumn& column ) const
         {
            check( column );
            tVectorCTV vector;
            for( tRow row{ 0 }; row < ROWS; ++row )
               vector.set( row, tColumn{ 0 }, get( row, column ) );

            return vector;
         }

         void set( const tRow& row, const tColumn& column, const T& value )
         {
            check( row, column );
            m_elements[ row.value( ) ][ column.value( ) ] = value;
         }

         void print( ) const
         {
            DBG_MSG( "------------------------------ TMatrix dump begin ------------------------------" );
            DBG_MSG( "name: %s / dimention: %zu", m_name.c_str( ), ROWS * COLUMNS )
            for( tRow row{ 0 }; row < ROWS; ++row )
               base::print( m_elements[ row.value( ) ], COLUMNS, true );
            DBG_MSG( "------------------------------- TMatrix dump end -------------------------------" );
         }

      private:
         void check( const tRow& row, const tColumn& column ) const
         {
            if( row >= ROWS || column >= COLUMNS )
               throw std::invalid_argument( "row or column is out of range" );
         }
         void check( const tRow& row ) const
         {
            if( row >= ROWS )
               throw std::invalid_argument( "row is out of range" );
         }
         void check( const tColumn& column ) const
         {
            if( column >= COLUMNS )
               throw std::invalid_argument( "column is out of range" );
         }

      private:
         T** m_elements = nullptr;
         std::string m_name;
   };

   template< typename T, tRow ROWS, tColumn COLUMNS >
      using tMatrix = TMatrix< T, ROWS, COLUMNS >;
   template< typename T, tColumn COLUMNS >
      using tVectorCV = TMatrix< T, tRow{ 1 }, COLUMNS >;
   template< typename T, tRow ROWS >
      using tVectorCTV = TMatrix< T, ROWS, tColumn{ 1 } >;
   template< typename T >
      using tScalar = TMatrix< T, tRow{ 1 }, tColumn{ 1 } >;

}



namespace base::math {

   template< typename T, tRow ROWS_1, tColumn COLUMNS_1, tRow ROWS_2, tColumn COLUMNS_2 >
   TMatrix< T, ROWS_1, COLUMNS_2 > multiply( const TMatrix< T, ROWS_1, COLUMNS_1 >& matrix_1, const TMatrix< T, ROWS_2, COLUMNS_2 >& matrix_2 )
   {
      if( COLUMNS_1.value( ) != ROWS_2.value( ) )
         throw std::invalid_argument( "wrong matrix multiplication" );

      TMatrix< T, ROWS_1, COLUMNS_2 > result;
      for( tRow row{ 0 }; row < ROWS_1; ++row )
         for( tColumn column{ 0 }; column < COLUMNS_2; ++column )
            for( tRow index{ 0 }; index < ROWS_2; ++index )
               result.get( row, column ) += matrix_1.get( row, tColumn{ index } ) * matrix_2.get( index, column );
      return result;
   }

}



#else



namespace {

   template< eIndexType tType >
   class TIndex
   {
      public:
         explicit TIndex( const size_t value ) : m_value( value ) { }
         ~TIndex( ) { }

      public:
         operator size_t( ) const
         {
            return m_value;
         }
         TIndex& operator++( )
         {
            ++m_value;
            return *this;
         }
         bool operator==( const TIndex& object ) const
         {
            return object.m_value == m_value;
         }
         bool operator!=( const TIndex& object ) const
         {
            return object.m_value != m_value;
         }
         TIndex& operator=( const size_t value )
         {
            m_value = value;
            return *this;
         }

      public:
         void value( const size_t value ) { m_value = value; }
         size_t value( ) const { return m_value; }

      private:
         size_t m_value = 0;
   };

}

namespace base::math {

   using tRow = TIndex< eIndexType::ROW >;
   using tColumn = TIndex< eIndexType::COLUMN >;



   template< typename T, size_t ROWS, size_t COLUMNS >
   class TMatrix
   {
      public:
         using tMatrix = TMatrix< T, ROWS, COLUMNS >;
         using tVectorCV = TMatrix< T, 1, COLUMNS >;
         using tVectorCTV = TMatrix< T, ROWS, 1 >;

      public:
         TMatrix( const std::string name = "TMatrix NoName", const std::initializer_list< T >& list = { } )
            : m_name( name )
         {
            size_t index = 0;
            for( const auto& item : list )
            {
               if( index == elements( ) )
                  break;
               m_elements[ index / COLUMNS ][ index % COLUMNS ] = item;
               index++;
            }
         }
         TMatrix( const tMatrix& object )
         {
            for( tRow row{ 0 }; row < ROWS; ++row )
               for( tColumn column{ 0 }; column < COLUMNS; ++column )
                  m_elements[ row.value( ) ][ column.value( ) ] = object.m_elements[ row.value( ) ][ column.value( ) ];
         }
         ~TMatrix( ) { }

         size_t elements( ) const
         {
            static const size_t size = ROWS * COLUMNS;
            return size;
         }

      public:
         operator T( ) const
         {
            T result = { };
            for( tRow row{ 0 }; row < ROWS; ++row )
               for( tColumn column{ 0 }; column < COLUMNS; ++column )
                  result += m_elements[ row.value( ) ][ column.value( ) ];
            return result;
         }

      public:
         const T& get( const tRow& row, const tColumn& column ) const
         {
            check( row, column );
            return m_elements[ row.value( ) ][ column.value( ) ];
         }

         T& get( const tRow& row, const tColumn& column )
         {
            check( row, column );
            return m_elements[ row.value( ) ][ column.value( ) ];
         }

         tVectorCV get( const tRow& row ) const
         {
            check( row );
            tVectorCV vector;
            for( tColumn column{ 0 }; column < COLUMNS; ++column )
               vector.set( tRow{ 0 }, column, get( row, column ) );

            return vector;
         }

         tVectorCTV get( const tColumn& column ) const
         {
            check( column );
            tVectorCTV vector;
            for( tRow row{ 0 }; row < ROWS; ++row )
               vector.set( row, tColumn{ 0 }, get( row, column ) );

            return vector;
         }

         void set( const tRow& row, const tColumn& column, const T& value )
         {
            check( row, column );
            m_elements[ row.value( ) ][ column.value( ) ] = value;
         }

         void print( ) const
         {
            DBG_MSG( "------------------------------ TMatrix dump begin ------------------------------" );
            DBG_MSG( "name: %s / dimention: %zu", m_name.c_str( ), ROWS * COLUMNS )
            for( tRow row{ 0 }; row < ROWS; ++row )
               base::print( m_elements[ row.value( ) ], COLUMNS, true );
            DBG_MSG( "------------------------------- TMatrix dump end -------------------------------" );
         }

      private:
         size_t index( const tRow& row, const tColumn& column ) const
         {
            check( row, column );
            return row * COLUMNS + column;
         }
         void check( const tRow& row, const tColumn& column ) const
         {
            if( row >= ROWS || column >= COLUMNS )
               throw std::invalid_argument( "row or column is out of range" );
         }
         void check( const tRow& row ) const
         {
            if( row >= ROWS )
               throw std::invalid_argument( "row is out of range" );
         }
         void check( const tColumn& column ) const
         {
            if( column >= COLUMNS )
               throw std::invalid_argument( "column is out of range" );
         }

      private:
         T m_elements[ ROWS ][ COLUMNS ] = { };
         std::string m_name;
   };

   template< typename T, size_t ROWS, size_t COLUMNS >
      using tMatrix = TMatrix< T, ROWS, COLUMNS >;
   template< typename T, size_t COLUMNS >
      using tVectorCV = TMatrix< T, 1, COLUMNS >;
   template< typename T, size_t ROWS >
      using tVectorCTV = TMatrix< T, ROWS, 1 >;
   template< typename T >
      using tScalar = TMatrix< T, 1, 1 >;

}



namespace base::math {

   template< typename T, size_t ROWS, size_t COLUMNS, size_t tCommon >
   TMatrix< T, ROWS, COLUMNS > multiply( const TMatrix< T, ROWS, tCommon >& matrix_1, const TMatrix< T, tCommon, COLUMNS >& matrix_2 )
   {
      TMatrix< T, ROWS, COLUMNS > result;
      for( tRow row{ 0 }; row < ROWS; ++row )
         for( tColumn column{ 0 }; column < COLUMNS; ++column )
            for( size_t index = 0; index < tCommon; ++index )
               result.get( row, column ) += matrix_1.get( row, tColumn{ index } ) * matrix_2.get( tRow{ index }, column );
      return result;
   }

}



#endif



#undef CLASS_ABBR
