
// main.cpp //


#include "graph.h"

#include <iostream>
#include <time.h>
#include <fstream>
#include <string>

#include "_src/clipp/clipp.h"

using namespace std;
using namespace clipp;




vector<u2> stringToVector( const string & in );

int main( int argc, char ** argv ) { srand( time(NULL) );

    enum Src        { RANDOM, FILE };
    enum Structure  { BASIC, PATH, STAIRCASE };

    class InvalidCommandLineArgument { };

    Src src = RANDOM;
    string file_in_path = "";

    Structure structure = BASIC;

    bool BF = true;
    bool EO = false;
    bool SI = false;

    bool FileOut = true;
    string file_out_path = "out.txt";

    auto cli = ( (
            option("-r", "--random").set(src, RANDOM)
                % "Randomize the Unit Interval Graph"
            |
            option("-fi", "--file-in").set(src, FILE) & value("file_in_path", file_in_path)
                % "Graph is loaded from file"
        ) % "Graph Source",

        (
            option("-b", "--basic").set(structure, BASIC)
                % "No restrictions on structure"
            |
            option("-p", "--path").set(structure, PATH)
                % "Path structure"
            |
            option("-s", "--staircase").set(structure, STAIRCASE)
                % "Staircase structure"
        ) % "Graph Structure",

        (
            option("-bf", "--brute-force").set(BF)
                % "Run brute-force algorithm",

            option("-eo", "--every-other").set(EO)
                % "Run every-other approx algorithm",

            option("-si", "--sifferman").set(SI)
                % "Run Sifferman alg (only on path)"
        ) % "Max-Cut Algorithm",

        (
            option("-fo", "--file-out").set(FileOut) & value("Output file path", file_out_path)
                % "Change the path of the output file"
        ) % "File Out Path"

    );

    if( !parse(argc, argv, cli) ) {
        cout << make_man_page( cli, argv[0] );
        return -1;
    }

    // cout << "src: " << src << endl;
    // cout << "file_in_path: " << file_in_path << endl;
    // cout << "structure: " << structure << endl;
    // cout << "FileOut: " << FileOut << endl;
    // cout << "file_out_path: " << file_out_path << endl;

    // return 0;

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
                    cout << "new Path();\n";
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
                    cout << "new Path( sizes, connections );\n";
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

    g->print();
    cout << "\n\n\n";

    if ( BF ) {
        try {
            g->BF_maxcut();
            cout << "\n\n\n";
        } catch ( Graph::GraphTooLarge() ) {
            cout << "Graph too large. Try a smaller input.\n";
        }
    }
    if ( EO ) {
        g->EO_maxcut();
        cout << "\n\n\n";
    }
    if ( SI ) {
        g->SI_maxcut();
        cout << "\n\n\n";
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

    delete g;

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
