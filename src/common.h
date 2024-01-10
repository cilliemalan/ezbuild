#include <filesystem>

#define EZBUILD_PROJ_FILE "ezbuild.proj"
#define EZBUILD_FILE ".ezbuild"

// if (x) return x
#define _R(x) do { int __r = (int)(x); if (__r) return __r; } while (0)
// if (!x) return -1;
#define _RN(x) _R((x) ? 0 : -1);
// if (!x) return -1;
#define _RB(x) _R((x) ? 0 : -1);

// if (!x) { result = 1; goto done; }
#define _N(x) do { if (!(x)) { result = 1; goto done; } } while (0)
// result = (x); if (x) goto done;
#define _F(x) do { result = (x); if (result) goto done; } while (0)
// result = (x) ? 0 : 1; if (result) goto done;
#define _B(x) _F((x) ? 0 : 1)
