/******************************************************************************
 *                                                                            *
 *  Copyright (C) 2002-2025 Cyril Soler. All rights reserved.                 *
 *                                                                            *
 *     Cyril Soler, INRIA                   <cyril.soler@inria.fr>            *
 *                                                                            *
 *  This code may be used for research purposes only, on condition to credit  *
 *  its author.                                                               *
 *                                                                            *
 *  This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE   *
 *  WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. *
 *                                                                            *
 ******************************************************************************/

#pragma once


#include <stdio.h>
#include <sstream>
#include <iostream>
#include <map>
#include <fstream>
#include <string.h>

class Config: public std::map<std::string,std::string>
{
	public:
		/// Constructor. Parses the config file filename and builds a database
		/// of variable names and values.
		Config(const std::string& filename)
		{
			FILE *f = fopen(filename.c_str(),"r") ;

            mFilename = filename;

			if(f == NULL)
                return;

			char buff[500] ;
			int lineno = 0 ;

			while(fgets(buff,500,f) != NULL)
			{
				lineno++ ;
				int lp = 0 ;
				int lq = 0 ;
				char *p = buff ;
				char *q = buff ;
				char *r = NULL ;

				if(strlen(buff) > 499)
				{
					std::cerr << "Line too long skipped (line " << lineno << ") in config file " << filename << std::endl ;
					goto cont ;
				}

				while((*p == ' ')||(*p == '\t')) p++ ;
				if(*p == '\n') goto cont ; 

				if(*p == '#')		// skip comment
					goto cont ;

				while(*q != '=') if(*q == '\n') goto cont1 ; else q++ ; 
				r = q-1 ;
				while((*r == ' ')||(*r == '\t')) r-- ;
				lp = r - p + 1 ;
				q++ ;
				while((*q == ' ')||(*q == '\t')) q++ ;
				if(*q == '\n') goto cont1 ; 
				
				lq = strlen(q) - 1 ;

//				p1 = new char[lp+1];
//				p2 = new char[lq+1];
//
//				strncpy(p1,p,lp) ; p1[lp] = 0 ;
//				strncpy(p2,q,lq) ; p2[lq] = 0 ;

				operator[](std::string(p,lp)) = std::string(q,lq) ;
				continue ;

cont1:
				std::cout << "Syntax error in config file " << filename << " line " << lineno << std::endl ;
cont:		;
			}

			fclose(f) ;
		}

		template<class T> void setValue(const std::string& name,const T& value)
		{
			std::ostringstream s ;
			s << value ;

			operator[](name) = s.str() ;
		}
		/// Returns the value associated to the variable of name name
		/// if anything fails, the returned value is false, else it is true.

		template<class T> T getValue(const std::string& name,const T& default_value) 
		{
			std::map<std::string,std::string>::const_iterator it = find(name) ;

			if(it == end())
				return default_value ;

			std::istringstream is(it->second) ;
			T t ;
			is >> t ;

			return t ;
		}

        void save(const std::string& fname = std::string()) const
        {
            if(fname.empty())
            {
                std::ofstream of(mFilename.c_str()) ;
                of << (*this) ;
                of.close() ;
            }
            else
            {
                std::ofstream of(fname.c_str()) ;
                of << (*this) ;
                of.close() ;
            }
        }

		friend std::ostream& operator<<(std::ostream& out,const Config& conf)
		{
			for(std::map<std::string,std::string>::const_iterator it(conf.begin());it!=conf.end();++it)
				out << it->first << " = " << it->second << std::endl ;

			return out ;
		}

        std::string mFilename;
};

extern Config *game_config;
