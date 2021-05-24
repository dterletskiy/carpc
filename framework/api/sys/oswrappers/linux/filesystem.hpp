#pragma once

#include <sys/types.h>
#include <sys/stat.h>

#include <string>
#include <set>



namespace base::os::os_linux::filesystem {

   extern thread_local int error;

   void canonify( std::string& path );
   std::string get_path_leaf( const std::string& path );
   void remove_leaf( std::string& path );
   void add_leaf( std::string& path, const std::string& leaf );
   std::string get_parent_path( const std::string& path );
   std::string& rename_leaf( std::string& path, const std::string& rename_with );

   bool is_regular_file( const std::string& path );
   bool is_directory( const std::string& path );
   bool is_root( const std::string& path );
   bool is_exists( const std::string& path );
   bool is_directory_empty( const std::string& path );

   bool create_directory( const std::string& path, const mode_t mode = S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH, const bool is_recursive = false );
   bool copy_file( const std::string& source, const std::string& destination, mode_t mode = 0 );
   bool copy_directory( const std::string& source, const std::string& destination, const bool is_recursive = false );
   bool copy( const std::string& source, const std::string& destination, const bool is_recursive = false );
   bool remove( const std::string& path );
   bool rename( const std::string& source, const std::string& destination );

} // namespace base::os::os_linux::filesystem
