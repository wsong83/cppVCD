// -*- Bison -*-
%skeleton "lalr1.cc"
%defines
%define namespace "vcd"
%define parser_class_name "vcd_parser"
%language "c++"
%output "vcd.cc"
%parse-param {vcd::VCDLexer* lexer}
%parse-param {vcd::WaveDB* db}
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

  using namespace vcd;

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
%token<tType>   VCDTimeUnit
%token<tType>   VCDScopeType
%token<tType>   VCDVarType
%token<tType>   VCDSimCmdType
%token<tStr>    VCDStr
%token<tValue>  VCDValue
%token<tBValue> VCDBValue
%token<tReal>   VCDReal
%token<tNum>    VCDNum

%type<tStr>     identifier
%type<tCId>     complex_identifier

// types

%start value_change_dump_definitions

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
    { db->pop_scope(); }
    | "$var"               var_decl               "$end"
    | "$version"           comments               "$end"

comment_command
    : "$comment"           comments               "$end"

comments
    :
    | VCDStr   { }
    | comments VCDStr { }

scope_decl
    : VCDScopeType identifier
    {
      db->push_scope($2);
    }

timescale_decl
    : VCDNum VCDTimeUnit
    {
      db->set_time_unit($1.get_ui(), timeunit_stype($2));
    }

var_decl
    : VCDVarType VCDNum identifier complex_identifier
    {
      db->add_id($3, $4.first, $4.second, $2.get_ui());
    }

complex_identifier
    : identifier
    {
      $$.first = $1;
    }
    | identifier '[' VCDNum ']'
    {
      $$.first = $1;
      $$.second = CRange($3.get_ui());
    }
    | identifier '[' VCDNum ':' VCDNum ']'
    {
      $$.first = $1;
      $$.second = CRange($3.get_ui(), $5.get_ui());
    }

simulation_command
    : VCDSimCmdType value_changes "$end" 
    | comment_command
    | value_change
    | simulation_time

simulation_time 
    : '#' VCDNum
    {
      db->set_time($2);
    }

value_changes
    : value_change
    | value_changes value_change

value_change
    : scalar_value_change
    | vector_value_change

scalar_value_change
    : VCDValue identifier
    {
      db->add_change($2, $1);
    }

vector_value_change
    : 'b' VCDBValue identifier
    { db->add_change($3, $2); }
	| 'r' VCDReal   identifier
    { db->add_change($3, $2); }

identifier
: VCDStr  { $$ = $1; }

%%

void vcd::vcd_parser::error (const vcd::location& loc, const std::string& msg) {
  std::cout << loc << ":" << msg << std::endl;
  }

