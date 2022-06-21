#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/sendfile.h>
#include <cwctype>
#include <syscall.h>

#include "carpc/oswrappers/linux/filesystem.hpp"

#include "carpc/trace/Trace.hpp"
#define CLASS_ABBR "Filesystem"



namespace {
#if OS_TARGET == OS_ANDROID

   ssize_t copy_file_range( int fd_in, off64_t *off_in, int fd_out, off64_t *off_out, size_t len, unsigned int flags )
   {
      // @TDA: maybe here we should also set errno
      return ::syscall( SYS_copy_file_range, fd_in, off_in, fd_out, off_out, len, flags );
   }

#endif
}



namespace carpc::os::os_linux::filesystem {

   thread_local int error = 0;

   const char c_separator = '/';
   const std::string separator{ c_separator };

   bool mkdir( const char* const path, const mode_t mode )
   {
      int result = ::mkdir( path, mode );
      error = errno;
      if( -1 == result )
      {
         SYS_ERR( "mkdir(%s, %u): error %d", path, mode, error );
         return false;
      }

      SYS_VRB( "mkdir(%s, %u)", path, mode );
      return true;
   }

   bool mkdirat( const int dirfd, const char* const path, const mode_t mode )
   {
      int result = ::mkdirat( dirfd, path, mode );
      error = errno;
      if( -1 == result )
      {
         SYS_ERR( "mkdirat(%d, %s, %u): error %d", dirfd, path, mode, error );
         return false;
      }

      SYS_VRB( "mkdirat(%d, %s, %u)", dirfd, path, mode );
      return true;
   }

   bool rmdir( const char* const path )
   {
      int result = ::rmdir( path );
      error = errno;
      if( -1 == result )
      {
         SYS_ERR( "rmdir(%s): error %d", path, error );
         return false;
      }

      SYS_VRB( "rmdir(%s)", path );
      return true;
   }

   bool chdir( const char* const path )
   {
      int result = ::chdir( path );
      error = errno;
      if( -1 == result )
      {
         SYS_ERR( "chdir(%s): error %d", path, error );
         return false;
      }

      SYS_VRB( "chdir(%s)", path );
      return true;
   }

   bool fchdir( const int dirfd )
   {
      int result = ::fchdir( dirfd );
      error = errno;
      if( -1 == result )
      {
         SYS_ERR( "fchdir(%d): error %d", dirfd, error );
         return false;
      }

      SYS_VRB( "fchdir(%d)", dirfd );
      return true;
   }

   int open( const char* const path, const int flags, const mode_t mode )
   {
      int fd = ::open( path, flags, mode );
      error = errno;
      if( -1 == fd )
      {
         SYS_ERR( "open(%s, %d, %u): error %d", path, flags, mode, error );
         return -1;
      }

      SYS_VRB( "open(%s, %d, %u): %d", path, flags, mode, fd );
      return fd;
   }

   int openat( const int dirfd, const char* const path, const int flags, const mode_t mode )
   {
      int fd = ::openat( dirfd, path, flags, mode );
      error = errno;
      if( -1 == fd )
      {
         SYS_ERR( "openat(%d, %s, %d, %u): error %d", dirfd, path, flags, mode, error );
         return -1;
      }

      SYS_VRB( "openat(%d, %s, %d, %u): %d", dirfd, path, flags, mode, fd );
      return fd;
   }

   bool close( const int fd )
   {
      int result = ::close( fd );
      error = errno;
      if( -1 == result )
      {
         SYS_ERR( "close(%d): error %d", fd, error );
         return false;
      }

      SYS_VRB( "close(%d)", fd );
      return true;
   }

   bool unlink( const char* const path )
   {
      int result = ::unlink( path );
      error = errno;
      if( -1 == result )
      {
         SYS_ERR( "unlink(%s): error %d", path, error );
         return false;
      }

      SYS_VRB( "unlink(%s)", path );
      return true;
   }

   bool rename( const char* const oldpath, const char* const newpath )
   {
      int result = ::rename( oldpath, newpath );
      error = errno;
      if( -1 == result )
      {
         SYS_ERR( "rename(%s, %s): error %d", oldpath, newpath, error );
         return false;
      }

      SYS_VRB( "rename(%s, %s)", oldpath, newpath );
      return true;
   }

   bool renameat( const int olddirfd, const char* const oldpath, const int newdirfd, const char* const newpath )
   {
      int result = ::renameat( olddirfd, oldpath, newdirfd, newpath );
      error = errno;
      if( -1 == result )
      {
         SYS_ERR( "renameat(%d, %s, %d, %s): error %d", olddirfd, oldpath, newdirfd, newpath, error );
         return false;
      }

      SYS_VRB( "renameat(%d, %s, %d, %s)", olddirfd, oldpath, newdirfd, newpath );
      return true;
   }

   bool renameat2( const int olddirfd, const char* const oldpath, const int newdirfd, const char* const newpath, const unsigned int flags )
   {
      int result = ::renameat2( olddirfd, oldpath, newdirfd, newpath, flags );
      error = errno;
      if( -1 == result )
      {
         SYS_ERR( "renameat2(%d, %s, %d, %s, %u): error %d", olddirfd, oldpath, newdirfd, newpath, flags, error );
         return false;
      }

      SYS_VRB( "renameat2(%d, %s, %d, %s, %u)", olddirfd, oldpath, newdirfd, newpath, flags );
      return true;
   }

   ssize_t sendfile( const int source, const int destination, off_t* const offset, const std::size_t count )
   {
      ssize_t sent_size = ::sendfile( source, destination, offset, count );
      error = errno;
      if( -1 == sent_size )
      {
         SYS_ERR( "sendfile(%d, %d, %p, %zu): error %d", source, destination, offset, count, error );
         return -1;
      }
      if( count != static_cast< std::size_t >( sent_size ) )
      {
         SYS_WRN( "sendfile(%d, %d, %p, %zu): %zu != %zd", source, destination, offset, count, count, sent_size );
         return sent_size;
      }

      SYS_VRB( "sendfile(%d, %d, %p, %zu)", source, destination, offset, count );
      return sent_size;
   }

   ssize_t copy_file_range( const int fd_in, off64_t* const off_in,
                            const int fd_out, off64_t* const off_out,
                            const std::size_t len, const unsigned int flags
   )
   {
      ssize_t copied_size = ::copy_file_range( fd_in, off_in, fd_out, off_out, len, flags );
      error = errno;
      if( -1 == copied_size )
      {
         SYS_ERR( "copy_file_range(%d, %p, %d, %p, %zu, %u ): error %d",
            fd_in, off_in, fd_out, off_out, len, flags, error
         );
         return -1;
      }
      if( len != static_cast< std::size_t >( copied_size ) )
      {
         SYS_WRN( "copy_file_range(%d, %p, %d, %p, %zu, %u ):  %zu != %zd",
            fd_in, off_in, fd_out, off_out, len, flags, len, copied_size
         );
         return copied_size;
      }

      SYS_VRB( "copy_file_range(%d, %p, %d, %p, %zu, %u)", fd_in, off_in, fd_out, off_out, len, flags );
      return copied_size;
   }

   ssize_t read( const int fd, void* const buffer, const std::size_t count )
   {
      ssize_t read_size = ::read( fd, buffer, count );
      error = errno;
      if( -1 == read_size )
      {
         SYS_ERR( "read(%d, %p, %zu): error %d", fd, buffer, count, error );
         return -1;
      }
      if( count != static_cast< std::size_t >( read_size ) )
      {
         SYS_WRN( "read(%d, %p, %zu) %zu != %zd", fd, buffer, count, count, read_size );
         return read_size;
      }

      SYS_VRB( "read(%d, %p, %zu)", fd, buffer, count );
      return read_size;
   }

   ssize_t write( const int fd , void* const buffer, const std::size_t count )
   {
      ssize_t wrote_size = ::write( fd, buffer, count );
      error = errno;
      if( -1 == wrote_size )
      {
         SYS_ERR( "write(%d, %p, %zu): error %d", fd, buffer, count, error );
         return -1;
      }
      if( count != static_cast< std::size_t >( wrote_size ) )
      {
         SYS_WRN( "write(%d, %p, %zu) %zu != %zd", fd, buffer, count, count, wrote_size );
         return wrote_size;
      }

      SYS_VRB( "write(%d, %p, %zu)", fd, buffer, count );
      return wrote_size;
   }

   off_t lseek( const int fd, const off_t offset, const int whence )
   {
      ssize_t offset_result = ::lseek( fd, offset, whence );
      error = errno;
      if( -1 == offset_result )
      {
         SYS_ERR( "lseek(%d, %d, %d): error %d", fd, offset, whence, error );
         return offset_result;
      }

      if( offset != offset_result )
      {
         SYS_WRN( "lseek(%d, %d, %d) result mismatch: %ld != %ld", fd, offset, whence, offset, offset_result );
      }
      else
      {
         SYS_VRB( "lseek(%d, %d, %d)", fd, offset, whence );
      }

      return offset_result;
   }

} // namespace carpc::os::os_linux::filesystem

namespace carpc::os::os_linux::filesystem {

   DIR* opendir( const char* const path )
   {
      DIR* dir = ::opendir( path );
      error = errno;
      if( nullptr == dir )
      {
         SYS_ERR( "opendir(%s): error %d", path, error );
         return nullptr;
      }

      SYS_ERR( "opendir(%s): %p", path, dir );
      return dir;
   }

   bool closedir( DIR* const dir )
   {
      int result = ::closedir( dir );
      error = errno;
      if( -1 == result )
      {
         SYS_ERR( "closedir(%p): error %d", dir, error );
         return false;
      }

      SYS_VRB( "closedir(%p)", dir );
      return true;
   }

   dirent* readdir( DIR* const dir )
   {
      const int error_backup = errno;
      dirent* entry = ::readdir( dir );
      error = errno;
      if( nullptr == entry && error_backup != error )
      {
         SYS_ERR( "readdir(%p): error %d", dir, error );
         return nullptr;
      }

      SYS_VRB( "readdir(%p): %p", dir, entry );
      return entry;
   }

   FILE* fopen( const char* const path, const char* const mode )
   {
      FILE* fd = ::fopen( path, mode );
      error = errno;
      if( nullptr == fd )
      {
         SYS_ERR( "fopen(%s, %s): error %d", path, mode, error );
         return fd;
      }

      SYS_VRB( "fopen(%s, %s): %p", path, mode, fd );
      return fd;
   }

   bool fclose( FILE* const stream )
   {
      int result = ::fclose( stream );
      error = errno;
      if( EOF == result )
      {
         SYS_ERR( "fclose(%p): error %d", stream, error );
         return false;
      }

      SYS_VRB( "fclose(%p)", stream );
      return true;
   }

   std::size_t fread( void* const ptr, std::size_t size, std::size_t nmemb, FILE* const stream )
   {
      std::size_t read_nmemb = ::fread( ptr, size, nmemb, stream );
      error = errno;
      if( nmemb != read_nmemb )
      {
         SYS_ERR( "fread(%p, %zu, %zu, %p): error %d / bytes %zu", ptr, size, nmemb, stream, error, read_nmemb );
         return read_nmemb;
      }

      SYS_VRB( "fread(%p, %zu, %zu, %p)", ptr, size, nmemb, stream );
      return read_nmemb;
   }

   std::size_t fwrite( void* const ptr, std::size_t size, std::size_t nmemb, FILE* const stream )
   {
      std::size_t wrote_nmemb = ::fwrite( ptr, size, nmemb, stream );
      error = errno;
      if( nmemb != wrote_nmemb )
      {
         SYS_ERR( "fwrite(%p, %zu, %zu, %p): error %d / bytes %zu", ptr, size, nmemb, stream, error, wrote_nmemb );
         return wrote_nmemb;
      }

      SYS_VRB( "fwrite(%p, %zu, %zu, %p)", ptr, size, nmemb, stream );
      return wrote_nmemb;
   }

   int fgetc( FILE* const stream )
   {
      int result = ::fgetc( stream );
      error = errno;
      if( EOF == result )
      {
         SYS_ERR( "fgetc(%p): error %d", stream, error );
         return -1;
      }

      SYS_VRB( "fgetc(%p)", stream );
      return result;      
   }

   int getc( FILE* const stream )
   {
      int result = ::getc( stream );
      error = errno;
      if( EOF == result )
      {
         SYS_ERR( "getc(%p): error %d", stream, error );
         return -1;
      }

      SYS_VRB( "getc(%p)", stream );
      return result;      
   }

   int getchar( )
   {
      int result = ::getchar( );
      error = errno;
      if( EOF == result )
      {
         SYS_ERR( "getchar( ): error %d", error );
         return -1;
      }

      SYS_VRB( "getchar( )" );
      return result;  
   }

   char* fgets( char* const s, const int n, FILE* const stream )
   {
      char* result = ::fgets( s, n, stream );
      error = errno;
      if( nullptr == result )
      {
         SYS_ERR( "fgets(%s, %d, %p): error %d", s, n, stream, error );
         return nullptr;
      }

      SYS_VRB( "fgets(%s, %d, %p)", s, n, stream );
      return result;  
   }

   int fputc( const int c, FILE* const stream )
   {
      int result = ::fputc( c, stream );
      error = errno;
      if( EOF == result )
      {
         SYS_ERR( "fputc(%c, %p): error %d", c, stream, error );
         return -1;
      }

      SYS_VRB( "fputc(%c, %p)", c, stream );
      return result;  
   }

   int putc( const int c, FILE* const stream )
   {
      int result = ::putc( c, stream );
      error = errno;
      if( EOF == result )
      {
         SYS_ERR( "putc(%c, %p): error %d", c, stream, error );
         return -1;
      }

      SYS_VRB( "putc(%c, %p)", c, stream );
      return result;  
   }

   int putchar( const int c )
   {
      int result = ::putchar( c );
      error = errno;
      if( EOF == result )
      {
         SYS_ERR( "putchar(%c): error %d", c, error );
         return -1;
      }

      SYS_VRB( "putchar(%c)", c );
      return result;  
   }

   int fputs( char* const s, FILE* const stream )
   {
      int result = ::fputs( s, stream );
      error = errno;
      if( EOF == result )
      {
         SYS_ERR( "fputs(%s, %p): error %d", s, stream, error );
         return EOF;
      }

      SYS_VRB( "fputs(%s, %p)", s, stream );
      return result;  
   }

   void clearerr( FILE* const stream )
   {
      ::clearerr( stream );
   }

   int feof( FILE* const stream )
   {
      return ::feof( stream );
   }

   int ferror( FILE* const stream )
   {
      return ::ferror( stream );
   }

   int fileno( FILE* const stream )
   {
      int result = ::fileno( stream );
      error = errno;
      if( -1 == result )
      {
         SYS_ERR( "fileno(%p): error %d", stream, error );
         return -1;
      }

      SYS_VRB( "fileno(%p)", stream );
      return result;
   }

} // namespace carpc::os::os_linux::filesystem

namespace carpc::os::os_linux::filesystem {

   std::string& canonify( std::string& path )
   {
      std::size_t pos;
      while( std::string::npos != ( pos = path.find( "\\" ) ) )
         path.replace( pos, 1, separator );

      if( c_separator == path.back( ) )
         path.pop_back( );

      return path;
   }

   std::string get_path_leaf( const std::string& path )
   {
      std::string result;
      const std::size_t sep = path.find_last_of( separator );
      if ( std::string::npos != sep )
         result = path.substr( sep + 1, path.size( ) - sep - 1 );

      return result;
   }

   std::string& remove_path_leaf( std::string& path )
   {
      const std::size_t sep = path.find_last_of( separator );
      if ( std::string::npos != sep )
         path = path.substr( 0, sep );

      return path;
   }

   std::string& add_path_leaf( std::string& path, const std::string& leaf )
   {
      path = path + separator + leaf;
      canonify( path );
      return path;
   }

   std::string get_parent_path( const std::string& path )
   {
      std::string result = path;
      remove_path_leaf( result );

      return result;
   }

   std::string& rename_path_leaf( std::string& path, const std::string& rename_with )
   {
      canonify( path );
      remove_path_leaf( path );
      path.push_back( c_separator );
      path += rename_with;
      return path;
   }

   bool is_regular_file( const std::string& path )
   {
      struct stat path_stat;
      return 0 == ::stat( path.c_str( ), &path_stat ) && S_ISREG( path_stat.st_mode );
   }

   bool is_directory( const std::string& path )
   {
      struct stat path_stat;
      return 0 == ::stat( path.c_str( ), &path_stat ) && S_ISDIR( path_stat.st_mode );
   }

   bool is_root( const std::string& path )
   {
      switch( path.size( ) )
      {
         case 1:  return c_separator == path[0];
         case 2:  return std::iswalpha( path[0] ) && ( ':' == path[1] );
         case 3:  return std::iswalpha( path[0] ) && ( ':' == path[1] ) && ( c_separator == path[2] );
         default: return false;
      }

      return false;
   }

   bool is_exists( const std::string& path )
   {
      struct stat buffer;
      return 0 == ::stat( path.c_str( ), &buffer );
   }

   bool is_directory_empty( const std::string& path )
   {
      if( false == is_directory( path ) )
         return false;

      DIR* dir = filesystem::opendir( path.c_str( ) );
      if( nullptr == dir )
         return false;

      std::size_t count = 0;
      dirent* entry;
      while( nullptr != ( entry = filesystem::readdir( dir ) ) )
         if( 2 < ++count )
            break;

      filesystem::closedir( dir );

      return 2 >= count;
   }

   bool create_directory( const std::string& path, const mode_t mode, const bool is_recursive )
   {
      SYS_VRB( "%s", path.c_str( ) );

      if( is_recursive )
      {
         std::string parent_path = get_parent_path( path );
         if( false == is_exists( parent_path ) )
            if( false == create_directory( parent_path, mode, true ) )
               return false;
      }

      const bool result = filesystem::mkdir( path.c_str( ), mode );

      return result || ( !result && EEXIST == error );
   }

   bool copy_file( const std::string& source, const std::string& destination, mode_t mode, const bool use_send_file )
   {
      SYS_VRB( "source: %s / destination: %s", source.c_str( ), destination.c_str( ) );

      if( false == is_regular_file( source ) )
         return false;

      if( is_exists( destination ) )
         return false;

      int fd_source = filesystem::open( source.c_str( ), O_RDONLY, 0 );
      if( -1 == fd_source )
         return false;

      struct stat stat_source;
      ::fstat( fd_source, &stat_source );

      if( 0 == mode )
         mode = stat_source.st_mode;

      int fd_destination = filesystem::open( destination.c_str( ), O_WRONLY | O_CREAT, mode );
      if( -1 == fd_destination )
      {
         filesystem::close( fd_source );
         return false;
      }

      ssize_t transfered_bytes = 0;
      if( use_send_file )
      {
         transfered_bytes = filesystem::sendfile( fd_destination, fd_source, 0, stat_source.st_size );
      }
      else
      {
         const std::size_t BUFFER_SIZE = 1024*1024;//stat_source.st_size;
         void* p_buffer = malloc( BUFFER_SIZE );
         ssize_t bytes = 0;
         while( 0 < ( bytes = filesystem::read( fd_source, p_buffer, BUFFER_SIZE ) ) )
         {
            transfered_bytes += bytes;
            filesystem::write( fd_destination, p_buffer, bytes );
         }
         free( p_buffer );
      }

      filesystem::close( fd_source );
      filesystem::close( fd_destination );

      if( transfered_bytes != (ssize_t)stat_source.st_size )
      {
         SYS_WRN( "%zd != %zd", transfered_bytes, (ssize_t)stat_source.st_size );
      }

      return transfered_bytes == (ssize_t)stat_source.st_size;
   }

   bool fcopy_file( const std::string& source, const std::string& destination )
   {
      SYS_VRB( "source: %s / destination: %s", source.c_str( ), destination.c_str( ) );

      if( false == is_regular_file( source ) )
         return false;

      if( is_exists( destination ) )
         return false;

      FILE* fd_source = filesystem::fopen( source.c_str( ), "r" );
      if( nullptr == fd_source )
         return false;

      struct stat stat_source;
      ::fstat( filesystem::fileno( fd_source ), &stat_source );

      FILE* fd_destination = filesystem::fopen( destination.c_str( ), "w" );
      if( nullptr == fd_destination )
      {
         filesystem::fclose( fd_source );
         return false;
      }

      const std::size_t BUFFER_SIZE = 1024*1024;//stat_source.st_size;
      void* p_buffer = malloc( BUFFER_SIZE );

      std::size_t transfered_bytes = 0;
      std::size_t bytes = 0;
      while( 0 < ( bytes = filesystem::fread( p_buffer, 1, BUFFER_SIZE, fd_source ) ) )
      {
         transfered_bytes += bytes;
         filesystem::fwrite( p_buffer, 1, bytes, fd_destination );
      }

      free( p_buffer );

      filesystem::fclose( fd_source );
      filesystem::fclose( fd_destination );

      if( transfered_bytes != static_cast< std::size_t >( stat_source.st_size ) )
      {
         SYS_WRN( "%zd != %zd", transfered_bytes, stat_source.st_size );
      }

      return transfered_bytes == static_cast< std::size_t >( stat_source.st_size );
   }

   bool copy_directory( const std::string& source, const std::string& destination, const bool is_recursive )
   {
      SYS_VRB( "source: %s / destination: %s", source.c_str( ), destination.c_str( ) );

      if( false == is_directory( source ) )
      {
         SYS_ERR( "not a directory" );
         return false;
      }

      if( false == is_exists( destination ) )
      {
         if( false == create_directory( destination ) )
            return false;
      }
      else if( false == is_directory( destination ) )
      {
         return false;
      }

      DIR* dir = filesystem::opendir( source.c_str( ) );
      if( nullptr == dir )
         return false;

      bool result = true;
      struct dirent* entry;
      while( nullptr != ( entry = filesystem::readdir( dir ) ) )
      {
         if( ( 0 == strcmp( entry->d_name, "." ) ) || ( 0 == strcmp( entry->d_name, ".." ) ) )
            continue;

         const std::string source_child = source + separator + std::string{ entry->d_name };
         const std::string destination_child = destination + separator + std::string{ entry->d_name };

         if( is_regular_file( source_child ) )
         {
            result &= copy_file( source_child, destination_child );
            if( false == result )
               break;
         }
         else if( is_directory( source_child ) )
         {
            result &= create_directory( destination_child );
            if( false == result )
               break;

            if( is_recursive )
            {
               result &= copy_directory( source_child, destination_child, true );
               if( false == result )
                  break;
            }
         }
      }

      filesystem::closedir( dir );

      return true;
   }

   bool copy( const std::string& source, const std::string& destination, const bool is_recursive )
   {
      if( is_directory( source ) )
      {
         if( is_exists( destination ) && !is_directory( destination ) )
            return false;
         return copy_directory( source, destination, is_recursive );
      }
      else if( is_regular_file( source ) )
      {
         if( is_exists( destination ) )
         {
            if( is_directory( destination ) )
            {
               std::string full_destination = destination;
               add_path_leaf( full_destination, get_path_leaf( source ) );
               return copy_file( source, full_destination );
            }
            else
            {
               return false;
            }
         }
         return copy_file( source, destination );
      }

      return false;
   }

   bool remove( const std::string& path )
   {
      bool remove_result = true;

      if( true == is_directory( path ) )
      {
         DIR* dir = filesystem::opendir( path.c_str( ) );
         if( nullptr == dir )
            return false;

         struct dirent* entry;
         while( nullptr != ( entry = filesystem::readdir( dir ) ) )
         {
            if( strcmp( entry->d_name, "." ) && strcmp( entry->d_name, ".." ) )
               remove_result &= remove( path + separator + std::string{ entry->d_name } );
         }

         filesystem::closedir( dir );
         remove_result &= filesystem::rmdir( path.c_str( ) );
      }
      else
      {
         remove_result &= filesystem::unlink( path.c_str( ) );
      }

      return remove_result;
   }

   bool rename( const std::string& source, const std::string& destination )
   {
      return filesystem::rename( source.c_str( ), destination.c_str( ) );
   }

} // namespace carpc::os::os_linux::filesystem
