
/***************************************************************************
 *   Copyright (C) 2020                                                    *
 *                                                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/


/**
	@author Broux Paul-Emmanuel <paulemmanuelb@gmail.com>
 */


#include <stdio.h>
#include <string.h>
#include "aescuda.h"



int main(int argc, char *argv[])
{


    ///////////////////////////////////////////////////////////////
    // command line arguments
    ///////////////////////////////////////////////////////////////
    int     warm_up_device      = 0;					// GPU kernel warm up
	int     warm_up_host        = 1;					// Host warm up (with GPU kernel inside)
	int		bench_mode			= 0;					// Activate benchmarking mode (less messages and only benchmark output allow)
	int		argCtr				= 0;					// Count options used in command line

    std::cout << std::endl << "********************************************************" << std::endl;
    std::cout << std::endl << "******           First CUDA GPU AES               ******" << std::endl;
    std::cout << std::endl << "********************************************************" << std::endl;


    if (argc > 1)
    {
        for( int n=1 ; n<argc ; n=n+2 )
        {
            if( (strcmp(argv[n],"-wuDevice") == 0) && (n+1<argc) )
            {
                warm_up_device = atoi(argv[n+1]);
				argCtr+=2;
            }
            else if( (strcmp(argv[n],"-wuHost") == 0) && (n+1<argc) )
            {
                warm_up_host = atoi(argv[n+1]);
				argCtr+=2;
            }
            else if( (strcmp(argv[n],"-benchMode") == 0) && (n+1<argc) )
            {
                bench_mode = atoi(argv[n+1]);
				argCtr+=2;
            }
            else if( (strcmp(argv[n],"-help") == 0) )
            {
                std::cout << "   Arguments on the command line should be :" << std::endl;
				std::cout << "   <e|d> <128|256> dataInput1 keyFile1 dataInput2 keyFile2 ......" << std::endl;
				std::cout << "   With couples of data files to encrypt|decrypt and keyfiles corresponding to the aes type chosen (128|256) ." << std::endl;
				std::cout << "   Then with \"-options value\" couples." << std::endl;
                std::cout << "   The order between options is not important." << std::endl << std::endl;
                std::cout << "   Accepted options are :" << std::endl ;
                std::cout << "   -benchMode, 0 to disable and 1 to avoid  several messages and keeping only benchmarking file output into bench directory" << std::endl;
                std::cout << "   -wuDevice number of device kernel launch before timing (around 1000 or less is adviced) " << std::endl;
                std::cout << "   -wuHost number of main Host code launch for several timing results (around 10 should be enough)" << std::endl;
                exit(0);
            }
        }


    }
	else {
		std::cout << "   Not enough arguments. Type -help option in the command line for further explanations." << std::endl;
		exit(0);
	}

    std::cout << "    benchMode		= " << bench_mode << std::endl;
    std::cout << "    wuHost		= " << warm_up_host << std::endl;
    std::cout << "    wuDevice		= " << warm_up_device << std::endl;

	//Storing arguments for main_AES
	int argc_AES = argc - argCtr;						//Usual arguments number for AES function in aescuda.cpp
	char *argv_AES[ argc_AES  ];

	for(int i=0; i < argc_AES ; i++){						
		argv_AES[i] = argv[i];	//Copy arguments needed to the main AES function
	}


	//Calling AES main function
	for(int i=0; i<warm_up_host ; i++){
		main_AES(argc_AES, argv_AES, bench_mode,  warm_up_device);
	}


	return 0;

}
