#pragma once

#ifdef AC_ENABLE_DEBUG_LOG

  #include <iostream>

  #ifndef DLOG
    #define DLOG(x) { std::cout << x << std::endl; }
  #endif

#else

  #ifndef DLOG
    #define DLOG(x)
  #endif

#endif