sigma.prototype.getNodesWithinDistance 
    = function( from, distance, options ) {
    var me = this
        , graph = me.graph
        , nodes = graph.nodes()
        , edges = graph.edges()
        , nodesWithinDistance
        ;

    me.initializeLayout( { clearOnly: 1 } );
    nodesWithinDistance = sigma.utils.getNodesWithinDistance(
        nodes
        , edges
        , from
        , distance
        , options
    );
    me.initializeLayout( { clearOnly: 1 } );
    return nodesWithinDistance;
};  
