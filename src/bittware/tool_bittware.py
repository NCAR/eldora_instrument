#
# Rules to build rr314 and export it as a SCons tool
#
import os
import SCons

if (not os.environ.has_key('DSP21KSF')):
    raise SCons.Errors.StopError, "Environment variable DSP21KSF must be set to use the bittware tool."

def bittware(env):
    #
    # Specify our product: the bittware library
    env.AppendUnique(CPPPATH = ['#/bittware',])
    env.AppendLibrary('bittware')
    #
    # we need the DSP21KSF environment variable
    env['DSP21KSF'] = os.environ['DSP21KSF']
    #
    # dsp21k include path
    dsp21ksfIncPath = os.path.join(env['DSP21KSF'], 'inc')
    env.AppendUnique(CPPPATH = [dsp21ksfIncPath,])
    #
    # dsp21ksf library path
    dsp21ksfLibPath = os.path.join(env['DSP21KSF'], 'lib')
    env.AppendUnique(LIBPATH = [dsp21ksfLibPath,])
    #
    # dsp21ksf libraries
    dsp21kLibs = ['hil', 'bwregs', 'adlibelf']
    env.Append(LIBS = dsp21kLibs)
    #
    #
    tools = ['utilities']
    env.Require(tools)

Export('bittware')

# The redrapids and library sources

tools = ['utilities','bittware']
env = Environment(tools = ['default'] + tools)

libsources = Split("""
Bittware.cpp
""")

libbittware = env.Library('bittware', libsources)

Default(libbittware)
