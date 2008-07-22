#
# Rules to build ddsqt and export it as a SCons tool
#
import os

tools = ['ddssvc', 'qt4','utilities']
env = Environment(tools = ['default'] + tools)

qt4modules = ['QtCore',]
env.EnableQt4Modules(qt4modules)

#tooldir = env.Dir('.')

libsources = Split("""
EldoraQtSource.cpp
EldoraQtTSSource.cpp
EldoraQtProductsSource.cpp
""")

libddsqt = env.Library('ddsqt', libsources)

Default(libddsqt)

def ddsqt(env):
    # The ddsqt library
    env.AppendUnique(CPPPATH = ['#/ddsqt',])
    env.AppendLibrary('ddsqt')
    
    tools = ['ddssvc']
    env.Require(tools)

Export('ddsqt')


