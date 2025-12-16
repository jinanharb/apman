#ifndef HEAD
#error "HEAD is undefined!"
#endif

#ifndef VERS
#error "VERS is undefined!"
#endif

#ifndef LINK
#error "LINK is undefined!"
#endif

#include <libellul/generics.h>

#include <unistd.h> /* getcwd */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define STRINGIFY( x ) #x
#define STR( x ) STRINGIFY( x )

static void print_help( char *progname ) {
  fprintf( stderr, "Usage: %s --flags|link [--local] [--debug]\n", progname );
  fprintf( stderr, " --flags   \t Compilation flags\n" );
  fprintf( stderr, " --link    \t Linking flags\n" );
  fprintf( stderr, " --local   \t Local compilation (default: system-wide)\n" );
  fprintf( stderr, " --debug   \t Use debug version (default: release)\n" );
  fprintf( stderr, " --version \t Print version\n" );
}

#define GETARG( flag ) for ( int arg = 1 ; arg < argc ; arg++ )         \
    if ( !strcmp( argv[ arg ], STR( -- ) STR( flag ) ) ) { flag = 1; break; }


int main ( int argc, char *argv[] ) {

  int flags = 0 ,  link = 0;
  int local = 0 , debug = 0;
  int vers  = 0;

  GETARG( vers );

  if ( vers ) {
    printf( "You are currently enjoying lib%s v.%s.\n", STR( HEAD ), STR( VERS ) );
    exit( EXIT_SUCCESS );
  }

  GETARG( flags );
  GETARG( link  );
  GETARG( local );
  GETARG( debug );

  /* fprintf( stderr, "OPTS: %s%s%s%s%s\n", flags ? "flags " : "" , link ? "link " : "" , local ? "local " : "" , debug ? "debug " : "", vers ? "version " : "" ); */

  if ( !flags && !link ) {
    fprintf( stderr, "Missing mandatory option --flags or --link.\n" );
    print_help( argv[ 0 ] );
    exit( EXIT_FAILURE );
  }

  if ( flags ) {
    if ( local ) printf( "-Ilib%s/include ", STR( HEAD ) );
  }

  if ( link ) {
    char *cwd = getcwd( NULL, 0 );
#if defined( __APPLE__ )
    if ( local ) printf( "-L%s -rpath %s ", cwd, cwd );
#else
    if ( local ) printf( "-L%s -Wl,-rpath=%s ", cwd, cwd );
#endif
    else {
      printf( "-l%s%s ", STR( HEAD ), debug ? "-debug" : "" );
      printf( "%s", STR( LINK ) );
    }

    free( cwd );
  }

  exit( EXIT_SUCCESS );
}

#undef STRINFIGY
#undef STR
#undef GETARG
