sigma.prototype.layoutNearby
    = function( options ) {
    var me = this
        , opt = options || {}
        , selectedFilter = opt.filter || function( node ) {
            return node.selected;
        }
        , nonselectedFilter = function( node ) {
            return !selectedFilter( node );
        }
        , subGraphFilter = opt.subGraphFilter
        , selectedGraph = me.graph.getSubGraph( { filter: selectedFilter } )
        , nonselectedGraph
        , subGraph
        ;

    if ( selectedGraph.nodes.length == 0 ) {
        return me;
    }

    me.initializeLayout();

    if ( 'function' == typeof subGraphFilter ) {
        subGraph = me.graph.getSubGraph( { filter: subGraphFilter } );
        subGraph.nodes.forEach( function( node ) {
            if ( !selectedFilter( node ) ) {
                node.fixed = true;
            }
        } );
    }
    else {
        nonselectedGraph = me.graph.getSubGraph( { filter: nonselectedFilter } );
        nonselectedGraph.nodes.forEach( function( node ) {
            node.fixed = true;
        } );
    }

    return me.layoutYifanHu( {
        skipInitialization: 1
        , filter: subGraphFilter
        , skipPreLayoutCheck: opt.skipPreLayoutCheck || 0
        , optimalDistance: opt.optimalDistance || 500
        , readPrefix: opt.readPrefix || 'yfh_'
        , maxIterations: opt.maxIterations || 50
        , relativeStrength: opt.relativeStrength || 0.2
        , layoutBalanced: opt.layoutBalanced || 0
        , layoutBalancedSpace: opt.layoutBalancedSpace || 300
        , spaceGrid: opt.spaceGrid || { xSize: 50, ySize: 50 }
    } );
};
