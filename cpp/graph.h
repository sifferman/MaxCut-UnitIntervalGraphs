
// graph.h //

#ifndef GRAPH_H
#define GRAPH_H

#define u1 unsigned char
#define u2 unsigned short
#define u4 unsigned int

#include <vector>
#include <set>
#include <math.h>
#include <memory>


struct edge ;
struct vertex ;

struct edge {
    edge() ;

    std::shared_ptr<vertex> points[2] ;

    void assignPoints( std::shared_ptr<vertex>, std::shared_ptr<vertex> ) ;
    void print() const ;
};
struct vertex {
    vertex() { name = rand() ; }

    std::set< std::shared_ptr<edge> > branches ;
    u2 name ;

    void addBranch( std::shared_ptr<edge> e ) { branches.insert( e ); }
    std::shared_ptr<edge> connectsTo( const std::shared_ptr<vertex> & ) const ;
    void print() const ;
    
    // max cut methods
    enum Side : u1 { left, right } ;
    mutable Side side ;
};






class Graph {
    public:
        Graph() { }

        bool hasVertex( const std::shared_ptr<vertex> & ) const ;
        u2 n() const { return V.size(); }
        void print() const ;

        // maxcut algorithm
        class GraphTooLarge { } ;
        // u2 BF_maxcut() const ;     // 2^n

    protected:
        u2 cut() const ;
        void updateEdges() ;
        std::set< std::shared_ptr<vertex> > V ;
        std::vector< std::shared_ptr<edge> > E ;
        
};





class Clique ;
class MaximalClique ;
class Sect ;



class UnitIntervalGraph : public Graph {
    public:
        UnitIntervalGraph() ;
        UnitIntervalGraph( const std::vector<u2> & sizes, const std::vector<u2> & connections );

        u2 t() const { return MC.size() ; }
        u2 k() const { return  S.size() ; }
        void print() const ;

        void makeMaximalCliques() const ;
        void makeSects() const ;
        
        class MismatchedParameters { } ;
        class BadConnectionsFormat { } ;
        u2 cutArrangement( const std::vector<u2> & cut ) const;
        u2 ES_maxcut() const ;     // (s+1)^k
        // u2 EO_maxcut() const ;     // poly INCORRECT

    protected:
        void fill_MC_and_S( const std::vector<u2> & sizes, const std::vector<u2> & connections ) ;

        mutable std::vector< std::shared_ptr<MaximalClique> > MC ;
        mutable std::vector< std::shared_ptr<Sect> > S ;


} ;







class Clique : public Graph {
    public:
        void addVertex( std::shared_ptr<vertex> & );
} ;


class MaximalClique : public Clique,
public std::enable_shared_from_this<MaximalClique> {
    public:
        void addSect( std::shared_ptr<Sect> & ) ;
        bool contains( std::shared_ptr<Sect> & ) const ;
        u2 numSects() const { return S.size(); }
    private:
        std::set< std::shared_ptr<Sect> > S ;
        friend class UnitIntervalGraph ;
        friend class Sect ;
        std::shared_ptr<MaximalClique> getThis() { return shared_from_this(); }
} ;


class Sect : public Clique,
public std::enable_shared_from_this<Sect> {
    public:
        void addToMaximalClique( std::shared_ptr<MaximalClique> & ) ;
        bool isMemberOf( std::shared_ptr<MaximalClique> & ) const ;
        u2 numConnections() const { return MC.size() ; }
        u2 numBranches() const { return (*V.begin())->branches.size() ; }
    private:
        std::set< std::shared_ptr<MaximalClique> > MC ;
        friend class UnitIntervalGraph ;
        friend class MaximalClique ;
        std::shared_ptr<Sect> getThis() { return shared_from_this(); }
} ;



#endif
