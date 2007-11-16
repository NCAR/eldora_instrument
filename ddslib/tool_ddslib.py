#
# Rules to build ddslib and export it as a SCons tool
#
tools = ['opendds', 'doxygen']
env = Environment(tools = ['default'] + tools)

def ddslib(env):
    env.AppendLibrary('EldoraDds')
    env.AppendDoxref('EldoraDds')
    env.AppendUnique(CPPPATH = ['#/ddslib',])
    env.Require(tools)

Export('ddslib')

libEldoraDds, sources, headers = env.DdsLibrary('EldoraDds.idl', env)

html = env.Apidocs(sources + headers, DOXYFILE_FILE = "#/Doxyfile")

Default(libEldoraDds)
