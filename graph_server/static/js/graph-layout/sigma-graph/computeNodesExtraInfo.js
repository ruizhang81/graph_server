sigma.classes.graph.addMethod(
    'computeNodesExtraInfo'
    , function( options ){

    var me = this 
        , nodes = me.nodesArray
        , edges = me.edgesArray
        ;

    sigma.utils.computeNodesExtraInfo(
        nodes
        , edges
        , options
    );

    return me;
});  
