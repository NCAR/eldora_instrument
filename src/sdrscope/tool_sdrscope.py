tools = ['qtt_scopeplot', 'fftw']
env = Environment(tools = ['default'] + tools)

# This will create ui_SdrScope.h
env.Uic4(['SdrScope.ui',])

sources = Split("""
SdrScope.cpp
""")
sdrscopelib = env.Library('sdrscope', sources)

Default(sdrscopelib)

# The following function and Export actually provide the sdrscope tool
def sdrscope(env):
    env.Require(tools)
    env.AppendUnique(CPPPATH = ['#/sdrscope'])
    env.AppendUnique(LIBPATH = ['#/sdrscope'])
    env.Append(LIBS = ['sdrscope'])

Export('sdrscope')
