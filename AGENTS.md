# AGENTS.md

`crgui` is a small C GUI library. The public API is the `Window` type and `createWindow` / `pollEvents` / `closeWindow` in `window.h`; `crgui.h` is the umbrella header. Only a Win32 backend exists.

## Build and run

Everything goes through `just`; there is no Makefile/CMake. On Windows, recipes use PowerShell and require a `gcc` on PATH (MSYS2/MinGW in this repo).

- `just` or `just build` — build `build/crgui.dll` and import library `build/libcrgui.dll.a` (default recipe)
- `just demo` — build the library, then `build/demo.exe` from `main.c`
- `just run` — build and launch the demo; it blocks until the window is closed, so use `just demo` for non-interactive verification
- `just rebuild` / `just clean`
- `just check` — show which compiler will be used
- Overrides: `just build cc=clang`, `just build build_dir=out`

There is no test, lint, or format target and no CI. Verification is a clean `just rebuild`; the sources build warning-free with `-Wall -Wextra -Wpedantic`. `test.c` and the prebuilt root-level `test.exe` are scratch files that no recipe references.

## Layout

- `platforms/windows/windowing.c` — the only source compiled into the shared library: window class, `CreateWindowExA`, and message pump.
- `main.c` — demo executable, linked against `build/crgui.dll`.
- Root headers contain real implementations and macros, not just declarations: `window.h`, `strings.h`, `dynamic.h`, `mem.h`.
- Files ending in `~` (`main.c~`, `window.h~`, etc.) are stale editor backups. Ignore them; the `.gitignore` covers them.

## Gotchas

- Linux is not supported: `window.h` hits `#error platform not supported`, and the `platforms/linux/windowing.c` referenced by the Justfile does not exist. Do not assume the Linux branch is functional.
- `strings.h` defines `str2cstr` as a non-static function (plus macros). Adding a second `.c` that includes it in the same binary causes duplicate-symbol errors; make header functions `static`/`inline` first. New library sources must also be added to `lib_sources` in the Justfile.
- `String` is a `daMaker(char, String)` dynamic array, not NUL-terminated. Use `str2cstr()` before passing it to Win32, and `.count` / `stringPrint()` for output.
- `createWindow` receives `title` by value and calls `daReset(title)` after creating the window, freeing the shared backing buffer. Treat the caller's `String` as consumed; do not reuse or free it again.
- The Justfile depends on `set unstable` and expression-form `os_family()` checks. Keep that pattern when adding or changing recipes.
