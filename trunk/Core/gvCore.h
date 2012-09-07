  #ifdef GVCore_EXPORTS
    #define GVCORE_EXPORT __declspec(dllexport)
  #else
    #define GVCORE_EXPORT __declspec(dllimport)
  #endif
