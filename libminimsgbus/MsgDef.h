
#ifdef _MSC_VER
#define FBC_EXPORT
#ifdef FBC_STATIC
#define FBC_API
#elif defined FBC_EXPORT
#define FBC_API __declspec(dllexport)
#else
#define FBC_API __declspec(dllimport)
#endif
#else
#define FBC_API
#endif