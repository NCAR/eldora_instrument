import os
import string
import re
from chdir import ChdirActions

# 
# A DDS project simply defines a datatype
# that will be handled by DDS. This is
# done in an idl file. A raft of supporting
# code and idl is generated from the original
# idl definition of the datatype.
#
# example usage:
# import os
# import DDSLib
# env = Environment(ENV = os.environ)
# lib = DDSLib.DdsLibrary('EldoraDds.idl', env)
#
# -------------------------------------------
#
# Create a library containing all DDS routines
# required for both server and client
#
# @param idlFile The idl file defining the DDS for a particular type
# #param env 
def DdsLibrary(idlFile, env):
	# get our current relative directory, needed
	# for some later comands which must be executed here.
	curDir = env.Dir('.').get_path(env.Dir('#'))
	#
	#
	# Fetch the ACE, TAO and DDS locations from
	# the environment
	#
	TAO_ROOT = os.environ['TAO_ROOT']
	ACE_ROOT = os.environ['ACE_ROOT']
	DDS_ROOT = os.environ['DDS_ROOT']
	#
	# ------------------------------------------
	# Add include specifications for ACE, TAO and DDS.
	#
	env.Append(CPPFLAGS=' -I'+ACE_ROOT)
	env.Append(CPPFLAGS=' -I'+TAO_ROOT)
	env.Append(CPPFLAGS=' -I'+DDS_ROOT)
	#
	ddsTypes = ddsTypeScanner(idlFile, env)
	target1 = taoIdlFiles(idlFile)
	target2 = dcpsTsFiles(ddsTypes)
	target3 = taoIdlFiles(target2[2])
	#
	env.Command(target1, idlFile, 'tao_idl -o $SOURCE.dir -Gdcps $SOURCE')
	#
	# Execute the dcp_tl.pl command in the current directory,
	# since dcps_ts.pl will only put its output in the current directory.
	dcpsCmd = ChdirActions(env, ["dcps_ts.pl $SOURCE.file"], curDir)
	env.Command(target2, idlFile, dcpsCmd)
	#
	tao_cmd = ChdirActions(env, ['tao_idl -I ' + DDS_ROOT +
				     ' $SOURCE.file'], curDir)
	typeSupportSource = ddsTypes+'TypeSupport.idl'
	env.Command(target3, typeSupportSource, tao_cmd)
	#
	ddsLib = env.Library('EldoraDds', 
   		taoIdlFiles('EldoraDds.idl')[0]
   		+dcpsTsFiles('Pulse')[0] 
   		+ taoIdlFiles('PulseTypeSupport.idl')[0])
	#
	return ddsLib

# -------- DDS support functions -----------
#
# Two functions are provided here which generate
# all of the file names associated with a
# a DDS project. These are taoIdlFiles() and
# dcpsTsFiles(). These functions create
# all of the filenames that will be created by
# the tao_idl and the dcps_ts.pl processors.
#
# These functions are used by the DsdLibrary()
# function. 
#
# -------------------------------------------
#
# Create the filenames that are produced when
# tao_idl, with the -Gdcps option, processes 
# an idl file
#
# @param idlFile The name of the idl file
# @return A 3 element list. The first slice
# contains a list of .cpp files. The second
# slice contains a list of .h files. The third
# slice contains a list of .inl files.
def taoIdlFiles(idlFile):
	root = idlFile.split('.')[0]
	cppFiles = [root+'C.cpp', root+'S.cpp']
	hFiles   = [root+'C.h', root+'S.h']
	inlFiles = [root+'C.inl', root+'S.inl']
	return [cppFiles, hFiles, inlFiles]

# 
# Create the type support filenames that are 
# produced for a given DDS data type definition 
# within a DDS idl file.
def dcpsTsFiles(ddsType):
	root = ddsType+'TypeSupport'
	cppFiles = [root+'Impl.cpp']
	hFiles   = [root+'Impl.h']
	idlFile  = root+'.idl'
	return [cppFiles, hFiles, idlFile]

# -------------------------------------------
#
#
# Pick out the DDS types defined by the
# #pragma DCPS_DATA_TYPE 
# statements in the idl file. 
#
# Currently only returns the first type defined.
# Need to fix the regular expression to process all
# lines.
#
# @param idlFile The idl file name
# @returns A tupple containing the type names. The type
# names do not include the module namespace qualification
#
def ddsTypeScanner(fileName, env):
# I don't know why this regular expression works to
# find the type name; I was on an airplane when 
# writing it and didn't have access 
# to the docs for regular expressions. It returns a
# tupple containing the modules name and the type name.
# We just return the unqualified type names, i.e. the
# second slice of the tupple.
	dcps_re = re.compile(r'#pragma\s+DCPS_DATA_TYPE\s+"(\S+)::(\S+)*"',re.M)
	node = env.FindFile(fileName, '.')
	contents = node.get_contents()
	dcps_types = dcps_re.findall(contents)
	return dcps_types[0][1]
