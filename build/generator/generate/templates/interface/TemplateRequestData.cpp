#include "TemplateClass.hpp"



namespace NAMESPACE_NAME {

   CLASS_NAME::CLASS_NAME( ITER_BEGIN{FIELD|const FIELD_TYPE& _FIELD_NAME_|, }ITER_END )
      : ITER_BEGIN{FIELD|FIELD_NAME( _FIELD_NAME_ )|NL      , }ITER_END
   {
   }

   CLASS_NAME::CLASS_NAME( const CLASS_NAME& other )
      : ITER_BEGIN{FIELD|FIELD_NAME( other.FIELD_NAME )|NL      , }ITER_END
   {
   }

   bool CLASS_NAME::operator==( const CLASS_NAME& other ) const
   {
      return ITER_BEGIN{FIELD|(FIELD_NAME == other.FIELD_NAME)| && }ITER_END
   }

   bool CLASS_NAME::operator<( const CLASS_NAME& other ) const
   {
      ITER_BEGIN{FIELD|if( FIELD_NAME != other.FIELD_NAME ) return FIELD_NAME < other.FIELD_NAME;|NL      }ITER_END
      return false;
   }

   const bool CLASS_NAME::to_stream( ipc::tStream& stream ) const
   {
      return ipc::serialize( stream, ITER_BEGIN{FIELD|FIELD_NAME|, }ITER_END );
   }

   const bool CLASS_NAME::from_stream( ipc::tStream& stream )
   {
      return ipc::deserialize( stream, ITER_BEGIN{FIELD|FIELD_NAME|, }ITER_END );
   }

   const std::string CLASS_NAME::name( ) const
   {
      return { };
   }

} // namespace NAMESPACE_NAME

