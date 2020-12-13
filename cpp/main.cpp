
// main.cpp //

#include "graph.h"

#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <fstream>
#include <sstream>
#include <string>

// #include "clipp/clipp.h"
// using namespace clipp;
using std::cout; using std::string;


std::vector<u2> stringToCut( const string & in );
void testMemory() ;
// void readFile(  const int & argc ) ;

int main( int argc, char ** argv ) { srand( time(NULL) );
    // testMemory() ;

    UnitIntervalGraph().ES_maxcut() ;
    
    cout << "done\n";
    return 0 ;
}

std::vector<u2> stringToCut( const string & in ) {

    std::vector<u2> out ;
    string next;

    std::istringstream iss(in);

    for( string next ; iss >> next ; ) {
        out.push_back( stoi( next ) );
    }
    
    return out;
}

void testMemory() {
    for ( ; true ; )
        UnitIntervalGraph g;
}

// void readFile(  const int & argc ) {
//     string line;
//     UnitIntervalGraph * graph_pointer;

    

//     if ( argc == 1 ) {
//         std::ifstream ifs( "graph.txt" );

//         std::getline( ifs, line );
//         std::vector<u2> sizes = stringToCut( line );
//         std::getline( ifs, line );
//         std::vector<u2> connections = stringToCut( line );

//         graph_pointer = new UnitIntervalGraph( sizes, connections );
//     }
//     else if ( argc == 2 ) {
//         graph_pointer = new UnitIntervalGraph();
//     }
//     else {
//         throw BadCommandParameters() ;      
//     }



//     graph_pointer->ES_maxcut();

//     cout << "Enter cut arrangment: (" << graph_pointer->k() << ")\n\n";

//     try {  while (true) {
//         std::getline( std::cin, line );
//         cout << "Cut size: " << graph_pointer->cutArrangement( stringToCut(line) ) << "\n\n";
//     }  } catch ( std::invalid_argument() ) { }
    
//     delete graph_pointer;
//     std::cout << "Done\n";

// }