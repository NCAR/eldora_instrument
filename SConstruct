import os
import eol_scons

def Eldora(env):
    # define ELDORA_ROOT as a double-quote-delimited string containing the 
    # top-level directory of the Eldora build.  E.g., this will add something
    # like -DELDORA_ROOT='"/code/burghart/eldora"' to each C/C++ compilation.
    env.AppendUnique(CPPDEFINES=['ELDORA_ROOT=' + 
                                 '\'"' + env.Dir('#').get_abspath() + '"\''])
    if env['CC'] == "gcc":
        env.AppendUnique(_LIBFLAGS=['-lpthread'])
        env.AppendUnique(CCFLAGS=['-fmessage-length=0'])
    else:
        env.AppendUnique(CCFLAGS=['/EHsc','/MDd','/GR','/GX'])
        env.PassEnv(r'VC.*')
        env.PassEnv(r'VS.*')
        env.PassEnv(r'DevEnvDir')
        env.PassEnv(r'Framework.*')

tools = ['sdrscope']
env = Environment(tools = ['default'] + tools, GLOBAL_TOOLS = [Eldora])

Export('env')

# Build the main eldora applications:
SConscript('eldorappi/SConscript')
SConscript('eldoraprod/SConscript')
SConscript('eldorascope/SConscript')
SConscript('eldoragui/SConscript')
SConscript('dorade/tool_dorade.py')

# Only build eldoradrx if the DSP21KSF environment variable is set.  This
# allows for still building everythin else on systems without the bittware 
# development system installed.
if (os.environ.has_key('DSP21KSF')):
    SConscript('eldoradrx/SConscript')
else:
    print
    print "No eldoradrx build: environment variable 'DSP21KSF' is not set"
    print    

# The qttoobox support
SConscript('qttoolbox/SConscript')

# Test apps for the utilities
SConscript('utilities/test/SConscript')

# A DDS test harness
SConscript('testdds/SConscript')

# ECB
SConscript('ecb/SConscript')

# Tom's original code
SConscript('tmp/SConscript')

options = env.GlobalOptions()
options.Update(env)
Help(options.GenerateHelpText(env))

