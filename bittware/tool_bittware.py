#
# Rules to build rr314 and export it as a SCons tool
#
import os

def bittware(env):
    #
    # Our bittware library
    env.AppendUnique(CPPPATH = ['#/bittware',])
    env.AppendLibrary('rr314')
    #
    # dsp21k include path
    env.AppendUnique(CPPPATH = ['/usr/local/dsp21ksf/inc',])
    #
    # dsp21ksf library paths:
    dsp21kLibDir = '/usr/local/dsp21ksf/lib'
    env.AppendUnique(LIBPATH = [dsp21kLibDir, '.'])
    #
    # channel adapter lib:
    dsp21kLibs = ['hil','bwregs']
    env.Append(LIBS = dsp21kLibs)
    #
    #
    tools = ['utilities']
    env.Require(tools)

Export('bittware')

# The redrapids and library sources

tools = ['utilities','bittware']
env = Environment(tools = ['default'] + tools)

#redRapidsDevelDir = os.path.join('#', 'redrapids', 
#                                 'DSK-320-002-R05 (CA C API and Sample Code)', 
#                                 'channeladapterlib', 'R05')
#redRapidsDevelLibDir = os.path.join(redRapidsDevelDir, 'linux_x86-32')
#env.AppendUnique(CPPPATH=[redRapidsDevelDir, ])
#env.AppendUnique(CPPDEFINES = ['LINUX', ])

#rrsources = [os.path.join(redRapidsDevelDir, 'ca_functions.c'), 
#             os.path.join(redRapidsDevelDir, 'ca_membuffer.c')]

libsources = Split("""
Bittware.cpp
""")

libbittware = env.Library('bittware', libsources)

Default(libbittware)
