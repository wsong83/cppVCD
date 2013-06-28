// -*- Bison -*-
%skeleton "lalr1.cc"
%defines
%define namespace "vcd"
%define parser_class_name "vcd_parser"
%language "c++"
%output "vcd.cc"
%{
/*
 * Copyright (c) 2013-2013 Wei Song <songw@cs.man.ac.uk> 
 *    Advanced Processor Technologies Group, School of Computer Science
 *    University of Manchester, Manchester M13 9PL UK
 *
 *    This source code is free software; you can redistribute it
 *    and/or modify it in source code form under the terms of the GNU
 *    General Public License as published by the Free Software
 *    Foundation; either version 2 of the License, or (at your option)
 *    any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program; if not, write to the Free Software
 *    Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA
 */

/* 
 * Bison grammer file for Value Change Dump (VCD)
 * 28/06/2013   Wei Song
 *
 *
 */

#include <string>
#include <iostream>

  namespace vcd {

    struct vcd_token_type {
      std::string              tStr;            /* string */ 
      std::string              tID;             /* identifier */
      char                     tValue;          /* a 4-value value */
      std::string              tValueVec;       /* value vector */
      double                   tReal;           /* real value */
    };

    int vcd_lexer(vcd_token_type *);
  }
  
#define yylex 

%}


%initial-action
{
}


///////////////////////////////////////////////////
// token definitions

%token VCDEnd                            "$end"
%token VCDComment                        "$comment"
%token VCDDate                           "$date"
%token VCDEndDef                         "$enddefinitions"
%token VCDScope                          "$scope"
%token VCDTimeScale                      "$timescale"
%token VCDUpScope                        "$upscope"
%token VCDVar                            "$var"
%token VCDVersion                        "$version"
%token VCDDumpAll                        "$dumpall"
%token VCDDumpOff                        "$dumpoff"
%token VCDDumpOn                         "$dumpon"
%token VCDDumpVars                       "$dumpvars"

%token VLOGBegin                         "begin"
%token VLOGFork                          "fork"
%token VLOGFunction                      "function"
%token VLOGModule                        "module"
%token VLOGTask                          "task"

// types
%type <tStr>            plain_string
%type <tID>             identifier
%type <tValue>          value
%type <tValueVec>       binary_number
%type <tReal>           real_number

%start source_text

%%

// the VCD file
value_change_dump_definitions
    : declaration_commands simulation_commands

declaration_commands
    : declaration_command
    | declaration_commands declaration_command

simulation_commands
    : simulation_command
    | simulation_commands simulation_command

// declaration
declaration_command
    : "$comment"           comments               "$end"
    | "$date"              comments               "$end"
    | "$enddefinitions"                           "$end"
    | "$scope"             scope_decl             "$end"
    | "$timescale"         timescale_decl         "$end"
    | "$upscope"                                  "$end"
    | "$var"               var_decl               "$end"
    | "$version"           comments               "$end"
    | error

comments
    :
    | plain_string   {}
    | comments plain_string {}

multi_plain_string
    : plain_string
    | multi_plain_string plain_string

scope_decl
    : "begin" tID
    {                           /* named sequential blocks */
    }
    | "fork" tID
    {                           /* named parallel block */
    }
    | "function" tID
    {                           /* function */
    }
    | "module" tID 
    {                           /* module */
    }
    | "task" tID
    {                           /* task */
    }
    | error

timescale_decl
    : multi_plain_string
    {                           /* 1|10|100 s|ms|us|ns|ps|fs */
    }
    | error


simulation_command ::=
	simulation_keyword { value_change } $end
	| $comment [ comment_text ] $end
	| simulation_time
	| value_change
simulation_keyword ::=
	$dumpall | $dumpoff | $dumpon | $dumpvars
simulation_time ::=
	# decimal_number
value_change ::=
	scalar_value_change
	| vector_value_change
scalar_value_change ::=
	value identifier_code
value ::=
	0 | 1 | x | X | z | Z
vector_value_change ::=
	b binary_number identifier_code
	| B binary_number identifier_code
	| r real_number identifier_code
	| R real_number identifier_code
identifier_code ::=
	{ ASCII character }
