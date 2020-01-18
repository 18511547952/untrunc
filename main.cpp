/*
    Untrunc - main.cpp

    Untrunc is GPL software; you can freely distribute,
    redistribute, modify & use under the terms of the GNU General
    Public License; either version 2 or its successor.

    Untrunc is distributed under the GPL "AS IS", without
    any warranty; without the implied warranty of merchantability
    or fitness for either an expressed or implied particular purpose.

    Please see the included GNU General Public License (GPL) for
    your rights and further details; see the file COPYING. If you
    cannot, write to the Free Software Foundation, 59 Temple Place
    Suite 330, Boston, MA 02111-1307, USA.  Or www.fsf.org

    Copyright 2010 Federico Ponchio
                                                                                
                                                        */

#include "mp4.h"
#include "atom.h"
#include "log.h"

#include <iostream>
#include <string>
using namespace std;

void usage() {
	cout << "Usage: untrunc [-a -i -v -w] <ok.mp4> [<corrupt.mp4>]\n\n"
		<< "	-a: test the ok video\n"
		<< "	-q: silent\n"
		<< "	-e: error\n"
		<< "	-v; verbose\n"
		<< "	-w: debug info\n\n";
}

int main(int argc, char *argv[]) {

    bool info = false;
    bool analyze = false;
    int i = 1;
    for(; i < argc; i++) {
        string arg(argv[i]);
        if(arg[0] == '-') {
            switch(arg[1]) {
            case 'i': info = true; break;
            case 'a': analyze = true; break;
            case 'q': Logger::log_level = Logger::SILENT; break;
			case 'e': Logger::log_level = Logger::ERROR; break;
            case 'v': Logger::log_level = Logger::INFO; break;
            case 'w': Logger::log_level = Logger::DEBUG; break;
            }
        } else
            break;
    }
    if(argc == i) {
        usage();
        return -1;
    }

    string ok = argv[i];
    string corrupt;
    i++;
    if(i < argc)
        corrupt = argv[i];

    Log::info << "Reading: " << ok << endl;
    Mp4 mp4;

    try {
        mp4.open(ok);
        if(info) {
            mp4.printMediaInfo();
            mp4.printAtoms();
        }
        if(analyze) {
            mp4.analyze();
        }
        if(corrupt.size()) {
            mp4.repair(corrupt);
			size_t lastindex = corrupt.find_last_of(".");
			string fixed = corrupt.substr(0, lastindex);
			mp4.saveVideo(fixed + "_fixed.mp4");
        }
    } catch(string e) {
        Log::error << e << endl;
        return -1;
    }
    return 0;
}
