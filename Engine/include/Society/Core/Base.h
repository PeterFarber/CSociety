#ifndef SC_CORE_H
#define SC_CORE_H

#if defined(_WIN32)
#if defined(EXPORTING)
#define DECLSPEC __declspec(dllexport)
#else
#define DECLSPEC __declspec(dllimport)
#endif
#else // non windows
#define DECLSPEC
#endif


#endif // !SC_CORE_H
