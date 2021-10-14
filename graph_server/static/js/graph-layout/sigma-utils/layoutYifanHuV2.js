sigma.utils.layoutYifanHuV2
    = function( nodes, edges, options ) {

    var opt = options || {}
        , nodes = nodes || []
        , edges = edges || []
        , forest
        , layoutBalanced = opt.layoutBalanced || 0
        , spaceGrid = opt.spaceGrid || {
            xSize: 50
            , ySize: 50
        }
        , results = []
        , newOpt = Object.assign( {}, opt, { layoutBalanced: 0 } )
        ;

    console.log( newOpt );

    forest = sigma.utils.getLayoutForest( nodes, edges );

    forest.forEach( function( tree ) {

        var subGraph = sigma.utils.getGraphFromTree( tree, edges )
            , result
            ;

        result = sigma.utils.layoutYifanHu( 
            subGraph.nodes
            , subGraph.edges
            , newOpt
        );

        results.push( result );

    } );

    if ( layoutBalanced ) {
        sigma.utils.layoutTreesByGrid(
            forest
            , {
                optimalDistance: opt.layoutBalancedSpace
                , readPrefix: 'yfh_'
                , spaceGrid: spaceGrid
            }
        );
    }

    return results;

};   
