project "bm"
   kind "ConsoleApp"
   language "C"
   targetdir "../out"

   files { "../engine/**.c", "../engine/**.h" }

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
      os.copyfile( "../3rdparty/SDL3-3.2.10/lib/" .. arch_str .. "/SDL3.dll", "../bin/SDL3.dll" )
   end

   links { "SDL3" }

   filter { "configurations:Debug" }
      defines { "DEBUG" }
      symbols "On"
   filter { "configurations:Release" }
      defines { "NDEBUG" }
      optimize "On"
   filter { }
