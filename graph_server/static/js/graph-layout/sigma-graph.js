sigma.classes.graph.addMethod( 'allNeighborIds', function( node ) {
    var nodeId = node
        , neighborsId = []
        ;
    if ( typeof node != 'string' ) {
        nodeId = nodeId.id;
    }

    for ( var key in this.allNeighborsIndex[ nodeId ] ) {
        neighborsId.push( key );
    } 
    return neighborsId;
} );  
sigma.classes.graph.addMethod( 'allNeighborNodes', function( node ) {
    var me = this
        , allNeighborIds = me.allNeighborIds( node )
        , allNeighbors = []
        ;

    allNeighborIds.forEach( function( nodeId ) {
        allNeighbors.push( me.nodes( nodeId ) );
    } );
    return allNeighbors;
} );
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
sigma.classes.graph.addMethod(
    'depthTravel'
    , function(root, callbacks){

    var me = this 
        , nodes = me.nodesArray
        , edges = me.edgesArray
        ;

    sigma.utils.depthTravel(
        nodes
        , edges
        , root 
        , callbacks
    );

    return me;

}); 

sigma.classes.graph.addMethod(
    'getCircleForest'
    , function(options){

    var me = this
        , nodes = me.nodesArray
        , edges = me.edgesArray
        ;

    return sigma.utils.getCircleForest(nodes, edges, options);
});    

sigma.classes.graph.addMethod(
    'getCircuits'
    , function(root){

    var me = this
        , nodes = me.nodesArray
        , edges = me.edgesArray
        , root = root || nodes[0]
        ;

    return sigma.utils.getCircuits(nodes, edges, root);

});  

sigma.classes.graph.addMethod(
    'getLayoutForest'
    , function(options){
    var me = this
        , opt = options || {}
        , g = opt.subGraph || me.getSubGraph(options)
        ;

    return sigma.utils.getLayoutForest(
        g.nodes
        , g.edges
        , options
    ); 
});    
sigma.classes.graph.addMethod(
    'getMaxDegreeNode'
    , function( options ){

    var me = this
        , opt = options || {}
        , g = opt.subGraph || me.getSubGraph( opt )
        ;

    return sigma.utils.getMaxDegreeNode( g.nodes, g.edges );        
});
sigma.classes.graph.addMethod(
    'getSubGraph'
    , function(options){
    var opt = options || {}
        , me = this
        , filter = opt.filter
        , edgeFilter = opt.edgeFilter
        , dummyRoot = opt.dummyRoot || null
        , dummyEdges = opt.dummyEdges || []
        , nodes = me.nodesArray.slice( 0 )
        , edges = me.edgesArray.slice( 0 )
        , _node_ids
        ;

    if('function' == typeof filter){
        nodes = [];
        edges = [];
        me.nodesArray.forEach(function(node){
            if(filter(node)){
                nodes.push(node);
            }
        });

        _node_ids = nodes.map(function(node){return node.id;});
        me.edgesArray.forEach(function(edge){
            if(_node_ids.indexOf(edge.source) >= 0 
                && _node_ids.indexOf(edge.target) >= 0){
                edges.push(edge);
            }
        });
    }

    if('function' == typeof edgeFilter){
        for(var i=edges.length-1; i>=0; i--){
            if(!edgeFilter(edges[i])){
                edges.splice(i, 1);
            }
        }
    }

    if ( dummyRoot && dummyEdges.length ) {
        nodes.push( dummyRoot );
        edges = edges.concat( dummyEdges );
    }

    return {
        nodes: nodes
        , edges: edges
    };
}); 
sigma.classes.graph.addMethod(
    'sortByNodesDegree'
    , function(reverse){
    var nodes = this.nodesArray
        , me = this
        ;

    nodes.forEach(function(node){
        node.degree = me.degree(node.id);
    });
    nodes.sort(function(a, b){
        return reverse 
            ? b.degree - a.degree
            : a.degree - b.degree;
    });

    return this;
}); 

sigma.classes.graph.addMethod(
    'widthTravel'
    , function(root, callbacks, excludes){

    var me = this
        , nodes = me.nodesArray
        , edges = me.edgesArray
        , root = root || nodes[0]
        , cbs = callbacks || {}
        ;

    sigma.utils.widthTravel(
        nodes     
        , edges
        , root
        , cbs
        , excludes
    );

    return me;

});     

