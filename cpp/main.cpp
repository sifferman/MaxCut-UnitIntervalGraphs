
// main.cpp //

#include "graph.h"

#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <fstream>
#include <sstream>
#include <string>

#include "_src/clipp/clipp.h"
using namespace std;
using namespace clipp;




vector<u2> stringToVector( const string & in );

int main( int argc, char ** argv ) { srand( time(NULL) );

    enum Src        { RANDOM, FILE };
    enum Structure  { BASIC, PATH, STAIRCASE };
    enum Algorithm  { BF, EO, SI };

    class InvalidCommandLineArgument { };

    Src src = RANDOM;
    string file_in_path = "";
    Structure structure = BASIC;
    Algorithm algorithm = BF;
    bool FileOut = true;
    string file_out_path = "out.txt";

    auto cli = (
        /* put stuff here */
        option("-r", "--random").set(src, RANDOM)
            % "Randomize the Unit Interval Graph",
        option("-fi", "--file-in").set(src, FILE) & value("file_in_path", file_in_path)
            % "Graph is loaded from file",
        
        option("-b", "--basic").set(structure, BASIC)
            % "No restrictions on structure",
        option("-p", "--path").set(structure, PATH)
            % "Path structure",
        option("-s", "--staircase").set(structure, STAIRCASE)
            % "Staircase structure",

        option("-bf", "--brute-force").set(algorithm, BF)
            % "Run brute-force algorithm",
        option("-eo", "--every-other").set(algorithm, EO)
            % "Run every-other approx algorithm",
        option("-si", "--sifferman").set(algorithm, SI)
            % "Run Sifferman alg (only on path)",

        option("-fo", "--file-out").set(FileOut) & value("file_out_path", file_out_path)
            % "Print result to file"
    );

    if( !parse(argc, argv, cli) ) {
        cout << make_man_page( cli, argv[0] );
        return -1;
    }

    UnitIntervalGraph * g;

    // https://stackoverflow.com/questions/10150468/how-to-redirect-cin-and-cout-to-files
    std::ofstream ofs;
    std::streambuf * coutbuf;
    if ( FileOut ) {
        ofs.open( file_out_path );
        coutbuf = std::cout.rdbuf(); //save old buf
        std::cout.rdbuf(ofs.rdbuf()); //redirect std::cout to out.txt
    }

    ifstream ifs;
    string line;
    vector<u2> sizes;
    vector<u2> connections;
    switch ( src ) {
        case RANDOM:
            switch ( structure ) {
                case BASIC:
                    g = new UnitIntervalGraph();
                    break;
                case PATH:
                    g = new Path();
                    break;
                case STAIRCASE:
                    g = new Staircase();
                    break;
                default:
                    throw InvalidCommandLineArgument();
                    break;
            } break;
            
        case FILE:
            ifs.open( file_in_path );

            getline( ifs, line );
            sizes = stringToVector( line );
            getline( ifs, line );
            connections = stringToVector( line );

            ifs.close();

            switch ( structure ) {
                case BASIC:
                    g = new UnitIntervalGraph( sizes, connections );
                    break;
                case PATH:
                    g = new Path( sizes, connections );
                    break;
                case STAIRCASE:
                    g = new Staircase( sizes, connections );
                    break;
                default:
                    throw InvalidCommandLineArgument();
                    break;
            } break;

        default:
            throw InvalidCommandLineArgument();
            break;
    }


    switch ( algorithm ) {
        case BF:
            g->BF_maxcut();
            break;
        case EO:
            g->EO_maxcut();
            break;
        case SI:
            g->SI_maxcut();
            break;
        default:
            throw InvalidCommandLineArgument();
            break;
    }



    // https://stackoverflow.com/questions/10150468/how-to-redirect-cin-and-cout-to-files
    if ( FileOut ) {
        ofs.close();
        std::cout.rdbuf(coutbuf); //reset to standard output again

        ifs.open( file_out_path );
        while( getline( ifs, line ) ) 
            cout << line << '\n';
        ifs.close();
    }

    return 0 ;
}




vector<u2> stringToVector( const string & in ) {

    vector<u2> out ;
    string next;

    istringstream iss(in);

    for( string next ; iss >> next ; ) {
        out.push_back( stoi( next ) );
    }
    
    return out;
}
