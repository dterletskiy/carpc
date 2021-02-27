#include <errno.h>
#include <string.h>

#include "Application.hpp"



using namespace instrumental;



Application::Application( int pid )
{
   if( 0 == pid )
      pid = getpid( );

   sprintf( proc.exe, "/proc/%d/exe", getpid( ) );

   memset( path, 0, sizeof( path ) );
   if( -1 == readlink( proc.exe, path, sizeof( path ) ) )
   {
      error = errno;
      printf( "readlink(%s) error: %s\n", proc.exe, strerror( errno ) );
   }
}

void Application::print( ) const
{
   printf( "path: %s\n", path );
}

bool Application::open( int flags )
{
   handle = ::open( path, flags );
   if( -1 == handle )
   {
      error = errno;
      printf( "open(%s) error: %s\n", path, strerror( errno ) );
      return false;
   }

   return true;
}

bool Application::close( )
{
   int result = ::close( handle );
   if( -1 == result )
   {
      error = errno;
      printf( "close(%s) error: %s\n", path, strerror( errno ) );
      return  false;
   }

   return true;
}

ssize_t Application::read( void* buffer, const std::size_t size, const bool match_size )
{
   ssize_t result = ::read( handle, buffer, size );
   if( -1 == result )
   {
      error = errno;
      printf( "read(%s) error: %s\n", path, strerror( errno ) );
      return result;
   }

   if( true == match_size && size != static_cast< std::size_t >( result ) )
   {
      printf( "read(%s) size mismatch: %zu != %zu\n", path, size, result );
      return result;
   }

   return result;
}

off_t Application::lseek( const off_t offset, int whence, const bool match_size )
{
   const off_t result = ::lseek( handle, offset, whence );
   if( -1 == result )
   {
      printf( "lseek(%s) error: %s\n", path, strerror( errno ) );
      return result;
   }

   if( true == match_size && offset != result )
   {
      printf( "lseek(%s) result mismatch: %ld != %ld\n", path, offset, result );
      return result;
   }

   return result;
}
