OUTPUT_FORMAT(pei-x86-64)
SEARCH_DIR("/usr/x86_64-w64-mingw32/lib");

PAGE_SIZE = __section_alignment__;

SECTIONS
{
  . = SIZEOF_HEADERS;
  . = ALIGN(PAGE_SIZE);

  .text __image_base__ + PAGE_SIZE :
  {
    KEEP(*(.text$start))
    *(SORT_BY_ALIGNMENT(.text*))
  }

  .data BLOCK(PAGE_SIZE) :
  {
    *(SORT_BY_ALIGNMENT(.rdata*))

    *(SORT_BY_ALIGNMENT(.data*))

    *(SORT_BY_ALIGNMENT(.bss*))
    *(COMMON)
  }

  .idata BLOCK(PAGE_SIZE) :
  {
    KEEP(SORT(*)(.idata$2))
    KEEP(SORT(*)(.idata$3))

    LONG(0); LONG(0); LONG(0); LONG(0); LONG(0);

    . = ALIGN(8);

    KEEP(SORT(*)(.idata$4))

    __IAT_start__ = .;

    SORT(*)(.idata$5)

    __IAT_end__ = .;

    KEEP(SORT(*)(.idata$6))
    KEEP(SORT(*)(.idata$7))
  }

  /DISCARD/ :
  {
      *(*)
  }
}
