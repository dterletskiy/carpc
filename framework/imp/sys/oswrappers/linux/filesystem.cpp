#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>
#include <sys/sendfile.h>
#include <cwctype>

#include "api/sys/oswrappers/linux/filesystem.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "Filesystem"



namespace base::os::os_linux::filesystem {

   thread_local int error = 0;

   const char c_separator = '/';
   const std::string separator{ c_separator };

   void canonify( std::string& path )
   {
      std::size_t pos;
      while( std::string::npos != ( pos = path.find( "\\" ) ) )
         path.replace( pos, 1, separator );

      if( c_separator == path.back( ) )
         path.pop_back( );
   }

   std::string get_path_leaf( const std::string& path )
   {
      std::string result;
      const std::size_t sep = path.find_last_of( separator );
      if ( std::string::npos != sep )
         result = path.substr( sep + 1, path.size( ) - sep - 1 );

      return result;
   }

   void remove_leaf( std::string& path )
   {
      const std::size_t sep = path.find_last_of( separator );
      if ( std::string::npos != sep )
         path = path.substr( 0, sep );
   }

   void add_leaf( std::string& path, const std::string& leaf )
   {
      path = path + separator + leaf;
      canonify( path );
   }

   std::string get_parent_path( const std::string& path )
   {
      std::string result = path;
      remove_leaf( result );

      return result;
   }

   std::string& rename_leaf( std::string& path, const std::string& rename_with )
   {
      canonify( path );
      remove_leaf( path );
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
         case 1:
         {
            return c_separator == path[0];
         }
         case 2:
         {
            return std::iswalpha( path[0] ) && ( ':' == path[1] );
         }
         case 3:
         {
            return std::iswalpha( path[0] ) && ( ':' == path[1] ) && ( c_separator == path[2] );
         }
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

      DIR* dir = ::opendir( path.c_str( ) );
      if( nullptr == dir )
         return false;

      std::size_t count = 0;
      dirent* entry;
      while( nullptr != ( entry = ::readdir( dir ) ) )
         if( 2 < ++count )
            break;

      ::closedir( dir );

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

      const int result = ::mkdir( path.c_str( ), mode );
      error = errno;
      if( 0 != result && EEXIST != error )
      {
         SYS_ERR( "mkdir( ) error: %d", error );
      }

      return ( 0 == result || EEXIST == error );
   }

   bool copy_file( const std::string& source, const std::string& destination, mode_t mode )
   {
      SYS_VRB( "source: %s / destination: %s", source.c_str( ), destination.c_str( ) );

      if( false == is_regular_file( source ) )
         return false;

      if( is_exists( destination ) )
         return false;

      int fd_source = ::open( source.c_str( ), O_RDONLY, 0 );
      error = errno;
      if( -1 == fd_source )
      {
         SYS_ERR( "open( ) error: %d", error );
         return false;
      }

      struct stat stat_source;
      ::fstat( fd_source, &stat_source );

      if( 0 == mode )
         mode = stat_source.st_mode;

      int fd_destination = ::open( destination.c_str( ), O_WRONLY | O_CREAT /*| O_TRUNC/**/, mode );
      error = errno;
      if( -1 == fd_destination )
      {
         SYS_ERR( "open( ) error: %d", error );
         ::close( fd_source );
         return false;
      }

      ssize_t transfered_bytes = ::sendfile( fd_destination, fd_source, 0, stat_source.st_size );

      ::close( fd_source );
      ::close( fd_destination );

      return transfered_bytes == (ssize_t)stat_source.st_size;
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

      DIR* dir = ::opendir( source.c_str( ) );
      error = errno;
      if( nullptr == dir )
      {
         SYS_ERR( "opendir( ) error: %d", error );
         return false;
      }

      bool result = true;
      struct dirent* entry;
      while( nullptr != ( entry = ::readdir( dir ) ) )
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

      ::closedir( dir );

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
               add_leaf( full_destination, get_path_leaf( source ) );
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
         DIR* dir = ::opendir( path.c_str( ) );
         if( nullptr == dir )
            return false;

         struct dirent* entry;
         while( nullptr != ( entry = ::readdir( dir ) ) )
         {
            if( strcmp( entry->d_name, "." ) && strcmp( entry->d_name, ".." ) )
               remove_result &= remove( path + separator + std::string{ entry->d_name } );
         }

         ::closedir( dir );

         int result = ::rmdir( path.c_str( ) );
         error = errno;
         if( 0 != result )
         {
            SYS_ERR( "unlink( ) error: %d", error );
            remove_result = false;
         }
      }
      else
      {
         int result = ::unlink( path.c_str( ) );
         error = errno;
         if( 0 != result )
         {
            SYS_ERR( "unlink( ) error: %d", error );
            remove_result = false;
         }
      }

      return remove_result;
   }

   bool rename( const std::string& source, const std::string& destination )
   {
      int result = ::rename( source.c_str( ), destination.c_str( ) );
      error = errno;
      if( 0 != result )
      {
         SYS_ERR( "rename( ) error: %d", error );
      }

      return 0 == result;
   }

} // namespace base::os::os_linux::filesystem
