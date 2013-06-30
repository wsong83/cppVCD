// -*- Bison -*-
%skeleton "lalr1.cc"
%defines
%define namespace "vcd"
%define parser_class_name "vcd_parser"
%language "c++"
%output "vcd.cc"
%parse-param {vcd::VCDLexer* lexer}
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

#include "vcd_util.hpp"
  
#define yylex lexer->lexer
#define YYSTYPE vcd::vcd_token_type

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
%token<tType>   VCDTimeUnit
%token<tType>   VCDScopeType
%token<tType>   VCDVarType
%token<tType>   VCDSimCmdType
%token<tStr>    VCDStr
%token<tValue>  VCDValue
%token<tBValue> VCDBValue
%token<tReal>   VCDReal
%token<tNum>    VCDNum

// types

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
    : comment_command
    | "$date"              comments               "$end"
    | "$enddefinitions"                           "$end"
    | "$scope"             scope_decl             "$end"
    | "$timescale"         timescale_decl         "$end"
    | "$upscope"                                  "$end"
    | "$var"               var_decl               "$end"
    | "$version"           comments               "$end"
    | error

comment_command
    : "$comment"           comments               "$end"
    | error

comments
    :
    | VCDStr   {}
    | comments VCDStr {}

scope_decl
    : VCDScopeType identifier
    | error

timescale_decl
    : VCDNum VCDTimeUnit
    | error

var_decl
    : VCDVarType VCDNum identifier complex_identifier
    | error

complex_identifier
    : identifier
    | identifier '[' VCDNum ']'
    | identifier '[' VCDNum ':' VCDNum ']'
    | error

simulation_command
    : VCDSimCmdType value_changes "$end" 
    | comment_command
    | value_change
    | simulation_time
    | error

simulation_time 
    : '#' VCDNum

value_change
    : scalar_value_change
    | vector_value_change
    | error

scalar_value_change
    : VCDValue identifier

vector_value_change
    : 'b' VCDBValue identifier
	| 'r' VCDReal   identifier

identifier
    : VCDStr
