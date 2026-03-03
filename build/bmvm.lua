project "bmvm"
   kind "StaticLib"
   language "C"
   targetdir "../out"

   files {
      "../bmvm/vm/*.c", "../bmvm/vm/*.h",
   }
   removefiles {
      "../bmvm/vm/*-main.c", "../bmvm/vm/main.c"
   }
   includedirs {
      "../bmvm/vm",
   }
   defines { "_CRT_SECURE_NO_WARNINGS" }

project "bmasm"
   kind "ConsoleApp"
   language "C"
   targetdir "../out"

   dependson { "bmvm" }
   links { "bmvm" }

   files {
      "../bmvm/vm/asm-main.c",
   }
   includedirs {
      "../bmvm/vm",
   }
   defines { "_CRT_SECURE_NO_WARNINGS" }
