project "bmvm"
   kind "StaticLib"
   language "C"
   targetdir "../out"

   files {
      "../bmvm/vm/*.c", "../bmvm/vm/*.h",
      "../bmvm/asm/*.c", "../bmvm/asm/*.h",
      "../bmvm/ld/*.c", "../bmvm/ld/*.h",
   }
   removefiles {
      "../bmvm/vm/*-main.c", "../bmvm/vm/main.c",
      "../bmvm/asm/*-main.c", "../bmvm/asm/main.c",
      "../bmvm/ld/*-main.c", "../bmvm/ld/main.c",
   }
   includedirs {
      "../bmvm/vm", "../bmvm/asm", "../bmvm/ld",
   }
   defines { "_CRT_SECURE_NO_WARNINGS" }

project "bmasm"
   kind "ConsoleApp"
   language "C"
   targetdir "../out"

   dependson { "bmvm" }
   links { "bmvm" }

   files {
      "../bmvm/asm/asm-main.c",
   }
   includedirs {
      "../bmvm/vm",
   }
   defines { "_CRT_SECURE_NO_WARNINGS" }
