sigma.utils.getLayoutForest
    = function(nodes, edges, options){

    var opt = options || {}
        ;

    if ( opt.makeMaxDegreeNodeRoot ) {
        nodes = sigma.utils.sortByNodesDegree( nodes, edges, 1 );
    }

    var nodesVisited = {}
        , forest = []
        , node = opt.dummyRoot 
            || opt.root 
            || nodes[ 0 ] 
        , excludes = opt.excludes
        ;

    do {
        sigma.utils.widthTravel(
            nodes
            , edges
            , node
            , {
                onNode: function(__node){
                    nodesVisited[__node.id] = true;
                    if ( 'function' == typeof opt.sortChildren ) {
                        __node._wt_children.sort( opt.sortChildren );
                    }
                    else {
                        // take `dummyNode` into considerations
                        __node._wt_children.sort( function( a, b ) {
                            var _ea = a._extra_info
                                , _eb = b._extra_info
                                , _eaL, _ebL
                                ;

                            if ( !_ea || !_eb ) {
                                return 0;
                            }

                            _eaL = _ea.edgeLabels;
                            _ebL = _eb.edgeLabels;
                            if ( !_eaL || !_ebL ) {
                                return 0;
                            }

                            _eaL = _eaL[ __node.id ];
                            _ebL = _ebL[ __node.id ];
                            if ( !_eaL || !_ebL ) {
                                return 0;
                            }

                            return _eaL.localeCompare( _ebL );
                        } );
                    }

                    // do extra processing using `onNode` callback
                    if ( 'function' == typeof opt.onNode ) {
                        opt.onNode( __node );
                    }
                }
            } 
            , excludes
        )
        forest.push(node);
        ;
    } while((node = hasMore()));

    function hasMore(){
        for(var i=0; i<nodes.length; i++){
            if(!nodesVisited[nodes[i].id]){
                return nodes[i];
            }
        }
        return 0;
    }

    return forest;
};
