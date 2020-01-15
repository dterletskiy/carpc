#pragma once



/****************************************************************************************************
 * 
 * Macro returns amount of arguments
 * Returns the number of arguments contained in __VA_ARGS__ before expansion as far as NARG is >0 and <70
 * Parameters could be any sign
 * link: https://stackoverflow.com/questions/2124339/c-preprocessor-va-args-number-of-arguments
 * Examples:
 *    std::cout << ARGS_COUNT( 1, 2, 3, a ,"b" ) << std::endl;
 * 
 ***************************************************************************************************/
#ifdef _MSC_VER // Microsoft compilers
   #define INTERNAL_ARGS_AUGMENTER(...) DUMMY, __VA_ARGS__
   #define _GET_ARG_N_( \
          _1_,  _2_,  _3_,  _4_,  _5_,  _6_,  _7_,  _8_,  _9_, _10_, \
         _11_, _12_, _13_, _14_, _15_, _16_, _17_, _18_, _19_, _20_, \
         _21_, _22_, _23_, _24_, _25_, _26_, _27_, _28_, _29_, _30_, \
         _31_, _32_, _33_, _34_, _35_, _36_, _37_, _38_, _39_, _40_, \
         _41_, _42_, _43_, _44_, _45_, _46_, _47_, _48_, _49_, _50_, \
         _51_, _52_, _53_, _54_, _55_, _56_, _57_, _58_, _59_, _60_, \
         _61_, _62_, _63_, _64_, _65_, _66_, _67_, _68_, _69_, _70_, \
         count, ...) count
   #define _REVERSE_SEQUENCE_() \
      69, 68, 67, 66, 65, 64, 63, 62, 61, 60, \
      59, 58, 57, 56, 55, 54, 53, 52, 51, 50, \
      49, 48, 47, 46, 45, 44, 43, 42, 41, 40, \
      39, 38, 37, 36, 35, 34, 33, 32, 31, 30, \
      29, 28, 27, 26, 25, 24, 23, 22, 21, 20, \
      19, 18, 17, 16, 15, 14, 13, 12, 11, 10, \
       9,  8,  7,  6,  5,  4,  3,  2,  1,  0
#else // Non-Microsoft compilers
   #define INTERNAL_ARGS_AUGMENTER(...) DUMMY, ## __VA_ARGS__
   #define _GET_ARG_N_( \
          _0_, \
          _1_,  _2_,  _3_,  _4_,  _5_,  _6_,  _7_,  _8_,  _9_, _10_, \
         _11_, _12_, _13_, _14_, _15_, _16_, _17_, _18_, _19_, _20_, \
         _21_, _22_, _23_, _24_, _25_, _26_, _27_, _28_, _29_, _30_, \
         _31_, _32_, _33_, _34_, _35_, _36_, _37_, _38_, _39_, _40_, \
         _41_, _42_, _43_, _44_, _45_, _46_, _47_, _48_, _49_, _50_, \
         _51_, _52_, _53_, _54_, _55_, _56_, _57_, _58_, _59_, _60_, \
         _61_, _62_, _63_, _64_, _65_, _66_, _67_, _68_, _69_, _70_, \
         count, ...) count
   #define _REVERSE_SEQUENCE_() \
      70, \
      69, 68, 67, 66, 65, 64, 63, 62, 61, 60, \
      59, 58, 57, 56, 55, 54, 53, 52, 51, 50, \
      49, 48, 47, 46, 45, 44, 43, 42, 41, 40, \
      39, 38, 37, 36, 35, 34, 33, 32, 31, 30, \
      29, 28, 27, 26, 25, 24, 23, 22, 21, 20, \
      19, 18, 17, 16, 15, 14, 13, 12, 11, 10, \
       9,  8,  7,  6,  5,  4,  3,  2,  1,  0
#endif // Microsoft compilers
#define INTERNAL_EXPAND(x) x
#define INTERNAL_EXPAND_ARGS_PRIVATE(...) INTERNAL_EXPAND(_GET_ARG_N_(__VA_ARGS__))
#define ARGS_COUNT(...)  INTERNAL_EXPAND_ARGS_PRIVATE(INTERNAL_ARGS_AUGMENTER(__VA_ARGS__), _REVERSE_SEQUENCE_())



/****************************************************************************************************
 * 
 * Macro returns amount of arguments
 * Returns the number of arguments contained in __VA_ARGS__ before expansion as far as NARG is >0 and <70
 * Parameters could be only compilable sign such as variable, digit, char
 * link: https://stackoverflow.com/questions/2124339/c-preprocessor-va-args-number-of-arguments
 * Examples:
 *    std::cout << ARGS_COUNT_C( 1, 2, 3, "b" ) << std::endl;
 * 
 ***************************************************************************************************/
#define ARGS_COUNT_C(...) detail::args_count(__VA_ARGS__)
namespace detail
{
   template< typename ...Args >
   constexpr std::size_t args_count( Args&&... ) { return sizeof...(Args); }
}



/****************************************************************************************************
 * 
 * Examples:
 *    C_ASSERT(1 ==  COUNT_ARGS(1));      // OK
 *    C_ASSERT(1 ==  COUNT_ARGS(1, 2));   // ERROR
 * 
 ***************************************************************************************************/
#define C_ASSERT(test) switch(0) { case 0: case test:; }


/****************************************************************************************************
 * 
 * Macro returns first argument
 * link: https://stackoverflow.com/questions/4750688/how-to-single-out-the-first-parameter-sent-to-a-macro-taking-only-a-variadic-par
 * Examples:
 *    std::cout << GET_FIRST_ARG( 1, 2, 3, 4, 5 ) << std::endl;
 * 
 ***************************************************************************************************/
#define __FIRST_ARG__(N, ...) N
#define _FIRST_ARG_(args) __FIRST_ARG__ args
#define GET_FIRST_ARG(...) _FIRST_ARG((__VA_ARGS__, 0))
