#pragma once

#include <cstdlib>
#include <cstdint>
#include <optional>



namespace {

   enum class eIndexType : std::uint8_t { ROW, COLUMN };

   #if __cplusplus > 201703L // C++2a

      template< eIndexType tType >
      class TIndex
      {
         public:
            constexpr explicit TIndex( const std::size_t& value ) : m_value( value ) { }
            template< eIndexType T >
               constexpr explicit TIndex( const TIndex< T >& object ) : m_value( object.value( ) ) { }
            constexpr ~TIndex( ) { }

         public:
            operator std::size_t( ) const
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
            TIndex& operator=( const std::size_t value )
            {
               m_value = value;
               return *this;
            }

         public:
            void value( const std::size_t value ) { m_value = value; }
            std::size_t value( ) const { return m_value; }

         public:
            std::size_t m_value = 0;
      };

   #else

      template< eIndexType tType >
      class TIndex
      {
         public:
            using tOpt = std::optional< TIndex< tType > >;

         public:
            explicit TIndex( const std::size_t value ) : m_value( value ) { }
            ~TIndex( ) { }

         public:
            operator std::size_t( ) const
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
            TIndex& operator+=( const TIndex& object )
            {
               m_value += object.m_value;
               return *this;
            }
            TIndex& operator+=( const std::size_t value )
            {
               m_value += value;
               return *this;
            }
            TIndex& operator=( const std::size_t value )
            {
               m_value = value;
               return *this;
            }

         public:
            void value( const std::size_t value ) { m_value = value; }
            std::size_t value( ) const { return m_value; }

         private:
            std::size_t m_value = 0;
      };

   #endif

}

namespace carpc::math {

   using tRow = TIndex< eIndexType::ROW >;
   using tColumn = TIndex< eIndexType::COLUMN >;
   using tCoord = std::pair< tRow, tColumn >;

   using tRowOpt = std::optional< TIndex< eIndexType::ROW > >;
   using tColumnOpt = std::optional< TIndex< eIndexType::COLUMN > >;
   using tCoordOpt = std::pair< tRowOpt, tColumnOpt >;

   enum class eCloneType : std::uint8_t { All, Diag, SideDiag, BothDiag };

}
