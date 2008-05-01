|
|	$Id$
|
|	Module:   	 ctor_dummy.s
|	Original Author: Richard E. K. Neitzel
|       Copywrited by the National Center for Atmospheric Research
|	Date:		 $Date$
|
| revision history
| ----------------
| $Log$
| Revision 1.1  1991/02/15  15:50:02  thor
| Initial revision
|
|
| calling convention:
|
| description:
|        This file makes a dummy CTOR/DTOR list that allows
| builtin++.o to load if there are no globals in use.
|
	.data
Id:
	.ascii "$Date$ $RCSfile$ $Revision$"

#NO_APP
.data
.globl ___CTOR_LIST__
___CTOR_LIST__:
	.long	0
	.long	0
.data
.globl ___DTOR_LIST__
___DTOR_LIST__:
	.long	0
	.long	0
	.long	0
