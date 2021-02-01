#include "api/sys/oswrappers/DUM.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "DUM"

using namespace base::os;



DUM::DUM( const char* url, const size_t port )
   : m_url( url )
   , m_port( port )
{
   m_result = curl_global_init( CURL_GLOBAL_DEFAULT );
   SYS_DBG( "curl_global_init: %s (%d)", curl_easy_strerror( m_result ), m_result );
}

DUM::~DUM( )
{
   curl_easy_cleanup( m_curl );
   curl_global_cleanup( );
}

bool DUM::init( )
{
   m_curl = curl_easy_init( );
   if( nullptr == m_curl )
   {
      SYS_ERR( "curl_easy_init: error" );
      return false;
   }

   setopt( CURLOPT_URL, m_url );
   setopt( CURLOPT_PORT, m_port );
   setopt( CURLOPT_HEADERDATA, 0 );

   // setopt( CURLOPT_CAPATH, "/etc/ssl/certs/" );
   // setopt( CURLOPT_CAINFO, "/etc/ssl/certs/ca-certificates.crt" );
   // setopt( CURLOPT_PROXY_CAPATH, "/etc/ssl/certs/" );
   // setopt( CURLOPT_PROXY_CAINFO, "/etc/ssl/certs/ca-certificates.crt" );

   return true;
}

bool DUM::perform( )
{
   m_result = curl_easy_perform( m_curl );
   if( CURLE_OK != m_result )
   {
      SYS_ERR( "curl_easy_perform: %s (%d)", curl_easy_strerror( m_result ), m_result );
      return false;
   }

   return true;
}

bool DUM::verbose( const bool value )
{
   return setopt( CURLOPT_VERBOSE, value );
}

bool DUM::header_processor( tProcessor callback, void* buffer )
{
   if( false == setopt( CURLOPT_HEADERFUNCTION, callback ) )
      return false;
   if( false == setopt( CURLOPT_HEADERDATA, buffer ) )
      return false;

   return true;
}

bool DUM::header_processor( FILE* file )
{
   if( false == setopt( CURLOPT_HEADERFUNCTION, nullptr ) )
      return false;
   if( false == setopt( CURLOPT_HEADERDATA, file ) )
      return false;

   return true;
}

bool DUM::data_processor( tProcessor callback, void* buffer )
{
   if( false == setopt( CURLOPT_WRITEFUNCTION, callback ) )
      return false;
   if( false == setopt( CURLOPT_WRITEDATA, buffer ) )
      return false;

   return true;
}

bool DUM::data_processor( FILE* file )
{
   if( false == setopt( CURLOPT_WRITEFUNCTION, nullptr ) )
      return false;
   if( false == setopt( CURLOPT_WRITEDATA, file ) )
      return false;

   return true;
}

bool DUM::read_processor( tProcessor callback, void* buffer )
{
   if( false == setopt( CURLOPT_READFUNCTION, callback ) )
      return false;
   if( false == setopt( CURLOPT_READDATA, buffer ) )
      return false;

   return true;
}

bool DUM::read_processor( FILE* file )
{
   if( false == setopt( CURLOPT_READFUNCTION, nullptr ) )
      return false;
   if( false == setopt( CURLOPT_READDATA, file ) )
      return false;

   return true;
}

bool DUM::client_sertificate( const char* cert, const char* cert_type, const char* key, const char* key_type, const char* password )
{
   setopt( CURLOPT_SSLCERT, cert );
   setopt( CURLOPT_SSLCERTTYPE, cert_type );
   setopt( CURLOPT_SSLKEY, key );
   setopt( CURLOPT_SSLKEYTYPE, key_type );
   setopt( CURLOPT_KEYPASSWD, password );

   return true;
}

bool DUM::ssl_verify( const bool peer, const bool host )
{
   setopt( CURLOPT_SSL_VERIFYPEER, peer );
   setopt( CURLOPT_SSL_VERIFYHOST , host );

   return true;
}

bool DUM::proxy_sertificate( const char* cert, const char* cert_type, const char* key, const char* key_type, const char* password )
{
   setopt( CURLOPT_PROXY_SSLCERT, cert );
   setopt( CURLOPT_PROXY_SSLCERTTYPE, cert_type );
   setopt( CURLOPT_PROXY_SSLKEY, key );
   setopt( CURLOPT_PROXY_SSLKEYTYPE , key_type );
   setopt( CURLOPT_PROXY_KEYPASSWD, password );

   return true;
}

bool DUM::proxy_ssl_verify( const bool peer, const bool host )
{
   setopt( CURLOPT_PROXY_SSL_VERIFYPEER, peer );
   setopt( CURLOPT_PROXY_SSL_VERIFYHOST , host );

   return true;
}

void DUM::print_setopt_err( const CURLoption option, CURLcode result )
{
   SYS_ERR( "curl_easy_setopt(%d): %s (%d)", option, curl_easy_strerror( result ), result );
}
