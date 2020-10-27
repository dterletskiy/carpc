#pragma once

#include <omp.h>
#include <stdexcept>

#include "api/sys/helpers/functions/format.hpp"
#include "api/sys/helpers/functions/print.hpp"
#include "api/sys/math/Common.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "MATH"



namespace base::math {

   #define __INTEGRAL_TYPE__( TYPE )         std::is_integral_v< TYPE >
   #define __FLOATING_POINT_TYPE__( TYPE )   std::is_floating_point_v< TYPE >



   template< typename TYPE >
   class TMatrix
   {
      public:
         using tGenerator = std::function< TYPE( const tRow, const tColumn ) >;
         using tGeneratorBased = std::function< TYPE( const TYPE&, const tRow, const tColumn ) >;
         using tModifyer = std::function< void( TYPE&, const tRow, const tColumn ) >;

      public:
         template< typename T, typename VALUE_TYPE >
            friend typename std::enable_if_t< __INTEGRAL_TYPE__( VALUE_TYPE ) || __FLOATING_POINT_TYPE__( VALUE_TYPE ), TMatrix< T > >
               operator*( const VALUE_TYPE value, const TMatrix< T >& matrix );

      public:
         using tMatrix = TMatrix< TYPE >;

      public:
         /***************************************************************************
          *
          * Creating matrix and initializing its elements by values from initializer list.
          * If initializer list size less them matrix elements, rest of matrix elements will
          * be initialized by default values. 
          * If initializer list size more them matrix elements, rest of them will be ignored.
          *
          **************************************************************************/
         TMatrix( const tRow& rows, const tColumn& columns, const std::initializer_list< TYPE >& list = { } );

         /***************************************************************************
          *
          * Creating matrix and initializing its elements by values what are returnd from
          * fanction. This function takes Row and Column number of currently initializing element as parameter.
          *
          **************************************************************************/
         TMatrix( const tRow& rows, const tColumn& columns, tGenerator rule );

         /***************************************************************************
          *
          * Copy constructor
          *
          **************************************************************************/
         TMatrix( const tMatrix& matrix );

         /***************************************************************************
          *
          * Move constructor
          *
          **************************************************************************/
         TMatrix( tMatrix&& matrix );

         /***************************************************************************
          *
          * Destructor
          *
          **************************************************************************/
         ~TMatrix( );

         /***************************************************************************
          *
          * Initializing matrix elements by values returned by lambda function (generator).
          * First parameter set the rule (what elements are effected by function).
          *
          **************************************************************************/
         void init( const eCloneType clone_type, tGenerator rule );

         /***************************************************************************
          *
          * Initializing matrix elements by values returned by lambda function.
          *
          **************************************************************************/
         void init( tGenerator rule );

         /***************************************************************************
          *
          * Print matrix information and elements
          *
          **************************************************************************/
         void print( const std::string& message = "" ) const;

      public:
         tMatrix& operator=( const tMatrix& matrix );
         tMatrix operator+( const tMatrix& matrix ) const;
         tMatrix operator-( const tMatrix& matrix ) const;
         tMatrix operator*( const tMatrix& matrix ) const;
         tMatrix operator/( const tMatrix& matrix ) const;
         template< typename VALUE_TYPE >
            typename std::enable_if_t< __INTEGRAL_TYPE__( VALUE_TYPE ) || __FLOATING_POINT_TYPE__( VALUE_TYPE ), tMatrix >
               operator*( const VALUE_TYPE& value ) const;
         template< typename VALUE_TYPE >
            typename std::enable_if_t< __INTEGRAL_TYPE__( VALUE_TYPE ) || __FLOATING_POINT_TYPE__( VALUE_TYPE ), tMatrix >
               operator/( const VALUE_TYPE& value ) const;
         tMatrix& operator+=( const tMatrix& matrix );
         tMatrix& operator-=( const tMatrix& matrix );
         tMatrix& operator*=( const tMatrix& matrix );
         tMatrix& operator/=( const tMatrix& matrix );
         template< typename VALUE_TYPE >
            typename std::enable_if_t< __INTEGRAL_TYPE__( VALUE_TYPE ) || __FLOATING_POINT_TYPE__( VALUE_TYPE ), tMatrix >
               operator*=( const VALUE_TYPE& value );
         template< typename VALUE_TYPE >
            typename std::enable_if_t< __INTEGRAL_TYPE__( VALUE_TYPE ) || __FLOATING_POINT_TYPE__( VALUE_TYPE ), tMatrix >
               operator/=( const VALUE_TYPE& value );
         operator TYPE( ) const;

      public:
         /***************************************************************************
          *
          * Multiply current matrix on other one according matrix multiplication rules.
          * New matrix will be returned.
          *
          **************************************************************************/
         tMatrix multiply( const tMatrix& matrix ) const;
         tMatrix multiply_omp( const tMatrix& matrix ) const;
         tMatrix dot( const tMatrix& matrix ) const;

         /***************************************************************************
          *
          * Return new matrix what is transposed to current one.
          *
          **************************************************************************/
         tMatrix transpose( ) const;
         tMatrix T( ) const;

         /***************************************************************************
          *
          * Apply lambda function in parameter to element of current matrix what is placed in row and column.
          * If row(column) is not set or has value == std::nullopt then this function will be applyed to each element
          * in row(column) in current matrix.
          * Current matrix will be modified.
          *
          **************************************************************************/
         tMatrix& apply( tModifyer rule, const tRow::tOpt row = std::nullopt, const tColumn::tOpt column = std::nullopt );

         /***************************************************************************
          *
          * Apply lambda function in seond parameter to each element of matrix in ferst parameter
          * and store result to corresponding element of current matrix.
          * Current matrix will be modified.
          * This function like a copy constructor but with aditional applying function to prime matrix.
          *
          **************************************************************************/
         tMatrix& apply( const tMatrix& matrix, tGeneratorBased rule );

         /***************************************************************************
          *
          * Returns the sum of all matrix's elements.
          * If action is set then this function applyes to each element before adding it
          * to common result.
          * Current matrix will not be changed.
          *
          **************************************************************************/
         TYPE sum( std::optional< tGeneratorBased > action ) const;

         /***************************************************************************
          *
          * Returns the product of all matrix's elements.
          * If action is set then this function applyes to each element before multiply it
          * to common result.
          * Current matrix will not be changed.
          *
          **************************************************************************/
         TYPE prod( std::optional< tGeneratorBased > action ) const;

         /***************************************************************************
          *
          * Returns minimum(maximum) element in current matrix
          *
          **************************************************************************/
         TYPE min( ) const;
         TYPE max( ) const;

         /***************************************************************************
          *
          * Returns minor matrix for current matrix and provided vector of rows and columns
          *
          **************************************************************************/
         tMatrix minor( const std::vector< tCoord >& vector ) const;

         /***************************************************************************
          *
          * Returns additional minor matrix for current matrix and provided vector of rows and columns
          *
          **************************************************************************/
         tMatrix minor_a( const std::vector< tCoord >& vector ) const;

      public:
         const TYPE& get( const tRow& row, const tColumn& column ) const;
         TYPE& get( const tRow& row, const tColumn& column );
         tMatrix get( const tRow& row ) const;
         tMatrix get( const tColumn& column ) const;
         void set( const tRow& row, const tColumn& column, const TYPE& value );

      public:
         std::size_t elements( ) const;
         std::size_t index( const tRow& row, const tColumn& column ) const;
         tRow row( const std::size_t index ) const;
         tColumn column( const std::size_t index ) const;
         tRow rows( ) const;
         tColumn columns( ) const;

      private:
         void check_operation( const tRow& row, const tColumn& column ) const;
         void check_access( const tRow& row, const tColumn& column ) const;
         void check_access( const std::size_t index ) const;

      private:
         TYPE**         m_elements = nullptr;
         tRow           m_rows;
         tColumn        m_columns;
   };



   template< typename T, typename VALUE_TYPE >
   typename std::enable_if_t< __INTEGRAL_TYPE__( VALUE_TYPE ) || __FLOATING_POINT_TYPE__( VALUE_TYPE ), TMatrix< T > >
      operator*( const VALUE_TYPE value, const TMatrix< T >& matrix )
   {
      auto rule = [matrix, value]( const tRow row, const tColumn column )
      {
         return matrix.m_elements[ row.value( ) ][ column.value( ) ] * value;
      };
      return TMatrix< T >( matrix.m_rows, matrix.m_columns, rule );
   }



   template< typename TYPE >
   TMatrix< TYPE >::TMatrix( const tRow& rows, const tColumn& columns, const std::initializer_list< TYPE >& list )
      : m_rows( rows )
      , m_columns( columns )
   {
      m_elements = new TYPE* [ m_rows.value( ) ];
      for ( std::size_t i = 0; i < m_rows.value( ); ++i )
         m_elements[ i ] = new TYPE [ m_columns.value( ) ];

      std::size_t index = 0;
      for( const auto& item : list )
      {
         if( index == elements( ) )
            break;
         m_elements[ index / m_columns.value( ) ][ index % m_columns.value( ) ] = item;
         ++index;
      }
      for( ; index < elements( ); ++index )
         m_elements[ index / m_columns.value( ) ][ index % m_columns.value( ) ] = { };
   }

   template< typename TYPE >
   TMatrix< TYPE >::TMatrix( const tRow& rows, const tColumn& columns, tGenerator rule )
      : m_rows( rows )
      , m_columns( columns )
   {
      m_elements = new TYPE* [ m_rows.value( ) ];
      for ( std::size_t i = 0; i < m_rows.value( ); ++i )
         m_elements[ i ] = new TYPE [ m_columns.value( ) ];

      init( rule );
   }

   template< typename TYPE >
   TMatrix< TYPE >::TMatrix( const tMatrix& matrix )
      : m_rows( matrix.m_rows )
      , m_columns( matrix.m_columns )
   {
      m_elements = new TYPE* [ m_rows.value( ) ];
      for( tRow row{ 0 }; row < m_rows; ++row )
      {
         m_elements[ row.value( ) ] = new TYPE [ m_columns.value( ) ];
         for( tColumn column{ 0 }; column < m_columns; ++column )
            m_elements[ row.value( ) ][ column.value( ) ] = matrix.m_elements[ row.value( ) ][ column.value( ) ];
      }
   }

   template< typename TYPE >
   TMatrix< TYPE >::TMatrix( tMatrix&& matrix )
      : m_rows( matrix.m_rows )
      , m_columns( matrix.m_columns )
   {
      m_elements = matrix.m_elements;
      matrix.m_elements = nullptr;
      matrix.m_rows = tRow{ 0 };
      matrix.m_columns = tColumn{ 0 };
   }

   template< typename TYPE >
   TMatrix< TYPE >::~TMatrix( )
   {
      for ( std::size_t i = 0; i < m_rows.value( ); i++ )
         delete [] m_elements[ i ];
      delete [] m_elements;
   }

   template< typename TYPE >
   void TMatrix< TYPE >::init( const eCloneType clone_type, tGenerator rule )
   {
      switch( clone_type )
      {
         case eCloneType::All:
         {
            for( tRow row{ 0 }; row < m_rows; ++row )
               for( tColumn column{ 0 }; column < m_columns; ++column )
                  m_elements[ row.value( ) ][ column.value( ) ] = rule( row, column );
            break;
         }
         case eCloneType::Diag:
         {
            for( tRow row{ 0 }; row < m_rows; ++row )
               for( tColumn column{ 0 }; column < m_columns; ++column )
                  if( row == column )
                     m_elements[ row.value( ) ][ column.value( ) ] = rule( row, column );
                  else
                     m_elements[ row.value( ) ][ column.value( ) ] = { };
            break;
         }
         case eCloneType::SideDiag:
         {
            for( tRow row{ 0 }; row < m_rows; ++row )
               for( tColumn column{ 0 }; column < m_columns; ++column )
                  if( row == m_columns - column - 1 )
                     m_elements[ row.value( ) ][ column.value( ) ] = rule( row, column );
                  else
                     m_elements[ row.value( ) ][ column.value( ) ] = { };
            break;
         }
         case eCloneType::BothDiag:
         {
            for( tRow row{ 0 }; row < m_rows; ++row )
               for( tColumn column{ 0 }; column < m_columns; ++column )
                  if( ( row == column ) || ( row == m_columns - column - 1 ) )
                     m_elements[ row.value( ) ][ column.value( ) ] = rule( row, column );
                  else
                     m_elements[ row.value( ) ][ column.value( ) ] = { };
            break;
         }
      }
   }

   template< typename TYPE >
   void TMatrix< TYPE >::init( tGenerator rule )
   {
      for( tRow row{ 0 }; row < m_rows; ++row )
         for( tColumn column{ 0 }; column < m_columns; ++column )
            m_elements[ row.value( ) ][ column.value( ) ] = rule( row, column );
   }

   template< typename TYPE >
   void TMatrix< TYPE >::print( const std::string& message ) const
   {
      if( false == message.empty( ) )
      {
         DBG_MSG( "%s", message.c_str( ) );
      }

      DBG_MSG( "dimention: %zu x %zu", m_rows.value( ), m_columns.value( ) )
      for( tRow row{ 0 }; row < m_rows; ++row )
         base::print( m_elements[ row.value( ) ], m_columns, true );
   }

   template< typename TYPE >
   TMatrix< TYPE >& TMatrix< TYPE >::operator=( const TMatrix< TYPE >& matrix )
   {
      check_operation( matrix.rows( ), matrix.columns( ) );

      for( tRow row{ 0 }; row < m_rows; ++row )
         for( tColumn column{ 0 }; column < m_columns; ++column )
            m_elements[ row.value( ) ][ column.value( ) ] = matrix.m_elements[ row.value( ) ][ column.value( ) ];

      return *this;
   }

   template< typename TYPE >
   TMatrix< TYPE > TMatrix< TYPE >::operator+( const TMatrix< TYPE >& matrix ) const
   {
      check_operation( matrix.rows( ), matrix.columns( ) );

      auto rule = [this, matrix]( const tRow row, const tColumn column )
      {
         return m_elements[ row.value( ) ][ column.value( ) ] + matrix.m_elements[ row.value( ) ][ column.value( ) ];
      };
      return TMatrix< TYPE >( m_rows, m_columns, rule );
   }

   template< typename TYPE >
   TMatrix< TYPE > TMatrix< TYPE >::operator-( const TMatrix< TYPE >& matrix ) const
   {
      check_operation( matrix.rows( ), matrix.columns( ) );

      auto rule = [this, matrix]( const tRow row, const tColumn column )
      {
         return m_elements[ row.value( ) ][ column.value( ) ] - matrix.m_elements[ row.value( ) ][ column.value( ) ];
      };
      return TMatrix< TYPE >( m_rows, m_columns, rule );
   }

   template< typename TYPE >
   TMatrix< TYPE > TMatrix< TYPE >::operator*( const TMatrix< TYPE >& matrix ) const
   {
      check_operation( matrix.rows( ), matrix.columns( ) );

      auto rule = [this, matrix]( const tRow row, const tColumn column )
      {
         return m_elements[ row.value( ) ][ column.value( ) ] * matrix.m_elements[ row.value( ) ][ column.value( ) ];
      };
      return TMatrix< TYPE >( m_rows, m_columns, rule );
   }

   template< typename TYPE >
   TMatrix< TYPE > TMatrix< TYPE >::operator/( const TMatrix< TYPE >& matrix ) const
   {
      check_operation( matrix.rows( ), matrix.columns( ) );

      auto rule = [this, matrix]( const tRow row, const tColumn column )
      {
         return m_elements[ row.value( ) ][ column.value( ) ] / matrix.m_elements[ row.value( ) ][ column.value( ) ];
      };
      return TMatrix< TYPE >( m_rows, m_columns, rule );
   }

   template< typename TYPE >
   template< typename VALUE_TYPE >
   typename std::enable_if_t< __INTEGRAL_TYPE__( VALUE_TYPE ) || __FLOATING_POINT_TYPE__( VALUE_TYPE ), TMatrix< TYPE > >
      TMatrix< TYPE >::operator*( const VALUE_TYPE& value ) const
   {
      auto rule = [this, value]( const tRow row, const tColumn column )
      {
         return m_elements[ row.value( ) ][ column.value( ) ] * value;
      };
      return TMatrix< TYPE >( m_rows, m_columns, rule );
   }

   template< typename TYPE >
   template< typename VALUE_TYPE >
   typename std::enable_if_t< __INTEGRAL_TYPE__( VALUE_TYPE ) || __FLOATING_POINT_TYPE__( VALUE_TYPE ), TMatrix< TYPE > >
      TMatrix< TYPE >::operator/( const VALUE_TYPE& value ) const
   {
      auto rule = [this, value]( const tRow row, const tColumn column )
      {
         return m_elements[ row.value( ) ][ column.value( ) ] / value;
      };
      return TMatrix< TYPE >( m_rows, m_columns, rule );
   }

   template< typename TYPE >
   TMatrix< TYPE >& TMatrix< TYPE >::operator+=( const TMatrix< TYPE >& matrix )
   {
      check_operation( matrix.rows( ), matrix.columns( ) );

      for( tRow row{ 0 }; row < m_rows; ++row )
         for( tColumn column{ 0 }; column < m_columns; ++column )
            m_elements[ row.value( ) ][ column.value( ) ] += matrix.m_elements[ row.value( ) ][ column.value( ) ];

      return *this;
   }

   template< typename TYPE >
   TMatrix< TYPE >& TMatrix< TYPE >::operator-=( const TMatrix< TYPE >& matrix )
   {
      check_operation( matrix.rows( ), matrix.columns( ) );

      for( tRow row{ 0 }; row < m_rows; ++row )
         for( tColumn column{ 0 }; column < m_columns; ++column )
            m_elements[ row.value( ) ][ column.value( ) ] -= matrix.m_elements[ row.value( ) ][ column.value( ) ];

      return *this;
   }

   template< typename TYPE >
   TMatrix< TYPE >& TMatrix< TYPE >::operator*=( const TMatrix< TYPE >& matrix )
   {
      check_operation( matrix.rows( ), matrix.columns( ) );

      for( tRow row{ 0 }; row < m_rows; ++row )
         for( tColumn column{ 0 }; column < m_columns; ++column )
            m_elements[ row.value( ) ][ column.value( ) ] *= matrix.m_elements[ row.value( ) ][ column.value( ) ];

      return *this;
   }

   template< typename TYPE >
   TMatrix< TYPE >& TMatrix< TYPE >::operator/=( const TMatrix< TYPE >& matrix )
   {
      check_operation( matrix.rows( ), matrix.columns( ) );

      for( tRow row{ 0 }; row < m_rows; ++row )
         for( tColumn column{ 0 }; column < m_columns; ++column )
            m_elements[ row.value( ) ][ column.value( ) ] /= matrix.m_elements[ row.value( ) ][ column.value( ) ];

      return *this;
   }

   template< typename TYPE >
   template< typename VALUE_TYPE >
   typename std::enable_if_t< __INTEGRAL_TYPE__( VALUE_TYPE ) || __FLOATING_POINT_TYPE__( VALUE_TYPE ), TMatrix< TYPE > >
      TMatrix< TYPE >::operator*=( const VALUE_TYPE& value )
   {
      for( tRow row{ 0 }; row < m_rows; ++row )
         for( tColumn column{ 0 }; column < m_columns; ++column )
            m_elements[ row.value( ) ][ column.value( ) ] *= value;

      return *this;
   }

   template< typename TYPE >
   template< typename VALUE_TYPE >
   typename std::enable_if_t< __INTEGRAL_TYPE__( VALUE_TYPE ) || __FLOATING_POINT_TYPE__( VALUE_TYPE ), TMatrix< TYPE > >
      TMatrix< TYPE >::operator/=( const VALUE_TYPE& value )
   {
      for( tRow row{ 0 }; row < m_rows; ++row )
         for( tColumn column{ 0 }; column < m_columns; ++column )
            m_elements[ row.value( ) ][ column.value( ) ] /= value;

      return *this;
   }

   template< typename TYPE >
   TMatrix< TYPE >::operator TYPE( ) const
   {
      TYPE result = { };
      for( tRow row{ 0 }; row < m_rows; ++row )
         for( tColumn column{ 0 }; column < m_columns; ++column )
            result += m_elements[ row.value( ) ][ column.value( ) ];
      return result;
   }

   template< typename TYPE >
   const TYPE& TMatrix< TYPE >::get( const tRow& row, const tColumn& column ) const
   {
      check_access( row, column );
      return m_elements[ row.value( ) ][ column.value( ) ];
   }

   template< typename TYPE >
   TYPE& TMatrix< TYPE >::get( const tRow& row, const tColumn& column )
   {
      check_access( row, column );
      return m_elements[ row.value( ) ][ column.value( ) ];
   }

   template< typename TYPE >
   TMatrix< TYPE > TMatrix< TYPE >::get( const tRow& row ) const
   {
      check_access( row, tColumn{ 0 } );
      TMatrix< TYPE > vector( tRow{ 1 }, m_columns );
      for( tColumn column{ 0 }; column < m_columns; ++column )
         vector.set( tRow{ 0 }, column, get( row, column ) );

      return vector;
   }

   template< typename TYPE >
   TMatrix< TYPE > TMatrix< TYPE >::get( const tColumn& column ) const
   {
      check_access( tRow{ 0 }, column );
      TMatrix< TYPE > vector( m_rows, tColumn{ 1 } );
      for( tRow row{ 0 }; row < m_rows; ++row )
         vector.set( row, tColumn{ 0 }, get( row, column ) );

      return vector;
   }

   template< typename TYPE >
   void TMatrix< TYPE >::set( const tRow& row, const tColumn& column, const TYPE& value )
   {
      check_access( row, column );
      m_elements[ row.value( ) ][ column.value( ) ] = value;
   }

   template< typename TYPE >
   std::size_t TMatrix< TYPE >::elements( ) const
   {
      return m_rows * m_columns;
   }

   template< typename TYPE >
   std::size_t TMatrix< TYPE >::index( const tRow& row, const tColumn& column ) const
   {
      check_access( row, column );
      return row * m_columns + column;
   }

   template< typename TYPE >
   tRow TMatrix< TYPE >::row( const std::size_t index ) const
   {
      check_access( index );
      return tRow{ index / m_columns };
   }

   template< typename TYPE >
   tColumn TMatrix< TYPE >::column( const std::size_t index ) const
   {
      check_access( index );
      return tColumn{ index % m_columns };
   }

   template< typename TYPE >
   tRow TMatrix< TYPE >::rows( ) const
   {
      return m_rows;
   }

   template< typename TYPE >
   tColumn TMatrix< TYPE >::columns( ) const
   {
      return m_columns;
   }

   template< typename TYPE >
   TMatrix< TYPE > TMatrix< TYPE >::multiply( const TMatrix< TYPE >& matrix ) const
   {
      if( columns( ) != matrix.rows( ) )
      {
         std::string error = base::format_string(
            "matrix multiplication with invalid shapes (", rows( ), ",", columns( ), ") (", matrix.rows( ), ",", matrix.columns( ), ")"
         );
         throw std::invalid_argument( error.c_str( ) );
      }

      TMatrix< TYPE > result( rows( ), matrix.columns( ) );
      for( tRow row{ 0 }; row < rows( ); ++row )
         for( tColumn column{ 0 }; column < matrix.columns( ); ++column )
            for( std::size_t index = 0; index < columns( ) /* matrix.rows( ) */; ++index )
               result.get( row, column ) += get( row, tColumn{ index } ) * matrix.get( tRow{ index }, column );
      return result;
   }

   template< typename TYPE >
   TMatrix< TYPE > TMatrix< TYPE >::multiply_omp( const TMatrix< TYPE >& matrix ) const
   {
      if( columns( ) != matrix.rows( ) )
      {
         std::string error = base::format_string(
            "matrix multiplication with invalid shapes (", rows( ), ",", columns( ), ") (", matrix.rows( ), ",", matrix.columns( ), ")"
         );
         throw std::invalid_argument( error.c_str( ) );
      }

      TMatrix< TYPE > result( rows( ), matrix.columns( ) );
      #pragma omp parallel for shared( result ) schedule( dynamic, 1 )
         for( tRow row = tRow( 0 ); row < rows( ); ++row )
            for( tColumn column{ 0 }; column < matrix.columns( ); ++column )
               for( std::size_t index = 0; index < columns( ) /* matrix.rows( ) */; ++index )
                  result.get( row, column ) += get( row, tColumn{ index } ) * matrix.get( tRow{ index }, column );
      return result;
   }

   template< typename TYPE >
   TMatrix< TYPE > TMatrix< TYPE >::dot( const TMatrix< TYPE >& matrix ) const
   {
      return multiply( matrix );
   }

   template< typename TYPE >
   TMatrix< TYPE > TMatrix< TYPE >::transpose( ) const
   {
      TMatrix< TYPE > result( tRow{ columns( ) }, tColumn{ rows( ) } );
      for( tRow row{ 0 }; row < rows( ); ++row )
         for( tColumn column{ 0 }; column < columns( ); ++column )
            result.set( tRow{ column }, tColumn{ row }, get( row, column ) );
      return result;
   }

   template< typename TYPE >
   TMatrix< TYPE > TMatrix< TYPE >::T( ) const
   {
      return transpose( );
   }

   template< typename TYPE >
   TMatrix< TYPE >& TMatrix< TYPE >::apply( tModifyer rule, const tRow::tOpt row, const tColumn::tOpt column )
   {
      if( std::nullopt == row && std::nullopt == column )
      {
         for( tRow my_row{ 0 }; my_row < m_rows; ++my_row )
            for( tColumn my_column{ 0 }; my_column < m_columns; ++my_column )
               rule( m_elements[ my_row.value( ) ][ my_column.value( ) ], my_row, my_column );
      }
      else if( std::nullopt == row && std::nullopt != column )
      {
         for( tRow my_row{ 0 }; my_row < m_rows; ++my_row )
            rule( m_elements[ my_row.value( ) ][ column.value( ).value( ) ], my_row, column.value( ) );
      }
      else if( std::nullopt != row && std::nullopt == column )
      {
         for( tColumn my_column{ 0 }; my_column < m_columns; ++my_column )
            rule( m_elements[ row.value( ).value( ) ][ my_column.value( ) ], row.value( ), my_column );
      }
      else
      {
         rule( m_elements[ row.value( ).value( ) ][ column.value( ).value( ) ], row.value( ), column.value( ) );
      }

      return *this;
   }

   template< typename TYPE >
   TMatrix< TYPE >& TMatrix< TYPE >::apply( const TMatrix< TYPE >& matrix, tGeneratorBased rule )
   {
      check_operation( matrix.rows( ), matrix.columns( ) );

      for( tRow row{ 0 }; row < m_rows; ++row )
         for( tColumn column{ 0 }; column < m_columns; ++column )
            m_elements[ row.value( ) ][ column.value( ) ] = rule( matrix.m_elements[ row.value( ) ][ column.value( ) ], row, column );

      return *this;
   }

   template< typename TYPE >
   TYPE TMatrix< TYPE >::sum( std::optional< tGeneratorBased > action ) const
   {
      TYPE result = { };
      if( std::nullopt != action )
      {
         for( tRow row{ 0 }; row < m_rows; ++row )
            for( tColumn column{ 0 }; column < m_columns; ++column )
               result += action.value( )( m_elements[ row.value( ) ][ column.value( ) ], row, column );
      }
      else
      {
         for( tRow row{ 0 }; row < m_rows; ++row )
            for( tColumn column{ 0 }; column < m_columns; ++column )
               result += m_elements[ row.value( ) ][ column.value( ) ];
      }
      return result;
   }

   template< typename TYPE >
   TYPE TMatrix< TYPE >::prod( std::optional< tGeneratorBased > action ) const
   {
      TYPE result = { };
      if( std::nullopt != action )
      {
         for( tRow row{ 0 }; row < m_rows; ++row )
            for( tColumn column{ 0 }; column < m_columns; ++column )
               result *= action.value( )( m_elements[ row.value( ) ][ column.value( ) ], row, column );
      }
      else
      {
         for( tRow row{ 0 }; row < m_rows; ++row )
            for( tColumn column{ 0 }; column < m_columns; ++column )
               result *= m_elements[ row.value( ) ][ column.value( ) ];
      }
      return result;
   }

   template< typename TYPE >
   TYPE TMatrix< TYPE >::min( ) const
   {
      TYPE result = m_elements[ 0 ][ 0 ];
      for( tRow row{ 0 }; row < m_rows; ++row )
         for( tColumn column{ 0 }; column < m_columns; ++column )
            if( result > m_elements[ row.value( ) ][ column.value( ) ] )
               result = m_elements[ row.value( ) ][ column.value( ) ];
      return result;
   }

   template< typename TYPE >
   TYPE TMatrix< TYPE >::max( ) const
   {
      TYPE result = m_elements[ 0 ][ 0 ];
      for( tRow row{ 0 }; row < m_rows; ++row )
         for( tColumn column{ 0 }; column < m_columns; ++column )
            if( result < m_elements[ row.value( ) ][ column.value( ) ] )
               result = m_elements[ row.value( ) ][ column.value( ) ];
      return result;
   }

   template< typename TYPE >
   TMatrix< TYPE > TMatrix< TYPE >::minor( const std::vector< tCoord >& vector ) const
   {
      for( const auto& element : vector )
         check_access( element.first, element.second );

      tMatrix result( tRow{ vector.size( ) }, tColumn{ vector.size( ) } );
      for( size_t index_row = 0; index_row < vector.size( ); ++index_row )
      {
         tRow row = vector[ index_row ].first;
         for( size_t index_column = 0; index_column < vector.size( ); ++index_column )
         {
            tColumn column = vector[ index_column ].second;
            result.set( tRow{ index_row }, tColumn{ index_column }, get( row, column ) );
         }
      }
      return result;
   }

   template< typename TYPE >
   TMatrix< TYPE > TMatrix< TYPE >::minor_a( const std::vector< tCoord >& vector ) const
   {
      for( const auto& element : vector )
         check_access( element.first, element.second );


      std::vector< tRow > rows_vector;
      for( size_t index = 0; index < rows( ); ++index )
      {
         bool is_add = true;
         for( auto& element : vector )
         {
            if( index == element.first )
            {
               is_add = false;
               break;
            }
         }
         if( true == is_add )
            rows_vector.push_back( tRow{ index } );
      }

      std::vector< tColumn > columns_vector;
      for( size_t index = 0; index < columns( ); ++index )
      {
         bool is_add = true;
         for( auto& element : vector )
         {
            if( index == element.second )
            {
               is_add = false;
               break;
            }
         }
         if( true == is_add )
            columns_vector.push_back( tColumn{ index } );
      }

      tMatrix result( tRow{ rows_vector.size( ) }, tColumn{ columns_vector.size( ) } );
      size_t index_row = 0;
      for( const tRow row : rows_vector )
      {
         size_t index_column = 0;
         for( const tColumn column : columns_vector )
         {
            result.set( tRow{ index_row }, tColumn{ index_column }, get( row, column ) );
            ++index_column;
         }
         ++index_row;
      }
      return result;
   }

   template< typename TYPE >
   void TMatrix< TYPE >::check_operation( const tRow& row, const tColumn& column ) const
   {
      if( row != m_rows || column != m_columns )
      {
         std::string error = base::format_string(
            "invalid shapes: current = (", rows( ), ",", columns( ), ") check = (", row, ",", column, ")"
         );
         throw std::invalid_argument( error.c_str( ) );
      }
   }

   template< typename TYPE >
   void TMatrix< TYPE >::check_access( const tRow& row, const tColumn& column ) const
   {
      if( row >= m_rows || column >= m_columns )
      {
         std::string error = base::format_string(
            "row or column is out of range: current = (", rows( ), ",", columns( ), ") check = (", row, ",", column, ")"
         );
         throw std::invalid_argument( error.c_str( ) );
      }
   }

   template< typename TYPE >
   void TMatrix< TYPE >::check_access( const std::size_t index ) const
   {
      if( index >= elements( ) )
         throw std::invalid_argument( "index is out of range" );
   }

}



namespace base::math {

   template< typename TYPE >
   TMatrix< TYPE > multiply( const TMatrix< TYPE >& matrix_1, const TMatrix< TYPE >& matrix_2 )
   {
      return matrix_1.multiply( matrix_2 );
   }

   template< typename TYPE >
   TMatrix< TYPE > dot( const TMatrix< TYPE >& matrix_1, const TMatrix< TYPE >& matrix_2 )
   {
      return matrix_1.dot( matrix_2 );
   }

   template< typename TYPE >
   TMatrix< TYPE > transpose( const TMatrix< TYPE >& matrix )
   {
      return matrix.transpose( );
   }

}



#undef CLASS_ABBR
