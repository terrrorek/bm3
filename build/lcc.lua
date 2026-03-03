local separator
if os.host() == "windows" then
   separator = "\\"
else
   separator = "/"
end

project "lburg"
   kind "ConsoleApp"
   language "C"
   targetdir "../out"

   files {
      "../3rdparty/lcc/lburg/*.c",
      "../3rdparty/lcc/lburg/*.h",
   }
   includedirs {
      "../3rdparty/lcc/lburg",
      "../3rdparty/lcc/include",
      "../3rdparty/lcc/"
   }

project "lcc_md"
   kind "ConsoleApp"
   language "C"
   targetdir "../out"

   dependson { "lburg" }
   files {
      "../3rdparty/lcc/src/*.md",
   }
   filter { "files:**.md" }
      buildcommands {
         '..' .. separator .. 'out' .. separator ..
            'lburg %[%{file.reldirectory}/%{file.basename}.md] %[../3rdparty/lcc/backends/_%{file.basename}.c]'
      }
      buildoutputs { '../3rdparty/lcc/backends/_%{file.basename}.c' }
   filter {}

project "lcc"
   kind "ConsoleApp"
   language "C"
   targetdir "../out"

   dependson { "lcc_md" }
   files {
      "../3rdparty/lcc/src/*.c",
      "../3rdparty/lcc/src/*.h",
      "../3rdparty/lcc/backends/_*.c"
   }
   includedirs {
      "../3rdparty/lcc/src",
      "../3rdparty/lcc/include",
      "../3rdparty/lcc/"
   }
