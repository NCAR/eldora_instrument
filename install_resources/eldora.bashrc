# .bashrc

# Source global definitions
if [ -f /etc/bashrc ]; then
	. /etc/bashrc
fi

# User specific aliases and functions
export ELDORADIR=/opt/eldora
export QTDIR=/usr/lib/qt4
export QT4DIR=/usr/lib/qt4
export QTINC=$QTDIR/include
export QTLIB=$QTDIR/lib

export ACE_ROOT=/opt/local/ACE+TAO-5.6/ACE_wrappers
export TAO_ROOT=$ACE_ROOT/TAO
export DDS_ROOT=/opt/local/OpenDDS-1.0

export PATH=$QTDIR/bin:$PATH
export PATH=$ACE_ROOT/bin:$PATH
export PATH=$DDS_ROOT/bin:$PATH
export PATH=$TAO_ROOT/bin:$PATH
export PATH=$ELDORADIR/bin:$PATH


export PATH=/usr/java/default/bin:$PATH

export LD_LIBRARY_PATH=$QTDIR/plugins/designer/:$LD_LIBRARY_PATH
export LD_LIBRARY_PATH=$ACE_ROOT/lib/:$LD_LIBRARY_PATH
export LD_LIBRARY_PATH=$TAO_ROOT/lib/:$LD_LIBRARY_PATH
export LD_LIBRARY_PATH=$DDS_ROOT/lib/:$LD_LIBRARY_PATH
export LD_LIBRARY_PATH=$ELDORADIR/lib/:$LD_LIBRARY_PATH

export DSP21KSF=/opt/dsp21ksf

export JAVA_HOME=/usr/default/java
