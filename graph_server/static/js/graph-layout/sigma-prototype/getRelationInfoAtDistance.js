sigma.prototype.getRelationInfoAtDistance 
    = function( from, distance, options ) {
    var me = this
        , graph = me.graph
        , nodes = graph.nodes()
        , edges = graph.edges()
        , relationInfo
        ;

    me.initializeLayout( { clearOnly: 1 } );
    relationInfo = sigma.utils.getRelationInfoAtDistance(
        nodes
        , edges
        , from
        , distance
        , options
    );
    me.initializeLayout( { clearOnly: 1 } );
    return relationInfo;
};  
