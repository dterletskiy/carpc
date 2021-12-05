#include <string>
#include "api/sys/common/IPC.hpp"



namespace NAMESPACE_NAME {

   class CLASS_NAME
   {
      public:
         CLASS_NAME( ) = default;
         CLASS_NAME( ITER_BEGIN{FIELD|const FIELD_TYPE& _FIELD_NAME_|, }ITER_END );
         CLASS_NAME( const CLASS_NAME& other );
         ~CLASS_NAME( ) = default;

      public:
         bool operator==( const CLASS_NAME& other ) const;
         bool operator<( const CLASS_NAME& other ) const;

      public:
         const bool to_stream( ipc::tStream& stream ) const;
         const bool from_stream( ipc::tStream& stream );

      public:
         const std::string name( ) const;

      private:
         ITER_BEGIN{FIELD|FIELD_TYPE FIELD_NAME = FIELD_VALUE;|NL         }ITER_END
   };

} // namespace NAMESPACE_NAME

