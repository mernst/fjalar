/*
   This file is part of Kvasir, a Valgrind skin that implements the
   C language front-end for the Daikon Invariant Detection System

   Copyright (C) 2004 Philip Guo, MIT CSAIL Program Analysis Group

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation; either version 2 of the
   License, or (at your option) any later version.
*/

/* dtrace-output.h:
   Functions for outputting Kvasir runtime variable values
   to a Daikon-compatible .dtrace file
*/

#ifndef DTRACE_OUTPUT_H
#define DTRACE_OUTPUT_H

#include "generate_daikon_data.h"
#include "kvasir_runtime.h"
#include "disambig.h"
#include "decls-output.h"

int openDtraceFile(const char *fname);
void printDtraceFunctionHeader(DaikonFunctionInfo* funcPtr, char isEnter);
void finishDtraceFile();
char mapInitToModbit(char init);

char outputDtraceValue(DaikonVariable* var,
		       void* basePtrValue,
		       VariableOrigin varOrigin,
		       char isHashcode,
		       char overrideIsInitialized,
		       char isDummy,
		       char isSequence,
		       unsigned long upperBound,
                       void** sequenceEltsToOutput,
		       char overrideFloatAsDouble,
		       DisambigOverride disambigOverride);

void printOneDtraceString(char* str);
void printOneCharAsDtraceString(char c);
void printOneDtraceStringAsIntArray(char* str);

char printDtraceSingleHashcode(DaikonVariable* var,
                               void* ptrValue);

void printDtraceHashcodeSequence(DaikonVariable* var,
                                 void** sequenceEltsToOutput,
                                 unsigned long upperBound);

char printDtraceSingleString(DaikonVariable* var,
                             void* ptrValue,
                             char overrideIsInitialized,
                             DisambigOverride disambigOverride);

void printDtraceStringSequence(DaikonVariable* var,
                               DisambigOverride disambigOverride,
                               void** sequenceEltsToOutput,
                               unsigned long upperBound);

char printDtraceSingleBaseValue(DaikonVariable* var,
                                char* ptrValue, // Make pointer arithmetic increment by 1
                                // not necessarily the same as var's declared type
                                DaikonDeclaredType decType,
                                char overrideIsInitialized,
                                DisambigOverride disambigOverride);

void printDtraceBaseValueSequence(DaikonVariable* var,
                                  // not necessarily the same as var's declared type
                                  DaikonDeclaredType decType,
                                  void** sequenceEltsToOutput,
                                  unsigned long upperBound,
                                  DisambigOverride disambigOverride,
                                  char overrideIsInitialized);

#endif