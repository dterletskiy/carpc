#pragma once

#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

#include <string>
#include <set>



namespace carpc::os::os_linux::filesystem {

   extern thread_local int error;

   bool mkdir( const char* const path, const mode_t mode = S_IRWXU | S_IRWXG | S_IRWXO );
   bool mkdirat( const int dirfd, const char* const path, const mode_t mode );
   bool rmdir( const char* const path );
   bool chdir( const char* const path );
   bool fchdir( const int dirfd );

   int open( const char* const path, const int flags, const mode_t mode = S_IRWXU | S_IRWXG | S_IRWXO );
   int openat( const int dirfd, const char* const path, const int flags, const mode_t mode = S_IRWXU | S_IRWXG | S_IRWXO );
   bool close( const int fd );
   bool unlink( const char* const path );
   bool rename( const char* const oldpath, const char* const newpath );
   bool renameat( const int olddirfd, const char* const oldpath, const int newdirfd, const char* const newpath );
   bool renameat2( const int olddirfd, const char* const oldpath, const int newdirfd, const char* const newpath, const unsigned int flags );
   ssize_t sendfile( const int source, const int destination, off_t* const offset, const std::size_t count );
   ssize_t copy_file_range( const int fd_in, off64_t* const off_in,
                            const int fd_out, off64_t* const off_out,
                            const std::size_t len, const unsigned int flags
   );
   ssize_t read( const int fd, void* const buffer, const std::size_t count );
   ssize_t write( const int fd, void* const buffer, const std::size_t count );
   int fgetc( FILE* const stream );
   int getc( FILE* const stream );
   int getchar( );
   char* fgets( char* const s, const int n, FILE* const stream );
   int fputc(  const int c, FILE* const stream );
   int putc(  const int c, FILE* const stream );
   int putchar(  const int c );
   int fputs( char* const s, FILE* const stream );
   off_t lseek( const int fd, const off_t offset, const int whence );

} // namespace carpc::os::os_linux::filesystem

namespace carpc::os::os_linux::filesystem {

   DIR* opendir( const char* const path );
   bool closedir( DIR* const dir );
   dirent* readdir( DIR* const dir );

   FILE* fopen( const char* const path, const char* const mode );
   bool fclose( FILE* const stream );
   std::size_t fread( void* const buffer, std::size_t size, std::size_t nmemb, FILE* const stream );
   std::size_t fwrite( void* const buffer, std::size_t size, std::size_t nmemb, FILE* const stream );

   void clearerr( FILE* const stream );
   int feof( FILE* const stream );
   int ferror( FILE* const stream );
   int fileno( FILE* const stream );

} // namespace carpc::os::os_linux::filesystem

namespace carpc::os::os_linux::filesystem {

   std::string& canonify( std::string& path );
   std::string get_path_leaf( const std::string& path );
   std::string& remove_path_leaf( std::string& path );
   std::string& add_path_leaf( std::string& path, const std::string& leaf );
   std::string& rename_path_leaf( std::string& path, const std::string& rename_with );
   std::string get_parent_path( const std::string& path );

   bool is_regular_file( const std::string& path );
   bool is_directory( const std::string& path );
   bool is_root( const std::string& path );
   bool is_exists( const std::string& path );
   bool is_directory_empty( const std::string& path );

   bool create_directory( const std::string& path, const mode_t mode = S_IRWXU | S_IRWXG | S_IRWXO, const bool is_recursive = false );
   bool copy_file( const std::string& source, const std::string& destination, mode_t mode = 0, const bool use_sendfile = true );
   bool fcopy_file( const std::string& source, const std::string& destination );
   bool copy_directory( const std::string& source, const std::string& destination, const bool is_recursive = false );
   bool copy( const std::string& source, const std::string& destination, const bool is_recursive = false );
   bool remove( const std::string& path );
   bool rename( const std::string& source, const std::string& destination );

} // namespace carpc::os::os_linux::filesystem





#if 0 // Example: copy file performance

   #include "carpc/tools/Performance.hpp"

   const char* const FILE_SOURCE = "/home/scorpion/Desktop/resources/random.txt";
   const char* const FILE_DESTINATION = "/home/scorpion/Desktop/resources/destination.txt";

   int main( int argc, char** argv, char** envp )
   {
      {
         carpc::tools::Performance performance( "system sendfile" );
         performance.start( );
         carpc::os::os_linux::filesystem::copy_file( FILE_SOURCE, FILE_DESTINATION, 0, true );
         performance.stop( );
         carpc::os::os_linux::filesystem::remove( FILE_DESTINATION );
      }

      {
         carpc::tools::Performance performance( "system copy" );
         performance.start( );
         carpc::os::os_linux::filesystem::copy_file( FILE_SOURCE, FILE_DESTINATION, 0, false );
         performance.stop( );
         carpc::os::os_linux::filesystem::remove( FILE_DESTINATION );
      }

      {
         carpc::tools::Performance performance( "library fcopy" );
         performance.start( );
         carpc::os::os_linux::filesystem::fcopy_file( FILE_SOURCE, FILE_DESTINATION );
         performance.stop( );
         carpc::os::os_linux::filesystem::remove( FILE_DESTINATION );
      }

      return 0;
   }

#endif
