project "game"
   kind "SharedLib"
   language "C"
   targetdir "../out"

   filter { "architecture:x32" }
      targetname "gamex86"
   filter { "architecture:x64" }
      targetname "gamex64"
   filter { }

   files {
      "../game/**.c", "../game/**.h",
      "../cgame/**.c", "../cgame/**.h",
   }
   includedirs {
      "../", "../engine", "../game",
      "../3rdparty/cglm",
      "../3rdparty/glad3.3",
      "../3rdparty/SDL3-3.2.10/include",
   }
   defines { "_CRT_SECURE_NO_WARNINGS" }

   filter { "configurations:Debug" }
      defines { "DEBUG" }
      symbols "On"
   filter { "configurations:Release" }
      defines { "NDEBUG" }
      optimize "On"
   filter { }
