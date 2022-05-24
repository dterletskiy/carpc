#include "carpc/common/Definitions.hpp"



namespace carpc {

   const char* const to_str( const eByteOrder& value )
   {
      switch( value )
      {
         case eByteOrder::O32_LITTLE_ENDIAN: return "O32_LITTLE_ENDIAN";
         case eByteOrder::O32_BIG_ENDIAN: return "O32_BIG_ENDIAN";
         case eByteOrder::O32_PDP_ENDIAN: return "O32_PDP_ENDIAN";
         case eByteOrder::O32_HONEYWELL_ENDIAN: return "O32_HONEYWELL_ENDIAN";
         default: return "UNDEFINED";
      }
      return "UNDEFINED";
   }

   const eByteOrder byte_order( )
   {
      return static_cast< eByteOrder >( o32_host_order.value );
   }

}
