sigma.prototype.getShortestPath = function( from, to, options ) {
    var me = this
        , graph = me.graph
        , nodes = graph.nodes()
        , edges = graph.edges()
        , pathGraph
        ;

    me.initializeLayout( { clearOnly: 1 } );
    pathGraph = sigma.utils.getShortestPath(
        nodes
        , edges
        , from
        , to
        , options
    );
    me.initializeLayout( { clearOnly: 1 } );
    return pathGraph;
};  
