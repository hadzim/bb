#ifndef TBSguard_H_
#define TBSguard_H_



#if defined(_WIN32) && (defined(_TBS_DLL) || defined(_MSC_VER))
#define TBS_DLL
#endif

#if defined(_WIN32) && defined(TBS_DLL)
#if defined(_TBS_EXPORTS)
#define TBS_API __declspec(dllexport)
#else
#define TBS_API __declspec(dllimport)
#endif
#endif

#if !defined(TBS_API)
#define TBS_API
#endif

#endif
