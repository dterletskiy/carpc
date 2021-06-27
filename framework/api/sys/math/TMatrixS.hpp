#pragma once

#include <stdexcept>
#include "api/sys/helpers/functions/print.hpp"
#include "api/sys/math/Common.hpp"



#if __cplusplus > 201703L // C++2a


namespace base::math {

   template< typename T, tRow ROWS, tColumn COLUMNS >
   class TMatrixS
   {
      public:
         using tTMatrixS = TMatrixS< T, ROWS, COLUMNS >;
         using tTVectorSCV = TMatrixS< T, tRow{ 1 }, COLUMNS >;
         using tTVectorSCTV = TMatrixS< T, ROWS, tColumn{ 1 } >;

      public:
         TMatrixS( const std::string name = "TMatrixS NoName", const std::initializer_list< T >& list = { } )
            : m_name( name )
         {
            m_elements = new T* [ ROWS.value( ) ];
            for ( std::size_t i = 0; i < ROWS.value( ); ++i )
               m_elements[ i ] = new T [ COLUMNS.value( ) ];

            std::size_t index = 0;
            for( const auto& item : list )
            {
               if( index == elements( ) )
                  break;
               m_elements[ index / COLUMNS.value( ) ][ index % COLUMNS.value( ) ] = item;
               ++index;
            }
         }
         TMatrixS( const tTMatrixS& object )
         {
            m_elements = new T* [ ROWS.value( ) ];
            for( tRow row{ 0 }; row < ROWS; ++row )
            {
               m_elements[ row.value( ) ] = new T [ COLUMNS.value( ) ];
               for( tColumn column{ 0 }; column < COLUMNS; ++column )
                  m_elements[ row.value( ) ][ column.value( ) ] = object.m_elements[ row.value( ) ][ column.value( ) ];
            }
         }
         ~TMatrixS( )
         {
            for ( std::size_t i = 0; i < ROWS.value( ); i++ )
               delete [] m_elements[ i ];
            delete [] m_elements;
         }

         std::size_t elements( ) const
         {
            static const std::size_t size = ROWS * COLUMNS;
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

         tTVectorSCV get( const tRow& row ) const
         {
            check( row );
            tTVectorSCV vector;
            for( tColumn column{ 0 }; column < COLUMNS; ++column )
               vector.set( tRow{ 0 }, column, get( row, column ) );

            return vector;
         }

         tTVectorSCTV get( const tColumn& column ) const
         {
            check( column );
            tTVectorSCTV vector;
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
            print( "------------------------------ TMatrixS dump begin ------------------------------\n" );
            print( "name: %s / dimention: %zu\n", m_name.c_str( ), ROWS * COLUMNS )
            for( tRow row{ 0 }; row < ROWS; ++row )
               base::print( m_elements[ row.value( ) ], COLUMNS, true );
            print( "------------------------------- TMatrixS dump end -------------------------------\n" );
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
      using tTMatrixS = TMatrixS< T, ROWS, COLUMNS >;
   template< typename T, tColumn COLUMNS >
      using tTVectorSCV = TMatrixS< T, tRow{ 1 }, COLUMNS >;
   template< typename T, tRow ROWS >
      using tTVectorSCTV = TMatrixS< T, ROWS, tColumn{ 1 } >;
   template< typename T >
      using tTScalarS = TMatrixS< T, tRow{ 1 }, tColumn{ 1 } >;

}



namespace base::math {

   template< typename T, tRow ROWS_1, tColumn COLUMNS_1, tRow ROWS_2, tColumn COLUMNS_2 >
   TMatrixS< T, ROWS_1, COLUMNS_2 > multiply( const TMatrixS< T, ROWS_1, COLUMNS_1 >& matrix_1, const TMatrixS< T, ROWS_2, COLUMNS_2 >& matrix_2 )
   {
      if( COLUMNS_1.value( ) != ROWS_2.value( ) )
         throw std::invalid_argument( "wrong matrix multiplication" );

      TMatrixS< T, ROWS_1, COLUMNS_2 > result;
      for( tRow row{ 0 }; row < ROWS_1; ++row )
         for( tColumn column{ 0 }; column < COLUMNS_2; ++column )
            for( tRow index{ 0 }; index < ROWS_2; ++index )
               result.get( row, column ) += matrix_1.get( row, tColumn{ index } ) * matrix_2.get( index, column );
      return result;
   }

}



#else



namespace base::math {

   template< typename T, std::size_t ROWS, std::size_t COLUMNS >
   class TMatrixS
   {
      public:
         using tTMatrixS = TMatrixS< T, ROWS, COLUMNS >;
         using tTVectorSCV = TMatrixS< T, 1, COLUMNS >;
         using tTVectorSCTV = TMatrixS< T, ROWS, 1 >;

      public:
         TMatrixS( const std::string name = "TMatrixS NoName", const std::initializer_list< T >& list = { } )
            : m_name( name )
         {
            std::size_t index = 0;
            for( const auto& item : list )
            {
               if( index == elements( ) )
                  break;
               m_elements[ index / COLUMNS ][ index % COLUMNS ] = item;
               index++;
            }
         }
         TMatrixS( const tTMatrixS& object )
         {
            for( tRow row{ 0 }; row < ROWS; ++row )
               for( tColumn column{ 0 }; column < COLUMNS; ++column )
                  m_elements[ row.value( ) ][ column.value( ) ] = object.m_elements[ row.value( ) ][ column.value( ) ];
         }
         ~TMatrixS( ) { }

         std::size_t elements( ) const
         {
            static const std::size_t size = ROWS * COLUMNS;
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

         tTVectorSCV get( const tRow& row ) const
         {
            check( row );
            tTVectorSCV vector;
            for( tColumn column{ 0 }; column < COLUMNS; ++column )
               vector.set( tRow{ 0 }, column, get( row, column ) );

            return vector;
         }

         tTVectorSCTV get( const tColumn& column ) const
         {
            check( column );
            tTVectorSCTV vector;
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
            print( "------------------------------ TMatrixS dump begin ------------------------------\n" );
            print( "name: %s / dimention: %zu\n", m_name.c_str( ), ROWS * COLUMNS );
            for( tRow row{ 0 }; row < ROWS; ++row )
               base::print( m_elements[ row.value( ) ], COLUMNS, true );
            print( "------------------------------- TMatrixS dump end -------------------------------\n" );
         }

      private:
         std::size_t index( const tRow& row, const tColumn& column ) const
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

   template< typename T, std::size_t ROWS, std::size_t COLUMNS >
      using tTMatrixS = TMatrixS< T, ROWS, COLUMNS >;
   template< typename T, std::size_t COLUMNS >
      using tTVectorSCV = TMatrixS< T, 1, COLUMNS >;
   template< typename T, std::size_t ROWS >
      using tTVectorSCTV = TMatrixS< T, ROWS, 1 >;
   template< typename T >
      using tTScalarS = TMatrixS< T, 1, 1 >;

}



namespace base::math {

   template< typename T, std::size_t ROWS, std::size_t COLUMNS, std::size_t tCommon >
   TMatrixS< T, ROWS, COLUMNS > multiply( const TMatrixS< T, ROWS, tCommon >& matrix_1, const TMatrixS< T, tCommon, COLUMNS >& matrix_2 )
   {
      TMatrixS< T, ROWS, COLUMNS > result;
      for( tRow row{ 0 }; row < ROWS; ++row )
         for( tColumn column{ 0 }; column < COLUMNS; ++column )
            for( std::size_t index = 0; index < tCommon; ++index )
               result.get( row, column ) += matrix_1.get( row, tColumn{ index } ) * matrix_2.get( tRow{ index }, column );
      return result;
   }

}



#endif



#undef CLASS_ABBR
