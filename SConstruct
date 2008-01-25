import os
import eol_scons

def Eldora(env):
    # define ELDORA_ROOT as a double-quote-delimited string containing the 
    # top-level directory of the Eldora build.  E.g., this will add something
    # like -DELDORA_ROOT='"/code/burghart/eldora"' to each C/C++ compilation.
    env.AppendUnique(CPPDEFINES=['ELDORA_ROOT=' + 
                                 '\'"' + env.Dir('#').get_abspath() + '"\''])
    if env['CC'] == "gcc":
        env.Append(_LIBFLAGS=['-lpthread'])
        env.Append(CCFLAGS='-fmessage-length=0')
        env.Append(CCFLAGS='-O2')
    else:
        env.Append(CCFLAGS=['/EHsc','/MDd','/GR','/GX'])
        env.PassEnv(r'VC.*')
        env.PassEnv(r'VS.*')
        env.PassEnv(r'DevEnvDir')
        env.PassEnv(r'Framework.*')

tools = ['sdrscope']
env = Environment(tools = ['default'] + tools, GLOBAL_TOOLS = [Eldora])

Export('env')

# The main eldora applications:
SConscript('eldorappi/SConscript')
SConscript('eldoradrx/SConscript')
SConscript('eldoraprod/SConscript')
SConscript('eldorascope/SConscript')

# The qttoobox support
SConscript('qttoolbox/SConscript')

# A DDS test harness
SConscript('testdds/SConscript')

# Tom's original code
SConscript('tmp/SConscript')

options = env.GlobalOptions()
options.Update(env)
Help(options.GenerateHelpText(env))

