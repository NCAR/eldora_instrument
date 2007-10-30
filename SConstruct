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
    else:
        env.Append(CCFLAGS=['/EHsc','/MDd','/GR','/GX'])
        env.PassEnv(r'VC.*')
        env.PassEnv(r'VS.*')
        env.PassEnv(r'DevEnvDir')
        env.PassEnv(r'Framework.*')

env = Environment(tools = ['default'], GLOBAL_TOOLS = [Eldora])

SConscript('ddslib/SConscript')
SConscript('ddssvc/SConscript')
SConscript('tmp/SConscript')
SConscript('testdds/SConscript')
SConscript('redrapids/SConscript')
