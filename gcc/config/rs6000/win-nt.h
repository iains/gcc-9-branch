/* Definitions of target machine for GNU compiler, for PowerPC
   running Windows/NT.
   Copyright (C) 1995 Free Software Foundation, Inc.
   Contributed by Cygnus Support.

This file is part of GNU CC.

GNU CC is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2, or (at your option)
any later version.

GNU CC is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GNU CC; see the file COPYING.  If not, write to
the Free Software Foundation, 59 Temple Place - Suite 330,
Boston, MA 02111-1307, USA.  */

/* Say this is Windows/NT for the other config files.  */
#define WINDOWS_NT 1
#define COFF_WITH_PE 1

/* Default ABI to compile code for */
#define DEFAULT_ABI ABI_NT

#include "rs6000/powerpc.h"

/* Pseudo target that we can test in the md file.  */
#undef 	TARGET_WINDOWS_NT
#define	TARGET_WINDOWS_NT 1

#undef	CPP_PREDEFINES
#define	CPP_PREDEFINES "-DWIN32 -D_WIN32 \
  -DWINNT -D__STDC__=0 -DALMOST_STDC \
  -D_POWER -D_ARCH_PPC -D__PPC__ -Asystem(winnt) -Acpu(powerpc) -Amachine(powerpc)"

#if 0
#include "winnt/win-nt.h"
#endif

#undef	LIB_SPEC
#define	LIB_SPEC "%{mwindows:-subsystem:windows -entry:WinMainCRTStartup \
  USER32.LIB GDI32.LIB COMDLG32.LIB WINSPOOL.LIB} \
 %{!mwindows:-subsystem console -e mainCRTStartup} \
 %{mcrtmt:LIBCMT.LIB KERNEL32.LIB} %{!mcrtmt:-lkernel32 -lcygwin} \
 %{v}"

#undef	LINK_SPEC
#define	LINK_SPEC "%{v:-V}"

/* Allow switches specified in LIB_SPEC, but don't do anything with them
   in the compiler.  */
#undef	SUBTARGET_SWITCHES
#define SUBTARGET_SWITCHES	\
   { "windows",	0 },		\
   { "crtmt",	0 },

#undef XCOFF_DEBUGGING_INFO

/* this is pure coff, not xcoff */
#define SDB_DEBUGGING_INFO
#define DBX_DEBUGGING_INFO

#undef  SDB_DELIM
#define SDB_DELIM ";"

#undef	PREFERRED_DEBUGGING_TYPE
#define PREFERRED_DEBUGGING_TYPE DBX_DEBUG

#undef PROCESSOR_DEFAULT
#define PROCESSOR_DEFAULT PROCESSOR_POWERPC

/* NT always runs little endian */
#undef  BYTES_BIG_ENDIAN
#define BYTES_BIG_ENDIAN 0 

#undef  WORDS_BIG_ENDIAN
#define WORDS_BIG_ENDIAN 0 

/* Define cutoff for using external functions to save floating point.
   Currently on NT, always use inline stores */
#undef	FP_SAVE_INLINE
#define FP_SAVE_INLINE(FIRST_REG) ((FIRST_REG) < 64)

/* Note, little endian systems trap on unaligned addresses, so never
   turn off strict alignment in that case. */

#undef STRICT_ALIGNMENT
#define	STRICT_ALIGNMENT 1

/* Align stack to 16 byte boundaries */
#undef	STACK_BOUNDARY
#define	STACK_BOUNDARY	128

/* No data type wants to be aligned rounder than this.  */
#undef	BIGGEST_ALIGNMENT
#define BIGGEST_ALIGNMENT 128

#undef TARGET_VERSION
#define TARGET_VERSION fprintf (stderr, " (PowerPC PE)");

#undef TARGET_DEFAULT 
#define TARGET_DEFAULT (MASK_POWERPC | MASK_NEW_MNEMONICS | MASK_NO_FP_IN_TOC | MASK_NO_SUM_IN_TOC)

#undef PROCESSOR_DEFAULT
#define PROCESSOR_DEFAULT PROCESSOR_PPC601

/* Address to save the TOC register */
#undef	RS6000_SAVE_TOC
#define RS6000_SAVE_TOC plus_constant (virtual_incoming_args_rtx, -RS6000_SAVE_AREA - 8)

/* Windows NT specifies that r13 is reserved to the OS, so it is not available
   to the normal user.  */

#undef	FIXED_R13
#define FIXED_R13 1


/* Output .file and comments listing what options there are */
#undef	ASM_FILE_START
#define ASM_FILE_START(FILE)					\
{								\
  ASM_OUTPUT_OPTIONS (FILE);					\
  output_file_directive (FILE, main_input_filename);		\
}

/* Define the extra sections we need.  We define three: one is the read-only
   data section which is used for constants.  This is a csect whose name is
   derived from the name of the input file.  The second is for initialized
   global variables.  This is a csect whose name is that of the variable.
   The third is the TOC.  */

#undef SELECT_SECTION

#undef	READONLY_DATA_SECTION
#undef	EXTRA_SECTIONS
#define EXTRA_SECTIONS toc, bss

/* Define the routines to implement these extra sections.  */

#undef	EXTRA_SECTION_FUNCTIONS
#define EXTRA_SECTION_FUNCTIONS				\
							\
void							\
toc_section ()						\
{							\
}


#undef SELECT_RTX_SECTION
#undef ASM_DECLARE_FUNCTION_NAME


/* This says how to output an assembler line
   to define a global common symbol.  */

#undef	ASM_OUTPUT_ALIGNED_COMMON
#define ASM_OUTPUT_ALIGNED_COMMON(FILE, NAME, SIZE, ALIGNMENT)	\
  do { fputs ("\t.comm \t", (FILE));			        \
       assemble_name ((FILE), (NAME));				\
       if ( (SIZE) > 4)                                         \
         fprintf ((FILE), ",%d,%d\n", (SIZE), 3);               \
       else                                                     \
	 fprintf( (FILE), ",%d\n", (SIZE));                     \
  } while (0) 

#undef	ASM_OUTPUT_ALIGNED_LOCAL

/* This says how to output an assembler line
   to define a global common symbol.  */

#undef  ASM_OUTPUT_COMMON
#define ASM_OUTPUT_COMMON(FILE, NAME, SIZE, ROUNDED)	\
  do { fputs ("\t.comm \t", (FILE));			\
       assemble_name ((FILE), (NAME));			\
       fprintf ((FILE), ",%d\n", (SIZE)); } while (0)

/* This says how to output an assembler line
   to define a local common symbol.  */

#undef ASM_OUTPUT_LOCAL
#define ASM_OUTPUT_LOCAL(FILE, NAME, SIZE,ROUNDED)	\
  do { fputs ("\t.lcomm \t", (FILE));			\
       assemble_name ((FILE), (NAME));			\
       fprintf ((FILE), ",%d\n", (SIZE));               \
     } while (0)


/* Stuff to force fit us into the Motorola PPC assembler */

#if 0
#undef ASM_FILE_START
#define ASM_FILE_START(FILE)					\
{								\
  output_file_directive (FILE, main_input_filename);		\
  fprintf (FILE, "\n#\tDirective section\n");                   \
  fprintf (FILE, "\t.section\t.drectve,\"iR\"\n");              \
  fprintf (FILE, "\t.byte\t\"-defaultlib:LIBC\" \n");           \
  fprintf (FILE, "\t.previous\n\n");                              \
}
#endif

#undef ASM_FILE_START
#define ASM_FILE_START(FILE)					\
{								\
  ASM_OUTPUT_OPTIONS (FILE);					\
  output_file_directive (FILE, main_input_filename);		\
  data_section ();						\
}

#undef ASM_FILE_END

#undef  ASM_OUTPUT_FUNCTION_PREFIX
#define ASM_OUTPUT_FUNCTION_PREFIX(FILE,NAME)                   \
{                                                               \
  fprintf (FILE, "\n#\tFunction: '..");                         \
  assemble_name (FILE, NAME);					\
  fprintf (FILE, "'\n");                                        \
  fprintf (FILE, "#\tText in section: <%s>\n\n","default");     \
  fprintf (FILE, "#\tSetup MS Structured-Exception-Handling\n"); \
  fprintf (FILE, "\t.pdata\n");					\
  fprintf (FILE, "\t.align 2\n");				\
  fprintf (FILE, "\t.ualong ..");                               \
  assemble_name (FILE, NAME);					\
  fprintf (FILE, ",");                                          \
  assemble_name (FILE, NAME);					\
  fprintf (FILE, ".e,0,0,");                                    \
  assemble_name (FILE, NAME);					\
  fprintf (FILE, ".b\n\n");                                     \
  fprintf (FILE, "#\tSwitch to the relocation section\n");      \
  fprintf (FILE, "\t.reldata\n");                               \
}


#define ASM_DECLARE_FUNCTION_NAME(FILE,NAME,DECL)		\
{                                                               \
  if (TREE_PUBLIC (DECL))					\
    {								\
      fprintf (FILE, "\t.globl ..");				\
      assemble_name (FILE, NAME);				\
      fprintf (FILE, "\n");					\
    }								\
  assemble_name (FILE, NAME);					\
  fprintf (FILE, ":\n");					\
  fprintf (FILE, "\t.ualong ..");				\
  assemble_name (FILE, NAME);					\
  fprintf (FILE, ",.toc\n");				        \
								\
  if (lookup_attribute ("dllexport",				\
			TYPE_ATTRIBUTES (TREE_TYPE (DECL))))	\
    {								\
      fprintf (FILE, "\t.globl __imp_");			\
      assemble_name (FILE, NAME);				\
      fprintf (FILE, "\n__imp_");				\
      assemble_name (FILE, NAME);				\
      fprintf (FILE, ":\n\t.ulong ");				\
      assemble_name (FILE, NAME);				\
      fprintf (FILE, "\n");					\
    }								\
								\
  fprintf (FILE, "\t.section .text\n\t.align 2\n..");		\
  assemble_name (FILE, NAME);					\
  fprintf (FILE, ":\n");					\
  fprintf (FILE, "\t.function\t..");                            \
  assemble_name (FILE, NAME);					\
  fprintf (FILE, "\n");					        \
}

/* This is how to output an assembler line defining a `double' constant.  */

#undef	ASM_OUTPUT_DOUBLE
#define ASM_OUTPUT_DOUBLE(FILE, VALUE)					\
  {									\
    if (REAL_VALUE_ISINF (VALUE)					\
        || REAL_VALUE_ISNAN (VALUE)					\
	|| REAL_VALUE_MINUS_ZERO (VALUE))				\
      {									\
	long t[2];							\
	REAL_VALUE_TO_TARGET_DOUBLE ((VALUE), t);			\
	fprintf (FILE, "\t.ualong 0x%lx\n\t.long 0x%lx\n",		\
		t[0] & 0xffffffff, t[1] & 0xffffffff);			\
      }									\
    else								\
      {									\
	char str[30];							\
	REAL_VALUE_TO_DECIMAL (VALUE, "%.20e", str);			\
	fprintf (FILE, "\t.double %s\n", str);				\
      }									\
  }

/* This is how to output an assembler line defining a `float' constant.  */

#undef	ASM_OUTPUT_FLOAT
#define ASM_OUTPUT_FLOAT(FILE, VALUE)					\
  {									\
    if (REAL_VALUE_ISINF (VALUE)					\
        || REAL_VALUE_ISNAN (VALUE)					\
	|| REAL_VALUE_MINUS_ZERO (VALUE))				\
      {									\
	long t;								\
	REAL_VALUE_TO_TARGET_SINGLE ((VALUE), t);			\
	fprintf (FILE, "\t.ualong 0x%lx\n", t & 0xffffffff);		\
      }									\
    else								\
      {									\
	char str[30];							\
	REAL_VALUE_TO_DECIMAL ((VALUE), "%.20e", str);			\
	fprintf (FILE, "\t.float %s\n", str);				\
      }									\
  }

/* Output before instructions.  */
#undef TEXT_SECTION_ASM_OP
#define TEXT_SECTION_ASM_OP "\t.text"

/* Output before writable data.  */
#undef DATA_SECTION_ASM_OP
#define DATA_SECTION_ASM_OP "\t.data"

/* Text to write out after a CALL that may be replaced by glue code by
   the loader.  The motorola asm demands that, for dll support, a .znop
   be issued after a bl instruction, and the symbol on the .znop is the
   symbol on the bl instruction */

#undef RS6000_CALL_GLUE
#define RS6000_CALL_GLUE "nop #\tFIXME: only works for non-dll calls."

#define RS6000_CALL_GLUE2 ".znop "

#undef ASM_OUTPUT_SPECIAL_POOL_ENTRY

/* Output something to declare an external symbol to the assembler.  Most
   assemblers don't need this.  */

#undef ASM_OUTPUT_EXTERNAL

#define ASM_OUTPUT_EXTERNAL(FILE, DECL, NAME)				\
{									\
  char *_name;								\
  rtx _symref = XEXP (DECL_RTL (DECL), 0);				\
  if ((TREE_CODE (DECL) == VAR_DECL					\
       || TREE_CODE (DECL) == FUNCTION_DECL)				\
      && (NAME)[0] != '*'						\
      && (NAME)[strlen (NAME) - 1] != ']')				\
    {									\
      _name = (char *) permalloc (strlen (XSTR (_symref, 0)) + 5);	\
      strcpy (_name, XSTR (_symref, 0));				\
      XSTR (_symref, 0) = _name;					\
    }									\
  else									\
    _name = XSTR (_symref, 0);						\
									\
  if (DECL_FUNCTION_CODE (DECL) == 0)					\
    {									\
      fputs ("\t.extern ", FILE);					\
      assemble_name (FILE, _name);					\
      putc ('\n', FILE);						\
      if (TREE_CODE (DECL) == FUNCTION_DECL)				\
	{								\
	  fputs ("\t.extern ..", FILE);					\
	  assemble_name (FILE, _name);					\
	  putc ('\n', FILE);						\
	}								\
    }									\
}

/* Similar, but for libcall.  We only have to worry about the function name,
   not that of the descriptor. */

#undef ASM_OUTPUT_EXTERNAL_LIBCALL

#define ASM_OUTPUT_EXTERNAL_LIBCALL(FILE, FUN)	\
{ fprintf (FILE, "\t.extern ..");		\
  assemble_name (FILE, XSTR (FUN, 0));		\
  fprintf (FILE, "\n");				\
}


/* Eliminate AIX style constant pool processing */
#undef	LEGITIMATE_CONSTANT_POOL_BASE_P
#define	LEGITIMATE_CONSTANT_POOL_BASE_P(X) 0

#undef	LEGITIMATE_CONSTANT_POOL_ADDRESS_P
#define	LEGITIMATE_CONSTANT_POOL_ADDRESS_P(X) 0

#undef ASM_OUTPUT_SPECIAL_POOL_ENTRY

#undef  ASM_IDENTIFY_GCC
#define ASM_IDENTIFY_GCC(x)

#undef	HAS_INIT_SECTION
#define HAS_INIT_SECTION
