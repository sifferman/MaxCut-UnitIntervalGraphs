
// maxcut_algorithms.cpp //

#include "graph.h"

#include <math.h>
#include <iostream>

using namespace std;


u2 Graph::cut() const {
    u2 out = 0;
    for ( auto e : E )
        if ( e->points[0]->side != e->points[1]->side )
            out++ ;
    return out ;
}

u2 UnitIntervalGraph::cutArrangement( const vector<u2> & cut ) const {

    if ( cut.size() != k() )
        throw MismatchedParameters();

    for ( u2 tc = 0 ; tc < k() ; tc++ ) {
        for ( u2 v = 0 ; v < TC.at(tc)->n() ; v++ ) {
            TC.at(tc)->V.at(v)->side = (vertex::Side)( v < cut.at(tc) );
        }
    }


    return this->cut();
}




///////////////////////
// Graph Brute Force //
///////////////////////

// Brute force Max-Cut algorithm. Time complexity: O*( 2^v ).
u2 Graph::BF_maxcut() const {
    if ( V.size() > 22 ) throw GraphTooLarge() ;

    cout << "#####################\n"
         << "# Graph Brute Force #\n"
         << "#####################\n\n";

    u4 max = 0 ;
    u2 cut = 0 ;
    size_t arrangement = 0 ;

    u2 v_i;


    // for each vertex arrangement, arrange each vertex then store the cut-value if it is bigger than max
    for ( size_t i = 0; i < pow( 2, V.size() ); i++ ) {
        for ( u2 v = 0; v < V.size(); v++ )
            V.at(v)->side = (vertex::Side) (( i >> v ) & 1) ;
        cut = this->cut();
        if ( cut > max ) { max = cut ; arrangement = i ; }
    }

    // revert to maximal arrangement
    for ( u2 v = 0; v < V.size(); v++ )
        V.at(v)->side = (vertex::Side) (( arrangement >> v ) & 1) ;

    // return max
    cout << "Max-Cut: " << max << endl;
    return max;
}




// Initialize Maximal Cliques.
void UnitIntervalGraph::makeMaximalCliques() const {
    // for each V[j], make maximal clique
    // push if it does not already exist
    cerr << "[WARNING]: Assuming that Unit Interval Graph already has its Maximal Cliques stored.\n";
}


// Initialize Sects.
void UnitIntervalGraph::makeTwinClasses() const {
    makeMaximalCliques() ;
    // for each C[j], remove all verticies in another clique
    // push
    cerr << "[WARNING]: Assuming that Unit Interval Graph already has its Twin Classes stored.\n";

}






// Max-Cut algorithm. Time complexity: O*( (s+1)^k ).
u2 UnitIntervalGraph::BF_maxcut() const {

    cout << "#############################\n"
         << "# Unit Interval Brute Force #\n"
         << "#############################\n\n";


    makeTwinClasses() ;
    const u2 num_twinclasses = TC.size() ;

    // MAX_ASSIGNMENT is the number of comparisons to make
    size_t MAX_ASSIGNMENT = 1 ;
    for ( u2 tc = 0; tc < num_twinclasses; tc++ ) MAX_ASSIGNMENT *= TC.at(tc)->n()+1 ;

    // cout << "Number of Comparisons to Make: " << MAX_ASSIGNMENT << endl ;

    if ( MAX_ASSIGNMENT >= ( pow(2,22) ) ) throw GraphTooLarge() ;

    vector<size_t> solutions ;
    u4 max = 0 ;

    u2 cuts[num_twinclasses] ;


    size_t assignment ;
    size_t best_assignment ;
    for ( assignment = 0; assignment < MAX_ASSIGNMENT; assignment++ ) {
        // according to assignment, arrange G into L and R
        size_t each_assignment = assignment ;
        for ( u2 tc = 0; tc < num_twinclasses; tc++ ) {
            cuts[tc] = each_assignment % (TC.at(tc)->n()+1) ;
            each_assignment /= TC.at(tc)->n()+1 ;
        }
        for ( u2 tc = 0; tc < num_twinclasses; tc++ )
            for ( u2 v = 0; v < TC.at(tc)->n(); v++ )
                TC.at(tc)->V.at(v)->side = (vertex::Side)( v < cuts[tc] ) ;

        // cut
        u2 cut = this->cut();
        // save the result if it is equal
        if ( cut == max ) { solutions.push_back( assignment ); }
        // replace the result if it is greater
        if ( cut > max ) {
            // cout << " replacing " << max << " with " << cut << endl;

            max = cut;
            best_assignment = assignment;
            solutions.clear() ;
            solutions.push_back( assignment ) ;
        }
    }

    // according to found best_assignment, arrange G into L and R so that G can be looked at later
    for ( u2 tc = 0; tc < num_twinclasses; tc++ ) {
        cuts[tc] = best_assignment % (TC.at(tc)->n()+1) ;
        best_assignment /= TC.at(tc)->n()+1 ;
    }
    for ( u2 tc = 0; tc < num_twinclasses; tc++ )
        for ( u2 v = 0; v < TC.at(tc)->n(); v++ )
            TC.at(tc)->V.at(v)->side = (vertex::Side)( v < cuts[tc] ) ;

    cout << "Printing all solutions: \n";
    for ( size_t i = 0 ; i < solutions.size() ; i++ ) {
        size_t each_assignment = solutions.at( i ) ;
        for ( u2 tc = 0; tc < num_twinclasses; tc++ ) {
            cout << ( each_assignment % (TC.at(tc)->n()+1) ) << " " ;
            each_assignment /= TC.at(tc)->n()+1 ;
        }
        cout << endl ;
    }
    cout << "Max-Cut: " << max << endl;


    return max;
}


// // Every-Other 1/2 Appropriation Algorithm for sorted Unit Interval Graphs
// O( v )
u2 UnitIntervalGraph::EO_maxcut() const {

    cout << "#############################\n"
         << "# Every-Other Approximation #\n"
         << "#############################\n\n";


    u1 A = 0;
    for ( auto v : V ) {
        v->side = (vertex::Side) A;
        A = ~A;
    }

    cout << "Printing all Solutions:\n (you may round each non-integer up or down)\n";
    for ( auto tc : TC )
        cout << (float)tc->n()/2 << " ";
    cout << endl;

    cout << endl;

    u2 approx = cut();
    cout << "Max-Cut / 2 <= " << approx << " <= Max-Cut\n";
    return approx;
}








// Sifferman Algorithm on Path Case (unproven) O(n)
u2 Path::SI_maxcut() const {
    vector<float> arrangement;

    cout << "##################\n"
         << "# Sifferman Path #\n"
         << "##################\n\n";



    for ( u1 tc = 0; tc < k(); tc++ ) {
        if        ( tc == 0     ) {
            arrangement.push_back( min(
                (float) TC.at(tc)->n(),
                (( (float) TC.at( tc )->n() + TC.at( tc+1 )->n() ) / 2)
            ) );
        } else if ( tc == k()-1 ) {
            arrangement.push_back( min(
                (float) TC.at(tc)->n(),
                (( (float) TC.at( tc-1 )->n() + TC.at( tc )->n() ) / 2)
            ) );
        } else {
            arrangement.push_back( min(
                (float) TC.at(tc)->n(),
                (( (float) TC.at( tc-1 )->n() + TC.at( tc )->n() + TC.at( tc+1 )->n() ) / 2)
            ) );
        }
    }

    // invert
    for ( u2 tc = 0; tc < k(); tc += 2 )
        arrangement.at(tc) = TC.at(tc)->n() - arrangement.at(tc);


    // print solutions
    cout << "Printing all Solutions:\n (you may round each non-integer up or down)\n";

    for ( u2 tc = 0; tc < k(); tc++ )
        cout << arrangement.at(tc) << " ";
    cout << endl;

    for ( u2 tc = 0; tc < k(); tc++ )
        cout << TC.at(tc)->n() - arrangement.at(tc) << " ";
    cout << endl;

    cout << endl;

    // print cut value
    vector<u2> cut;
    for ( u2 tc = 0; tc < k(); tc++ )
        cut.push_back( floor( arrangement.at(tc) ) );
    u2 cut_value = cutArrangement( cut );

    cout << "Max-Cut: " << cut_value << endl;

    return cut_value;
}
