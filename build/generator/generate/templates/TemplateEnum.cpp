#include "TemplateClass.hpp"



namespace NAMESPACE_NAME {

   const char* c_str( const ENUM_NAME& value )
   {
      switch( value )
      {
         ITER_BEGIN{FIELD|case ENUM_NAME::FIELD_NAME:         return "ENUM_NAME::FIELD_NAME";|NL         }ITER_END
         default:
            return "ENUM_NAME::UNDEFINED";
      }
   }

} // namespace NAMESPACE_NAME

