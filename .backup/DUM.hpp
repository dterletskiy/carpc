#pragma once

#include <curl/curl.h>



namespace carpc::os {

   class DUM
   {
      // https://curl.haxx.se/libcurl/c/simplessl.html
      // https://curl.haxx.se/libcurl/c/cacertinmem.html

      public:
         using tProcessor = size_t (*)( void*, size_t, size_t, void* );

      public:
         DUM( const char* url, const size_t port );
         ~DUM( );

         bool init( );

         bool perform( );

         bool verbose( const bool value = false );

         bool header_processor( tProcessor callback, void* buffer );
         bool header_processor( FILE* file );

         bool data_processor( tProcessor callback, void* buffer );
         bool data_processor( FILE* file );

         bool read_processor( tProcessor callback, void* buffer );
         bool read_processor( FILE* file );

         bool client_sertificate( const char* cert, const char* cert_type = "PEM", const char* key = nullptr, const char* key_type = "PEM", const char* password = nullptr );
         bool ssl_verify( const bool peer, const bool host );

         bool proxy_sertificate( const char* cert, const char* cert_type = "PEM", const char* key = nullptr, const char* key_type = "PEM", const char* password = nullptr );
         bool proxy_ssl_verify( const bool peer, const bool host );

      private:
         template< typename T >
            bool setopt( const CURLoption option, T value );
         void print_setopt_err( const CURLoption option, CURLcode result );

      private:
         CURL* m_curl = nullptr;
         CURLcode m_result = CURL_LAST;
         const char* m_url = nullptr;
         const size_t m_port = 0;
   };

} // namespace carpc::os




namespace carpc::os {

   template< typename T >
   bool DUM::setopt( const CURLoption option, T value )
   {
      m_result = curl_easy_setopt( m_curl, option, value );
      if( CURLE_OK != m_result )
      {
         print_setopt_err( option, m_result );
         return false;
      }
      return true;
   }

}



#if 0 // Examples

struct CulrMemory
{
   void print( const char* message = "" ) const
   {
      MSG_DBG( "%s", message );
      if( nullptr == response )
         return;
      printf( "%s\n", response );
   }

   char* response = nullptr;
   size_t size = 0;
};

size_t write_callback( void* data, size_t size, size_t nmemb, void* userp )
{
   struct CulrMemory* mem = (struct CulrMemory*)userp;
   // MSG_DBG( "data: %p / size: %zu / nmemb: %zu / userp: %p", data, size, nmemb, userp );
   // MSG_DBG( "mem->response: %p / mem->size: %zu", mem->response, mem->size );
 
   size_t realsize = size * nmemb;
   char* ptr = (char*)realloc( (void*)(mem->response), mem->size + realsize + 1 );
   if( nullptr == ptr )
     return 0;  /* out of memory! */
 
   mem->response = ptr;
   memcpy( &( mem->response[ mem->size ] ), data, realsize );
   mem->size += realsize;
   mem->response[ mem->size ] = 0;
 
   return realsize;
}

size_t read_callback( void *data, size_t size, size_t n, void *buffer )
{
   return 0;
}



int main( int argc, char** argv )
{
   const char* url = "https://www.example.com";
   const size_t port = 443;

   const char* pHeaderFile = "/mnt/p4v/Desktop/header";
   const char* pDataFile = "/mnt/p4v/Desktop/data";

   CulrMemory chunk_header;
   CulrMemory chunk_data;


   FILE* file_header = fopen( pHeaderFile, "wb" );
   FILE* file_data = fopen( pDataFile, "wb" );

   carpc::os::DUM dumm( url, port );
   dumm.init( );
   // dumm.header_processor( write_callback, (void*)&chunk_header );
   dumm.header_processor( file_header );
   // dumm.data_processor( write_callback, (void*)&chunk_data );
   dumm.data_processor( file_data );
   dumm.perform( );

   chunk_header.print( "Header" );
   chunk_data.print( "Data" );

   fclose( file_header );
   fclose( file_data );

   return 0;
}



#endif