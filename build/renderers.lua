project "rend_gl"
   kind "SharedLib"
   language "C"
   targetdir "../out"

   files {
      "../rend/gl/**.c", "../rend/gl/**.h",
   }
   includedirs {
      "../", "../engine",
      "../3rdparty/cglm",
      "../3rdparty/glad3.3",
      "../3rdparty/SDL3-3.2.10/include",
   }
   defines { "_CRT_SECURE_NO_WARNINGS" }

   if os.host() == "windows" then
      filter { "system:windows" }
         local arch_str = nil
         if os.hostarch() == "x86_64" then
            arch_str = "x64"
         elseif os.hostarch() == "x86" then
            arch_str = "x86"
         elseif os.hostarch() == "AARCH64" then
            arch_str = "arm64"
         end
         libdirs { "../3rdparty/SDL3-3.2.10/lib/" .. arch_str }
      filter {}
   end

   links { "SDL3" }

   filter { "configurations:Debug" }
      defines { "DEBUG" }
      symbols "On"
   filter { "configurations:Release" }
      defines { "NDEBUG" }
      optimize "On"
   filter { }

project "rend_soft"
   kind "SharedLib"
   language "C"
   targetdir "../out"

   files {
      "../rend/soft/**.c", "../rend/soft/**.h",
   }
   includedirs {
      "../", "../engine",
      "../3rdparty/cglm",
      "../3rdparty/glad3.3",
      "../3rdparty/SDL3-3.2.10/include",
   }
   defines { "_CRT_SECURE_NO_WARNINGS" }

   if os.host() == "windows" then
      filter { "system:windows" }
         local arch_str = nil
         if os.hostarch() == "x86_64" then
         arch_str = "x64"
         elseif os.hostarch() == "x86" then
            arch_str = "x86"
         elseif os.hostarch() == "AARCH64" then
            arch_str = "arm64"
         end
         libdirs { "../3rdparty/SDL3-3.2.10/lib/" .. arch_str }
      filter {}
   end

   links { "SDL3" }

   filter { "configurations:Debug" }
      defines { "DEBUG" }
      symbols "On"
   filter { "configurations:Release" }
      defines { "NDEBUG" }
      optimize "On"
   filter { }
