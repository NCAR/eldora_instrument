import eol_scons

def Eldora(env):
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
