
// graph.cpp //

#include "graph.h"


#include <iostream>
#include <stdlib.h>
#include <set>
#include <string>

using namespace std;




//////////
// EDGE //
//////////

edge::edge() {
    points[0] = NULL ;
    points[1] = NULL ;
}

void edge::assignPoints( std::shared_ptr<vertex> v1, std::shared_ptr<vertex> v2) {
    points[0] = v1 ;
    points[1] = v2 ;
}

void edge::print() const {
    points[0]->print() ;
    cout << " --- " ;
    points[1]->print() ;
}






////////////
// Vertex //
////////////

// O( v::degree )
std::shared_ptr<edge> vertex::connectsTo( const std::shared_ptr<vertex> & v ) const {
    for ( auto e : branches )
        if ( v == e->points[0] || v == e->points[1] )
            return e ;
    return NULL ;
}

void vertex::print() const {
    cout << "("<<name<<":"<<this<<", "<< ((side==left)?"L":"R") <<")" ;
}








///////////
// GRAPH //
///////////

// O( n )
bool Graph::hasVertex( const std::shared_ptr<vertex> & check ) const {
    for ( auto v : V )
        if ( v == check )
            return true;
    return false;
}


void Graph::print() const {
    cout << "graph here\n" ;
}



// if two verticies have a connection in their branches, the edge is added to E
// O( m·logm )
void Graph::updateEdges() {
    E.clear();
    set< std::shared_ptr<edge> > temp_edges ;

    // for all verticies
    for ( auto v : V )
        // go through each branch
        for ( auto e : v->branches ) {
            std::shared_ptr<edge> edge_pointer = e ;
            auto edge_it = temp_edges.find( edge_pointer ) ;

            if ( edge_it == temp_edges.end() ) { // if other vertex not already added, add to temp_edges
                temp_edges.insert( edge_pointer ) ;
            } else {                            // if found, erase it and add it to E
                temp_edges.erase( edge_it );
                E.push_back( edge_pointer );
            }

        }
    

}









///////////////////////
// UnitIntervalGraph //
///////////////////////

UnitIntervalGraph::UnitIntervalGraph() {

    vector<u2> sizes , connections ;

    u2 k = 3 + (rand() % 7) ;

    while ( sizes.size() < k )
        sizes.push_back( 1 + (rand() % 7) ) ;
    

    switch ( k ) {
        case (0) : throw BadConnectionsFormat() ;
        case (3) : connections.push_back(2) ;
        case (2) : connections.insert( connections.begin(), 1 ) ;
        default  : connections.push_back(1) ; break ;
    } while ( connections.size() < k ) { // case k > 3
        if      (   connections.size() == 0
                 || connections.size() == k-1 ) connections.push_back(1) ;
        else if (   connections.size() == 1
                 || connections.size() == k-2 ) connections.push_back(2) ;

        else if ( connections.back() == 1 )
            connections.push_back( 2 ) ;
        else if ( connections.back() == k+1-connections.size() )
            connections.push_back( connections.back()-1 ) ;
        
        else if ( connections.back() == k-connections.size() )
            connections.push_back( connections.back()-1 + (rand()%2) ) ;

        else
            connections.push_back( connections.back()-1 + (rand()%3) ) ;
    }

    fill_MC_and_TC( sizes, connections ) ;
    
}
UnitIntervalGraph::UnitIntervalGraph( const vector<u2> & sizes, const vector<u2> & connections ) {
    fill_MC_and_TC( sizes, connections ) ;
}


void UnitIntervalGraph::fill_MC_and_TC( const vector<u2> & sizes, const vector<u2> & connections ) {

    if ( MC.size() != 0 || TC.size() != 0 ) return ;


    if ( sizes.size() != connections.size() ) throw MismatchedParameters() ;
    if ( sizes.size() == 0 ) return ;

    if (   ( connections.at(0)  != 1 )
        || ( connections.back() != 1 )
    ) throw BadConnectionsFormat() ;
    

    // add the inputed number of twin classes
    for ( u2 tc = 0 ; tc < sizes.size() ; tc++ ) {
        TC.push_back( make_shared<TwinClass>() );

        // while the new twin class' size is less than the inputed size
        while ( TC.at(tc)->n() < sizes.at(tc) ) {
            
            // add vertex
            auto next_vertex = make_shared<vertex>();
            V.push_back( next_vertex ) ;

            // add it to sect and set its branches
            TC.at(tc)->addVertex( next_vertex ) ;

        }

    }



    MC.push_back( make_shared<MaximalClique>() );
    TC.at(0)->addToMaximalClique( MC.back() ) ;


    for ( u2 tc = 1 ; tc < connections.size() ; tc++ ) {
        if ( connections.at(tc) == 0 ) throw BadConnectionsFormat() ;

        // mc starts as the first MaximalCLique containing TC.at(tc-1)
        u2 mc = 0 ;
        while ( !MC.at(mc)->contains( TC.at(tc-1) ) ) mc++ ;

        char delta = connections.at(tc) - connections.at(tc-1) ;

        switch ( delta ) {
            case  1 : // add first mc
                TC.at(tc)->addToMaximalClique( MC.at(mc) ) ;
            case  0 : // push new mc
                MC.push_back( make_shared<MaximalClique>() );
            case -1 : // add remaining 
                mc++;
                while ( mc < MC.size() ) {
                    TC.at(tc)->addToMaximalClique( MC.at(mc) ) ;
                    mc++ ;
                }
                break;
            default: throw BadConnectionsFormat() ;
        }

    }

    cout << "sect size: ";
    for ( auto tc : TC ) cout << tc->n() << " "; cout << endl;
    cout << "connects : ";
    for ( auto tc : TC ) cout << tc->numConnections() << " "; cout << endl;

    updateEdges() ;

}

string line( const u2 & , const u2 & ) ;

void UnitIntervalGraph::print() const {
    cout << "                        TwinClass sizes :   " ;
    for ( u2 tc = 0 ; tc < TC.size() ; tc++ )
        cout << TC.at(tc)->n() << " " ; cout << "\n\n" ;

    cout << "Number of MCs Each TwinClass Belongs To :   ";
    for ( u2 tc = 0 ; tc < TC.size() ; tc++ )
        cout << TC.at(tc)->numConnections() << " " ; cout << endl ;
    
    string next_line = line( 0 , MC.at(0)->TC.size() ) ;
    while ( next_line.length() < 2*TC.size()+2 ) next_line += " " ;

    cout << "       Number of Sects in Each Mxml Clq : "
         << next_line << MC.at(0)->n() 
         << endl ;
    for ( u2 mc = 1, tc = 0 ; mc < MC.size() ; mc++ ) {
        while ( !TC.at(tc)->isMemberOf( MC.at(mc) )  ) tc++ ;

        next_line = line( tc, MC.at(mc)->TC.size() ) ;
        while ( next_line.length() < 2*TC.size()+2 ) next_line += " " ;

        cout << "                                          "
             << next_line << MC.at(mc)->n() 
             << endl ;        
    }
    // for ( u4 i = 0 ; i < k() ; i++ )
    //     cout << 
    //     TC.at(i)->n() * ( TC.at(i)->numBranches() - TC.at(i)->n() + 1 )
    //     << " " ; cout << endl ;
    // for ( u4 i = 0 ; i < k() ; i++ )
    //     cout << 
    //     ( TC.at(i)->n() * ( TC.at(i)->n() - 1) ) / 2
    //     << " " ; cout << endl ;
    // for ( u4 i = 0 ; i < k() ; i++ )
    //     cout << 
    //     ( TC.at(i)->n() * ( TC.at(i)->numBranches() - TC.at(i)->n() + 1 ) ) +
    //     ( ( TC.at(i)->n() * ( TC.at(i)->n() - 1) ) / 2 )
    //     << " " ; cout << endl ;
}

string line( const u2 & s , const u2 & mc_size ) {
    string out ;
    u2 length = 2*s ;
    while ( out.length() < length ) out += " " ;
    length += 2*mc_size - 1 ;
    while ( out.length() < length ) out += (out.length() % 2)? "-" : "o" ;
    return (char)(mc_size+'0') + (" " + out) ;
}






//  Path  //

Path::Path()
    : UnitIntervalGraph( _noParams_() ) {

    vector<u2> sizes , connections ;

    u2 k = 3 + (rand() % 7) ;

    while ( sizes.size() < k )
        sizes.push_back( 1 + (rand() % 7) ) ;

    if ( k == 0 || k == 2 ) throw BadConnectionsFormat() ;
    if ( k >= 1 ) connections.push_back(1) ;
    if ( k >= 2) connections.push_back(1) ;
    while ( connections.size() < k )
        connections.insert( connections.begin()+1, 2 ) ;

    fill_MC_and_TC( sizes, connections );

}
Path::Path( const std::vector<u2> & sizes, const std::vector<u2> & connections )
    : UnitIntervalGraph( _noParams_() ) {

    // double check that it is a path
    if (   connections.size()   == 0
        || connections.front()  != 1
        || connections.back()   != 1 )
        throw BadConnectionsFormat();
    for ( u1 i = 1; i < connections.size()-1; i++ )
        if ( connections.at(i) != 2 )
            throw BadConnectionsFormat();

    fill_MC_and_TC( sizes, connections );
}


// Staircase //

Staircase::Staircase()
    : UnitIntervalGraph( _noParams_() ) {

    vector<u2> sizes , connections ;

    u2 k = 3 + 2*(rand() % 5) ;

    while ( sizes.size() < k )
        sizes.push_back( 1 + (rand() % 7) ) ;

    if ( k == 0 || k == 2 ) throw BadConnectionsFormat() ;

    while ( connections.size() < k ) {
        connections.push_back( min(
            connections.size()+1    ,
            k - connections.size()
        ) ) ;
    }

    fill_MC_and_TC( sizes, connections );

}
Staircase::Staircase( const std::vector<u2> & sizes, const std::vector<u2> & connections )
    : UnitIntervalGraph( sizes, connections ) {

    cerr << "[WARNING]: Assuming that input is a staircase. No check was completed.\n";

}






////////////
// CLIQUE //
////////////

// O( C::n )
void Clique::addVertex( shared_ptr<vertex> & next ) {
    if ( hasVertex(next) ) return ;


    // for every vertex in the clique
    for ( auto v : V ) {
        shared_ptr<edge> new_edge = next->connectsTo( v ) ;
        // if a connection does not exist
        if ( !new_edge ) {
            // make new edge
            new_edge = make_shared<edge>() ;
            new_edge->assignPoints( v, next ) ;
 
            // add it to the vertex branches
            v->addBranch( new_edge ) ;
            next->addBranch( new_edge ) ;
        }
        // add edge to E
        E.push_back( new_edge ) ;
    }
    // add vertex to V
    V.push_back( next ) ;
 

}











///////////////////
// MAXIMALCLIQUE //
///////////////////

// O( S::n )
void MaximalClique::addSect( std::shared_ptr<TwinClass> & tc ) {
    // add all verticies in tc to V
    for ( auto v : tc->V )
        addVertex( v ) ;
    // add this to tc.MC;
    auto THIS = getThis();
    if ( !tc->isMemberOf( THIS ) )
        tc->MC.insert( THIS );
    // add tc to S
    if ( !contains( tc ) )
        TC.insert( tc ) ;
}

// O( log( MC::numTwinClasses ) )
bool MaximalClique::contains( std::shared_ptr<TwinClass> & tc ) const { 
    return ( TC.find( tc ) != TC.end() ) ;
}










///////////////
// TWINCLASS //
///////////////

// O( S::n )
void TwinClass::addToMaximalClique( std::shared_ptr<MaximalClique> & mc ) {
    auto THIS = getThis();
    mc->addSect( THIS ) ;
}

// O( log( S::numConnections ) )
bool TwinClass::isMemberOf( std::shared_ptr<MaximalClique> & mc ) const {
    return ( MC.find( mc ) != MC.cend() ) ;
}
