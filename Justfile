# crgui - build
#
# Windows recipes run in PowerShell, Linux recipes run in bash.
# The default compiler is gcc; override it with `just build cc=...`.
#
#     just              # build the shared library (default)
#     just build
#     just run          # build and run the demo executable
#     just clean
#     just check        # show the toolchain being used
#
# Defaults can be overridden on the command line:
#
#     just build cc=clang
#     just build build_dir=out

# `os_family()` comparisons are still unstable in current just releases.
set unstable
set lists

set shell := ["bash", "-uc"]
set windows-shell := ["powershell.exe", "-NoLogo", "-Command"]

cc := env_var_or_default("CC", "gcc")
build_dir := "build"
lib_name := "crgui"

# Sources that make up the shared library.
lib_sources := if os_family() == "windows" { "platforms/windows/windowing.c" } else { "platforms/X11/windowing.c" }

cflags := "-O2 -g -Wall -Wextra -Wpedantic -fPIC"
includes := "-I."

lib_file := if os_family() == "windows" { lib_name + ".dll" } else { "lib" + lib_name + ".so" }
lib_path := build_dir + "/" + lib_file
implib := build_dir + "/lib" + lib_name + ".dll.a"

shared_flags := if os_family() == "windows" { "-shared -static-libgcc \"-Wl,--export-all-symbols\" \"-Wl,--out-implib," + implib + "\"" } else { "-shared" }

system_libs := if os_family() == "windows" { "-luser32 -lgdi32 -lkernel32" } else { "" }

link_flags := "-L" + build_dir + " -l" + lib_name

demo_ext := if os_family() == "windows" { ".exe" } else { "" }
demo_path := build_dir + "/demo" + demo_ext

# Build the shared library (default recipe).
default: build

# Build the shared library and, on Windows, its import library.
build:
    {{ if os_family() == "windows" { "New-Item -ItemType Directory -Force -Path " + build_dir + " | Out-Null" } else { "mkdir -p " + build_dir } }}
    {{ cc }} {{ cflags }} {{ includes }} {{ shared_flags }} -o "{{ lib_path }}" {{ lib_sources }} {{ system_libs }}
    echo "built {{ lib_path }}"

# Build the demo executable from main.c and run it.
run: demo
    {{ if os_family() == "windows" { "& \"./" + demo_path + "\"" } else { "LD_LIBRARY_PATH=" + build_dir + " \"./" + demo_path + "\"" } }}

# Build the demo executable against the shared library.
demo: build
    {{ if os_family() == "windows" { "New-Item -ItemType Directory -Force -Path " + build_dir + " | Out-Null" } else { "mkdir -p " + build_dir } }}
    {{ cc }} {{ cflags }} {{ includes }} -o "{{ demo_path }}" main.c {{ link_flags }} {{ system_libs }}
    @echo "built {{ demo_path }}"

# Remove all build artifacts.
clean:
    {{ if os_family() == "windows" { "if (Test-Path " + build_dir + ") { Remove-Item -Recurse -Force " + build_dir + " }" } else { "rm -rf " + build_dir } }}

# Rebuild the shared library from scratch.
rebuild: clean build

# Show the compiler that will be used.
check:
    {{ if os_family() == "windows" { "Get-Command " + cc + " | Select-Object -ExpandProperty Source" } else { "command -v " + cc } }}
    {{ cc }} --version
