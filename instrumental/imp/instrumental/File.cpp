#include "File.hpp"



using namespace instrumental;



File::File( const char* const file, const int flags )
{
   init( file, flags );
}

File::~File( )
{
   close( );
}

bool File::init( const char* const file, const int flags )
{
   if( nullptr == file )
      return false;

   memset( path, 0, sizeof( path ) );
   strcpy( path, file );

   if( 0 <= flags )
      return open( flags );

   return true;
}

bool File::open( int flags )
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

bool File::close( )
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

ssize_t File::read( void* buffer, const std::size_t size, const bool match_size )
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

off_t File::lseek( const off_t offset, int whence, const bool match_size )
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
