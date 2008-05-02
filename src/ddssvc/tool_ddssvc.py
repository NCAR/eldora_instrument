#
# Rules to build ddssvc and export it as a SCons tool
#
import os

tools = ['ddslib']
env = Environment(tools = ['default'] + tools)

def ddssvc(env):
    env.AppendLibrary('ddssvc')
    env.AppendUnique(CPPPATH = ['#/ddssvc',])
    env.Require(tools)

Export('ddssvc')


# sources
sources = Split("""
DDSPublisher.cpp
DDSWriter.cpp
DDSSubscriber.cpp
DDSReader.cpp
""")

subscriber = env.Library('ddssvc', sources)

Default(subscriber)