#ifndef DDSTEMPLATESIGS_H_
#define DDSTEMPLATESIGS_H_
/// @file Definitions which encapsulate template signatures used in the DDS wrapper classes.
/// Those classes are templatizes upon a bunch of classes that are required for the Corba/DDS
/// support, and these classes are all defined during the Corba IDL compilation. Thus, if
/// the IDL defines a type Pulse, then the IDL compilation will result in the types
/// Pulse, PulseTypeSupportImpl, PulseTypeSupport_var, PulseDataWriter, PulseDataWriter_var
/// These defines just pretty up the template implmentation and instantiation code.
/// @todo There don't really need to be separate reader and writer signatures. The implmentations
/// should be changed to use something like DDSDATARW in place of DDSDATAREADER and DDSDATAWRITER, etc.

/// Definitions for the writer class
/// The template signature for the template parameter in the writer class declaration in the header file:
#define WRITERSIG1 class DDSTYPE, class DDSTYPESUPPORTIMPL, class DDSTYPESUPPORT_VAR, class DDSDATAWRITER, class DDSDATAWRITER_VAR
/// The signature necessary to identify the writerimplmentation in the implmentation file:
#define WRITERSIG2 DDSTYPE, DDSTYPESUPPORTIMPL, DDSTYPESUPPORT_VAR, DDSDATAWRITER, DDSDATAWRITER_VAR
/// A macro which will expand the base type into all of the types that are required for instantiation. Note
/// the use of cpp concatenation.
#define WRITERSIG(T) T, T ## TypeSupportImpl, T ## TypeSupport_var, T ## DataWriter, T ## DataWriter_var

/// Definitions for the reader class
/// The template signature for the template parameter in the reader class declaration in the header file:
#define READERSIG1 class DDSTYPE, class DDSTYPESUPPORTIMPL, class DDSTYPESUPPORT_VAR, class DDSDATAREADER, class DDSDATAREADER_VAR
/// The signature necessary to identify the readerimplmentation in the implmentation file:
#define READERSIG2 DDSTYPE, DDSTYPESUPPORTIMPL, DDSTYPESUPPORT_VAR, DDSDATAREADER, DDSDATAREADER_VAR
/// A macro which will expand the base type into all of the types that are required for instantiation. Note
/// the use of cpp concatenation.
#define READERSIG(T) T, T ## TypeSupportImpl, T ## TypeSupport_var, T ## DataReader, T ## DataReader_var

#endif /*DDSTEMPLATESIGS_H_*/
