// GVCORE_EXPORT macro
#if defined(_WIN32) && !defined(GV_STATIC_LINKING)
#ifdef GVCORE_EXPORTS
#define GVCORE_EXPORT __declspec(dllexport)
#else
#define GVCORE_EXPORT __declspec(dllimport)
#endif
#else
#define GVCORE_EXPORT
#endif