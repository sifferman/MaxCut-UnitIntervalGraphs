
// main.cpp //

#include "graph.h"

#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <fstream>
#include <sstream>
#include <string>


using namespace std;

class BadCommandParameters { } ;

vector<u2> stringToCut( const string & in );
void testMemory() ;
void readFile(  const int & argc ) ;

int main( int argc, char ** argv ) { srand( time(NULL) );

    // vector<u2> sizes       = stringToCut( "1 2 1" );
    // vector<u2> connections = stringToCut( "1 2 1" );
    // PerfectStaircase( sizes ).ES_maxcut();

    // readFile( argc ) ;
    // testMemory() ;

    UnitIntervalGraph().ES_maxcut() ;
    

    return 0 ;
}

vector<u2> stringToCut( const string & in ) {

    vector<u2> out ;
    string next;

    istringstream iss(in);

    for( string next ; iss >> next ; ) {
        out.push_back( stoi( next ) );
    }
    
    return out;
}

void testMemory() {
    for ( size_t i = 1 ; true ; i++ )
        UnitIntervalGraph g;
}

void readFile(  const int & argc ) {
    string line;
    UnitIntervalGraph * graph_pointer;

    

    if ( argc == 1 ) {
        ifstream ifs( "graph.txt" );

        getline( ifs, line );
        vector<u2> sizes = stringToCut( line );
        getline( ifs, line );
        vector<u2> connections = stringToCut( line );

        graph_pointer = new UnitIntervalGraph( sizes, connections );
    }
    else if ( argc == 2 ) {
        graph_pointer = new UnitIntervalGraph();
    }
    else {
        throw BadCommandParameters() ;      
    }



    graph_pointer->ES_maxcut();

    cout << "Enter cut arrangment: (" << graph_pointer->k() << ")\n\n";

    try {  while (true) {
        getline( cin, line );
        cout << "Cut size: " << graph_pointer->cutArrangement( stringToCut(line) ) << "\n\n";
    }  } catch ( invalid_argument() ) { }
    
    delete graph_pointer;
    cout << "Done\n";

}