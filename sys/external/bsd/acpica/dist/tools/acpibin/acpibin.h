/******************************************************************************
 *
 * Module Name: acpibinh - Include file for AcpiBin utility
 *
 *****************************************************************************/

/*
 * Copyright (C) 2000 - 2020, Intel Corp.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions, and the following disclaimer,
 *    without modification.
 * 2. Redistributions in binary form must reproduce at minimum a disclaimer
 *    substantially similar to the "NO WARRANTY" disclaimer below
 *    ("Disclaimer") and any redistribution must be conditioned upon
 *    including a substantially similar Disclaimer requirement for further
 *    binary redistribution.
 * 3. Neither the names of the above-listed copyright holders nor the names
 *    of any contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * Alternatively, this software may be distributed under the terms of the
 * GNU General Public License ("GPL") version 2 as published by the Free
 * Software Foundation.
 *
 * NO WARRANTY
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDERS OR CONTRIBUTORS BE LIABLE FOR SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGES.
 */

#include "acpi.h"
#include "accommon.h"
#include "acapps.h"

#define DB_CONSOLE_OUTPUT            0x02
#define ACPI_DB_REDIRECTABLE_OUTPUT  0x01

/*
 * Global variables. Defined in main.c only, externed in all other files
 */
#ifdef _DECLARE_GLOBALS
#define EXTERN
#define INIT_GLOBAL(a,b)        a=b
#else
#define EXTERN                  extern
#define INIT_GLOBAL(a,b)        a
#endif


/* Globals */

EXTERN BOOLEAN              INIT_GLOBAL (Gbl_TerseMode, FALSE);
EXTERN BOOLEAN              INIT_GLOBAL (AbGbl_DisplayAllMiscompares, FALSE);
EXTERN UINT32               INIT_GLOBAL (AbGbl_CompareOffset, 0);


/* Prototypes */

int
AbCompareAmlFiles (
    char                    *File1Path,
    char                    *File2Path);

int
AbDumpAmlFile (
    char                    *File1Path,
    char                    *File2Path);

void
AbComputeChecksum (
    char                    *File1Path);

void
AbDisplayHeader (
    char                    *File1Path);
