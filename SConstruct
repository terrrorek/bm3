import os
import sys
import subprocess
import platform

env = Environment( TARGET_ARCH='x86' )
arch = 'x86'

obj_files = []
game_files = []

gl_files = []
soft_files = []

global_cflags = []
global_ldflags = []
global_libs = []
global_libpath = []
global_includes = ['.']

AddOption(
   '--float-math',
   dest='float-math',
   default=False,
   action='store_true',
   help='don\'t use SSE2 instructions for math'
)
AddOption(
   '--debug-symbols',
   dest='debug',
   default=False,
   action='store_true',
   help='generate debug symbols'
)

if os.name == 'posix' and not env.GetOption( 'float-math' ):
   global_cflags += ['-msse', '-msse2']
elif os.name == 'nt' and not env.GetOption( 'float-math' ):
   global_cflags.append( '/arch:SSE2' )
   global_cflags += ['/D__SSE2__', '/D__SSE2_MATH__']
elif os.name == 'posix' and env.GetOption( 'float-math' ):
   global_cflags.append( '-D__FLOAT_MATH__' )
elif os.name == 'nt' and env.GetOption( 'float-math' ):
   global_cflags.append( '/D__FLOAT_MATH__' )

if os.name == 'posix' and env.GetOption('debug'):
   global_cflags.append( '-g' )
   global_cflags.append( '-D__DEBUG' )
   global_ldflags.append( '-g' )
   # global_cflags.append( '-fsanitize=address' )
elif os.name == 'nt' and env.GetOption('debug'):
   global_cflags.append( '/Zi' )
   global_cflags.append( '/FS' )
   global_cflags.append( '/D__DEBUG' )
   # global_cflags.append( '/fsanitize=address' )
   global_ldflags.append( '/DEBUG' )

if os.name == 'nt':
   global_libs += ['user32', 'ws2_32']
elif os.name == 'posix':
   env['STATIC_AND_SHARED_OBJECTS_ARE_THE_SAME'] = 1
   global_cflags += ['-Wall', '-Wextra', '-pedantic']
   global_ldflags.append( '-m32' )

class Subproject:
   folder: str
   extension: str

   out: list
   cflags: list = []
   cdefines: list = []
   include: list = ['.']

   def build(self, cf, inc, lib, libpath):
      print(f'====== BEGIN {self.folder} ======')
      if not os.path.exists( self.folder ):
         os.makedirs( self.folder )

      path = f'{self.folder}/{self.extension}'
      source = env.Glob( path )
      env.VariantDir( f'objects/{self.folder}', self.folder )
      var_source = [ f'objects/{f}' for f in source ]

      self.out.append(
         env.Object(
            var_source,
            CDEFINES = self.cdefines,
            CPPPATH = self.include + inc,
            # CFLAGS = self.cflags + cf,
            CPPFLAGS = self.cflags + cf,
         )
      )
      print(f'====== END {self.folder} ======')

   def __init__(self, folder, extension, out,
         cflags: list = [], cdefines: list = [], include: list = ['.']):
      self.folder = folder
      self.extension = extension
      self.cflags = cflags
      self.cdefines = cdefines
      self.include = include
      self.out = out

class Library:
   path: str
   libs: list = []
   includes: list = []
   libpath: list = []

   def build(self, cf, inc, lib, libpath):
      print(f'====== BEGIN {self.path} ======')
      inc += self.includes
      lib += self.libs
      libpath += self.libpath
      print(f'====== END {self.path} ======')
      pass

   def __init__(self, path, libs, libpath, includes):
      self.path = path
      self.libs = libs
      self.includes = includes
      self.libpath = libpath

class CompileLibrary(Subproject):
   pass

def update_includes(folder):
   for file in os.listdir( folder ):
      if os.path.isdir( file ) and \
         file != 'objects' and \
         file != 'out' and \
         file != '3rdparty' and \
         file != 'game' and \
         file != 'rend' and \
         file != 'tools':
            global_includes.append( f'{folder}/{file}' )

update_includes( '.' )
dll_cflags = ['-fPIC'] if os.name == 'posix' else []
TASKS = [
   Library('3rdparty/SDL3', ['SDL3'],
      [f'3rdparty/SDL3/lib/{arch}'],
      [f'3rdparty/SDL3/include/SDL3',
       f'3rdparty/SDL3/include']),
   Library('3rdparty/cglm', [], [], [f'3rdparty/cglm/include']),

   CompileLibrary('3rdparty/blake2', '*.c', obj_files),

   Subproject('common', '*.c', obj_files),

   Subproject('filesys', '*.c', obj_files),

   Subproject('engine', '*.c', obj_files),
   Subproject('engine/video', '*.c', obj_files),
   Subproject('engine/game', '*.c', obj_files),
   Subproject('engine/host', '*.c', obj_files),
   Subproject('engine/client', '*.c', obj_files),
   Subproject('engine/common', '*.c', obj_files),
   Subproject('engine/net', '*.c', obj_files),

   Subproject('rend/gl', '*.c', gl_files, dll_cflags),
   Subproject('rend/soft', '*.c', soft_files, dll_cflags),

   Subproject('game', '*.c', obj_files),
]

for task in TASKS:
   task.build( global_cflags, global_includes, \
               global_libs, global_libpath )

if os.name == 'posix':
   global_libs.append( 'dl' )

env.Program(
   'out/bm',
   obj_files,
   LIBS=global_libs,
   LIBPATH=global_libpath,
   LINKFLAGS=global_ldflags,
)

env.SharedLibrary(
   'out/rend_soft',
   soft_files,
   LIBS=global_libs,
   LIBPATH=global_libpath,
   LINKFLAGS=global_ldflags,
)

env.SharedLibrary(
   'out/rend_gl',
   gl_files,
   LIBS=global_libs,
   LIBPATH=global_libpath,
   LINKFLAGS=global_ldflags,
)
