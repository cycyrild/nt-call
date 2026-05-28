#!/bin/sh
set -eu

lld_executable=$1
nt_call_archive=$2
shift 2

script_dir=$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)

gnu_executable="${lld_executable%.exe}.ld.exe"
map_file="${lld_executable%.exe}.ld.map"
lld_temp_object="${lld_executable}.lto.obj"

if [ ! -f "$lld_temp_object" ]; then
    echo "No post-LTO COFF object found at '$lld_temp_object'" >&2
    exit 1
fi

x86_64-w64-mingw32-ld \
    -T "$script_dir/gnu-final.ld" \
    --subsystem console \
    -e start \
    --gc-sections \
    --no-insert-timestamp \
    --section-alignment=0x1000 \
    --file-alignment=0x1000 \
    --strip-all \
    -Map="$map_file" \
    "$lld_temp_object" \
    "$nt_call_archive" \
    "$@" \
    -o "$gnu_executable"
